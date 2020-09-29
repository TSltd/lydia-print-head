/**
 *  Modbus master example 1:
 *  The purpose of this example is to query an array of data
 *  from an external Modbus slave device. 
 *  The link media can be USB or RS232.
 *
 *  Recommended Modbus slave: 
 *  diagslave http://www.modbusdriver.com/diagslave.html
 *
 *  In a Linux box, run 
 *  "./diagslave /dev/ttyUSB0 -b 19200 -d 8 -s 1 -p none -m rtu -a 1"
 * 	This is:
 * 		serial port /dev/ttyUSB0 at 19200 baud 8N1
 *		RTU mode and address @1
 */

#include "ModbusRtu.h"
#include <Controllino.h> /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

// data array for modbus network sharing
uint16_t au16data[16];
uint8_t u8state;


#define ku8MBReadCoils 0x01          ///< Modbus function 0x01 Read Coils
#define ku8MBReadDiscreteInputs 0x02 ///< Modbus function 0x02 Read Discrete Inputs
#define ku8MBWriteSingleCoil 0x05    ///< Modbus function 0x05 Write Single Coil
#define ku8MBWriteMultipleCoils 0x0F ///< Modbus function 0x0F Write Multiple Coils

#define ku8MBReadHoldingRegisters 0x03       ///< Modbus function 0x03 Read Holding Registers
#define ku8MBReadInputRegisters 0x04         ///< Modbus function 0x04 Read Input Registers
#define ku8MBWriteSingleRegister 0x06        ///< Modbus function 0x06 Write Single Register
#define ku8MBWriteMultipleRegisters 0x10     ///< Modbus function 0x10 Write Multiple Registers
#define ku8MBMaskWriteRegister 0x16          ///< Modbus function 0x16 Mask Write Register
#define ku8MBReadWriteMultipleRegisters 0x17 ///< Modbus function 0x17 Read Write Multiple Registers
#define ku8MBLinkTestOmronMX2Only 0x08       ///< Modbus function 0x08 Тест связи с инвертром Omron MX2 функция только для него


#define R_INFO_START        0
#define R_INFO_LENGTH       0xA


#define W_SP_VALUE          300
#define W_SP_START          0x2601


Modbus master(0, 3);
modbus_t telegram;
unsigned long u32wait;

long u16RegAdd = 0;
long u16CoilsNo = 0xA;


/// Status bytes - STOP : 768 | 2
/// Status bytes - RUN(&OUT) : 512 | 256
/// Status bytes - RUN(none) : 512 | 256
/// Status bytes - RUN(none) : 512 | 28674
//
//  Status bits : RUN/STOP = 24
// 
//

long COMMAND = ku8MBReadHoldingRegisters;

unsigned long setByte(unsigned long value, byte position, byte new_byte)
{
  unsigned bitpos = position * 8;
  unsigned long mask = 0xFFU << bitpos;
  value &= ~mask;
  value |= new_byte;
  return value;
}

void setup()
{
  Serial.begin(19200); // baud-rate at 19200

  // https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/
  master.begin(19200, SERIAL_8E1); // SERIAL_8E1

  master.setTimeOut(2000); // if there is no answer in 2000 ms, roll over
  u32wait = millis() + 1000;
  u8state = 0;

/*
  int b2 = highByte(u16RegAdd); //  ADD_HI
  int b3 = lowByte(u16RegAdd);  //  ADD_LO

  Serial.println(" ---- ");

  Serial.println(u16RegAdd);
  Serial.println(" ---- ");

  Serial.print("ADD_HI :\t - POS[2]  = ");
  Serial.print(b2);
  Serial.print("\t - Hex:  ");
  Serial.println(b2, HEX);

  Serial.print("ADD_LOW :\t - POS[3] = ");
  Serial.print(b3);
  Serial.print("\t - Hex:  ");
  Serial.println(b3, HEX);

  int b4 = highByte(u16CoilsNo); //  ADD_HI
  int b5 = lowByte(u16CoilsNo);  //  ADD_LO

  Serial.println(" ---- ");

  Serial.print("NB_HI :\t - POS[4]  = ");
  Serial.print(b4);
  Serial.print("\t - Hex:  ");
  Serial.println(b4, HEX);

  Serial.print("NB_LO :\t - POS[5] = ");
  Serial.print(b5);
  Serial.print("\t - Hex:  ");
  Serial.println(b5, HEX);
  */
}
#define MX2_STATE 0x0003 // (2 bytes) Status of the inverter
void loop()
{

  switch (u8state)
  {
  case 0:
    if (millis() > u32wait)
      u8state++; // wait state
    break;
  case 1:
  {
    telegram.u8id = 1;                          // slave address
    telegram.u8fct = COMMAND; // function code (this one is registers read)
    telegram.u16RegAdd = u16RegAdd;             // start address in slave
    telegram.u16CoilsNo = u16CoilsNo;           // number of elements (coils or registers) to read
    telegram.au16reg = au16data;                // pointer to a memory array in the Arduino

    master.query(telegram); // send query (only once)
    u8state++;
    break;
  }
  case 2:
    master.poll(); // check incoming messages
    if (master.getState() == COM_IDLE)
    {
      u8state = 0;
      u32wait = millis() + 1000;
      Serial.print(master.getLastError());
      Serial.print("\t ADC-0: ");
      Serial.print(au16data[0], HEX);
      Serial.print("\t ADC-1 : ");
      Serial.print(au16data[1], DEC);
      Serial.print("\t ADC-2 : ");
      Serial.print(au16data[2], DEC);
      Serial.print("\t ADC-3 : ");
      Serial.print(au16data[3], DEC);
      Serial.print("\t ADC-4 : ");
      Serial.print(au16data[4], DEC);
      Serial.print("\t ADC-5 : ");
      Serial.print(au16data[5], DEC);

      Serial.print("\t ADC-6 : ");
      Serial.print(au16data[6], DEC);

      Serial.print("\t ADC-7 : ");
      Serial.print(au16data[7], DEC);

      Serial.print("\t ADC-8 : ");
      Serial.print(au16data[8], DEC);


      Serial.println("-------------------------------------");
      Serial.println("");
    }
    break;
  }
}