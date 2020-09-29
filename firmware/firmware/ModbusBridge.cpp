#include <Streaming.h>
#include "./Addon.h"
#include "ModbusRtu.h"

#include "ModbusBridge.h"

#define RS485Serial 3

#define MasterModbusAdd 0
#define SlaveModbusAdd 1

#define ku8MBReadCoils 0x01          ///< Modbus function 0x01 Read Coils
#define ku8MBReadDiscreteInputs 0x02 ///< Modbus function 0x02 Read Discrete Inputs
#define ku8MBWriteSingleCoil 0x05    ///< Modbus function 0x05 Write Single Coil
#define ku8MBWriteMultipleCoils 0x0F ///< Modbus function 0x0F Write Multiple Coils

// Modbus function codes for 16 bit access
#define ku8MBReadHoldingRegisters 0x03       ///< Modbus function 0x03 Read Holding Registers
#define ku8MBReadInputRegisters 0x04         ///< Modbus function 0x04 Read Input Registers
#define ku8MBWriteSingleRegister 0x06        ///< Modbus function 0x06 Write Single Register
#define ku8MBWriteMultipleRegisters 0x10     ///< Modbus function 0x10 Write Multiple Registers
#define ku8MBMaskWriteRegister 0x16          ///< Modbus function 0x16 Mask Write Register
#define ku8MBReadWriteMultipleRegisters 0x17 ///< Modbus function 0x17 Read Write Multiple Registers

Modbus master(MasterModbusAdd, RS485Serial, 0);
modbus_t ModbusQuery[1];
uint16_t ModbusSlaveRegisters[8];
millis_t WaitingTime;

enum MBB_STATE
{
   WAITING = 0,
   QUERY = 1,
   RESPONSE = 2
};

int _state = 0;

#define MX2_STATE 0x0003    // (2 bytes) Status of the inverter: [0: Initial state, 2: Stop 3: Turn 4: Coast stop 5: Jog 6: DC braking]
#define MX2_DIR 0x1004      // (2 bytes) Status of the inverter: [0: Initial state, 2: Stop 3: Turn 4: Coast stop 5: Jog 6: DC braking]
#define MX2_AMPERAGE 0x1003 // (2 bytes) Output current monitoring (0,01 [A])

#define MX2_START 0x0001     // (bit) Run command 1: Run, 0: Stop (valid with A002 = 03)
#define MX2_SET_DIR 0x0002   // (bit) Command of direction of rotation 1: Reverse rotation, 0: Rotation in the forward direction (valid with A002 = 03)
#define MX2_RESET 0x0004     // (bit) Reset emergency shutdown (RS) 1: Reset
#define MX2_READY 0x0011     // (bit) Ready IF 1: Ready, 0: Not ready
#define MX2_DIRECTION 0x0010 // (bit) Direction of rotation 1: Reverse rotation, 0: Rotation in the forward direction (deadlock with "d003")
#define TEST_NUMBER 1234 // Verification code for function 0x08
#define ku8MBLinkTestOmronMX2Only       0x08 ///< Modbus function 0x08 Тест связи с инвертром Omron MX2 функция только для него ku8MBLinkTestOmronMX2Only
short ModbusBridge::setup()
{
   // Serial.println("---------- setup ----");
   master.begin(19200);     // baud-rate at 19200
   master.setTimeOut(5000); // if there is no answer in 5000 ms, roll over
   WaitingTime = millis() + 400;
   /*
   fn = MX2_STATE;
   addr = MB_FC_READ_REGISTERS;
   */
   fn = ku8MBLinkTestOmronMX2Only;
   addr = 0;
   nb = 4;
}
short ModbusBridge::run(short val){
   fn = ku8MBWriteSingleCoil;
   addr = MX2_START;
//   _u16WriteQty = (u8State ? 0xFF00 : 0x0000);
   ModbusQuery[0].au16reg = 0xFF00;
   ModbusQuery[0].u16CoilsNo  = 1;
}
short ModbusBridge::setFn(short val)
{
   // fn = val;
   run(0);
}
short ModbusBridge::setAddr(short val)
{
   addr = val;
}
short ModbusBridge::setNb(short val)
{
   nb = val;
}
short ModbusBridge::loop()
{
   switch (_state)
   {

   case WAITING:
   {
      if (millis() > WaitingTime)
      {
         _state++; // wait state
      }
      break;
   }
   case QUERY:
   {
      /*ModbusQuery[0].u8id = SlaveModbusAdd;               // slave address
      ModbusQuery[0].u8fct = MB_FC::MB_FC_READ_REGISTERS; // function code (this one is registers read)
      ModbusQuery[0].u16RegAdd = MX2_AMPERAGE;                 // start address in slave
      ModbusQuery[0].u16CoilsNo = 4;                      // number of elements (coils or registers) to read
      ModbusQuery[0].au16reg = ModbusSlaveRegisters;      // pointer to a memory array in the CONTROLLINO
*/
      ModbusQuery[0].u8id = SlaveModbusAdd;          // slave address
      ModbusQuery[0].u8fct = fn;                     // function code (this one is registers read)
      ModbusQuery[0].u16RegAdd = addr;               // start address in slave
      ModbusQuery[0].u16CoilsNo = nb;                // number of elements (coils or registers) to read
      ModbusQuery[0].au16reg = ModbusSlaveRegisters; // pointer to a memory array in the CONTROLLINO

      master.query(ModbusQuery[0]); // send query (only once)
      _state++;
      break;
   }
   case RESPONSE:
   {
      master.poll(); // check incoming messages
      if (master.getState() == COM_IDLE)
      {
         // response from the slave was received
         _state = 0;
         WaitingTime = millis() + 300;
         // registers read was proceed
         Serial.println("---------- READ RESPONSE RECEIVED ----");
         Serial.print(" ADC0: 0x");
         Serial.print(ModbusSlaveRegisters[0], HEX);
         Serial.print(" , Digital0: ");
         Serial.print(ModbusSlaveRegisters[1], HEX);
         Serial.print(" , nb in ");
         Serial.print(ModbusSlaveRegisters[2], DEC);
         Serial.print(" , nb out ");
         Serial.println(ModbusSlaveRegisters[3], DEC);
         Serial.println("");
      }
      break;
   }
   }
}

void ModbusBridge::debug(Stream *stream)
{
   // *stream << this->name << ":";
}

void ModbusBridge::info(Stream *stream)
{
   // *stream << this->name << "\n\t";
}