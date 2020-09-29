#include <Controllino.h> /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include "ModbusRtu.h"   /* Usage of ModBusRtu library allows you to implement the Modbus RTU protocol in your sketch. */

// This MACRO defines Modbus master address.
// For any Modbus slave devices are reserved addresses in the range from 1 to 247.
// Important note only address 0 is reserved for a Modbus master device!

#define MasterModbusAdd 0
#define SlaveModbusAdd 1

// This MACRO defines number of the comport that is used for RS 485 interface.
// For MAXI and MEGA RS485 is reserved UART Serial3.
#define RS485Serial 3

// The object ControllinoModbuSlave of the class Modbus is initialized with three parameters.
// The first parametr specifies the address of the Modbus slave device.
// The second parameter specifies type of the interface used for communication between devices - in this sketch - RS485.
// The third parameter can be any number. During the initialization of the object this parameter has no effect.
Modbus ControllinoModbusMaster(MasterModbusAdd, RS485Serial, 0);

// This uint16 array specified internal registers in the Modbus slave device.
// Each Modbus device has particular internal registers that are available for the Modbus master.
// In this example sketch internal registers are defined as follows:
// (ModbusSlaveRegisters 0 - 3 read only and ModbusSlaveRegisters 4 - 7 write only from the Master perspective):
// ModbusSlaveRegisters[0] - Read an analog value from the CONTROLLINO_A0 - returns value in the range from 0 to 1023.
// ModbusSlaveRegisters[1] - Read an digital value from the CONTROLLINO_D0 - returns only the value 0 or 1.
// ModbusSlaveRegisters[2] - Read the number of incoming messages - Communication diagnostic.
// ModbusSlaveRegisters[3] - Read the number of number of outcoming messages - Communication diagnostic.
// ModbusSlaveRegisters[4] - Sets the Relay output CONTROLLINO_R0 - only the value 0 or 1 is accepted.
// ModbusSlaveRegisters[5] - Sets the Relay output CONTROLLINO_R1 - only the value 0 or 1 is accepted.
// ModbusSlaveRegisters[6] - Sets the Relay output CONTROLLINO_R2 - only the value 0 or 1 is accepted.
// ModbusSlaveRegisters[7] - Sets the Relay output CONTROLLINO_R3 - only the value 0 or 1 is accepted.
uint16_t ModbusSlaveRegisters[8];

// This is an structe which contains a query to an slave device
modbus_t ModbusQuery[2];

uint8_t myState;      // machine state
uint8_t currentQuery; // pointer to message query

unsigned long WaitingTime;

void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(19200);
  Serial.println("-----------------------------------------");
  Serial.println("CONTROLLINO Modbus RTU Master Test Sketch");
  Serial.println("-----------------------------------------");
  Serial.println("");

#define MX2_STATE 0x0003        // (2 bytes) Status of the inverter
#define MX2_TARGET_FR 0x0001    // (4 bytes) Source (reference) of the frequency reference (0,01 [Hz])
#define MX2_ACCEL_TIME 0x1103   // (4 bytes) Acceleration time (cm compressor) in 0.01 sec
#define MX2_DEACCEL_TIME 0x1105 // (4 bytes) Braking time (cm compressor) in 0.01 sec

#define MX2_CURRENT_FR 0x1001 // (4 bytes) Output frequency control (0,01 [Hz])
#define MX2_AMPERAGE 0x1003   // (2 bytes) Output current monitoring (0,01 [A])
#define MX2_VOLTAGE 0x1011    // (2 bytes) Monitoring output voltage 0.1 [V]
#define MX2_POWER 0x1012      // (2 bytes) Power control 0.1 [kW]
#define MX2_POWER_HOUR 0x1013 // (4 bytes) Watt-hour control 0.1 [kW / h]
#define MX2_HOUR 0x1015       // (4 bytes) Control of operating time in the "Run" mode 1 [h]
#define MX2_HOUR1 0x1017      // (4 bytes) Monitoring of running hours with power on 1 [h]
#define MX2_TEMP 0x1019       // (2 bytes) Radiator temperature control (0.1 degree) -200 ... 1500
#define MX2_VOLTAGE_DC 0x1026 // (2 bytes) DC voltage control (PN) 0.1 [V]
#define MX2_NUM_ERR 0x0011    // (2 bytes) Trip counter 0 ... 65530
#define MX2_ERROR1 0x0012     // (20 bytes) Description 1 trip the remaining 5 lie sequentially behind the first address error are calculated MX2_ERROR1 + i * 0x0a
#define MX2_INIT_DEF 0x1357   // (2 bytes) Set the initialization mode to 0 (nothing), 1 (clearing the shutdown history), 2 (clearing the shutdown history and initializing data), 4 (clearing the shutdown history, initializing data and the program EzSQ)
#define MX2_INIT_RUN 0x13b7   // (2 bytes) Initialization start 0 (off), 1 (on)

#define MX2_SOURCE_FR 0x1201  // (2 bytes) Frequency reference source
#define MX2_SOURCE_CMD 0x1202 // (2 bytes) Command source
#define MX2_BASE_FR 0x1203    // (2 bytes) Main frequency 300 ... "maximum frequency" 0.1 Hz
#define MX2_MAX_FR 0x1204     // (2 bytes) Maximum frequency 300 ... 4000 (10000) 0.1 Hz
#define MX2_DC_BRAKING 0x1245 // (2 bytes) Enable DC Braking
#define MX2_STOP_MODE 0x134e  // (2 bytes) Choosing a stop method B091 = 01
#define MX2_MODE 0x13ae       // (2 bytes) IF mode selection b171 = 03
#define MX2_A002 0x1202       // A002 Source of the “Run” command 01 .. Control terminals 02 ... “Run” key on the keypad or digital panel 03 ... Input via the ModBus network 04 ... Add. card = 01
#define MX2_A003 0x1203       // A003 Main frequency Can be set in the range from 30 Hz to the maximum frequency (A004) (0.1 Hz) = 120 * 10
#define MX2_A004 0x1204       // A004 Maximum frequency Can be set in the range from the fundamental frequency to 400 Hz (0.1 Hz) = 120 * 10

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
#define ku8MBLinkTestOmronMX2Only 0x08       ///< Modbus function 0x08 Тест связи с инвертром Omron MX2 функция только для него

#define MX2_STATE 0x0003 // (2 bytes) Status of the inverter

  // ModbusQuery 0: read registers
  ModbusQuery[0].u8id = SlaveModbusAdd;             // slave address
  ModbusQuery[0].u8fct = ku8MBReadHoldingRegisters; // function code (this one is registers read)
  ModbusQuery[0].u16RegAdd = 0x0;                   // start address in slave
  ModbusQuery[0].u16CoilsNo = 2;                    // number of elements (coils or registers) to read
  ModbusQuery[0].au16reg = ModbusSlaveRegisters;    // pointer to a memory array in the CONTROLLINO

  ModbusSlaveRegisters[0] = 0;
  ModbusSlaveRegisters[1] = 0;
  ModbusSlaveRegisters[2] = 0;
  ModbusSlaveRegisters[3] = 2;


  // ModbusSlaveRegisters[0] = 1234;

  // ModbusQuery 1: write a single register
  ModbusQuery[1].u8id = SlaveModbusAdd;              // slave address
  ModbusQuery[1].u8fct = ku8MBReadHoldingRegisters;  // function code (this one is write a single register)
  ModbusQuery[1].u16RegAdd = 0;                      // start address in slave
  ModbusQuery[1].u16CoilsNo = 4;                     // number of elements (coils or registers) to write
  ModbusQuery[1].au16reg = ModbusSlaveRegisters + 4; // pointer to a memory array in the CONTROLLINO

  // ModbusSlaveRegisters[4] = 1234; // initial value for the relays

  ControllinoModbusMaster.begin(19200);      // baud-rate at 19200
  ControllinoModbusMaster.setTimeOut(5000); // if there is no answer in 5000 ms, roll over

  WaitingTime = millis() + 1000;
  myState = 0;
  currentQuery = 0;

  //Controllino_RS485RxEnable();
  //Controllino_RS485TxEnable();

  delay(5000);
}

void loop()
{
  switch (myState)
  {
  case 0:
    if (millis() > WaitingTime)
      myState++; // wait state
    break;
  case 1:
    Serial.print("---- Sending query ");
    Serial.print(currentQuery);
    Serial.println(" -------------");
    ControllinoModbusMaster.query(ModbusQuery[currentQuery]); // send query (only once)
    myState++;
    currentQuery++;
    if (currentQuery == 2)
    {
      currentQuery = 0;
    }
    break;
  case 2:
    ControllinoModbusMaster.poll(); // check incoming messages
    if (ControllinoModbusMaster.getState() == COM_IDLE)
    {
      // response from the slave was received
      myState = 0;
      WaitingTime = millis() + 1000;
      // debug printout
      if (currentQuery == 0)
      {
        // registers write was proceed
        Serial.println("---------- WRITE RESPONSE RECEIVED ----");
        Serial.println("");
      }
      if (currentQuery == 1)
      {
        // registers read was proceed
        Serial.println("---------- READ RESPONSE RECEIVED ----");
        Serial.print("Slave ");
        
        Serial.print(SlaveModbusAdd, DEC);
        Serial.print(" ADC0: 0x");
        Serial.print(ModbusSlaveRegisters[0], HEX);
        Serial.print(" , Digital0: ");
        Serial.print(ModbusSlaveRegisters[1], HEX);
        Serial.print(" , ModbusCounterIn: ");
        Serial.print(ModbusSlaveRegisters[2], DEC);
        Serial.print(" , ModbusCounterOut: ");
        Serial.println(ModbusSlaveRegisters[3], DEC);
        Serial.println("-------------------------------------");
        Serial.println("");
      }
    }
    break;
  }
}