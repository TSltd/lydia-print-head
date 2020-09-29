#include <SPI.h>
#include <Controllino.h> /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

#define MAX_BUFFER 64 //!< maximum size for the communication buffer in bytes

int incomingByte = 0; // for incoming serial data
uint8_t au8Buffer[MAX_BUFFER];
uint8_t au8RBuffer[MAX_BUFFER];
uint8_t u8BufferSize = 6;

long u16RegAdd = 0;
long u16CoilsNo = 2001;

uint16_t calcCRC(uint8_t u8length)
{
  unsigned int temp, temp2, flag;
  temp = 0xFFFF;
  for (unsigned char i = 0; i < u8length; i++)
  {
    temp = temp ^ au8Buffer[i];
    Serial.println(au8Buffer[i], DEC);
    for (unsigned char j = 1; j <= 8; j++)
    {
      flag = temp & 0x0001;
      temp >>= 1;
      if (flag)
        temp ^= 0xA001;
    }
  }
  // Reverse byte order.
  temp2 = temp >> 8;
  temp = (temp << 8) | temp2;
  temp &= 0xFFFF;
  // the returned value is already swapped
  // crcLo byte is first & crcHi byte is last
  return temp;
}
void write()
{

  /**
   *  10 Coils : 01 03 00 00 00 0A C5 CD
   * 
   *  SP = 100 : 01 06 26 01 00 64 D2 A9
   *  SP = 20 :  01 06 26 01 00 14 D3 4D
   */

  Controllino_RS485TxEnable();
  Serial.println("sending...");
  uint8_t u8BufferSize = 6;

  au8Buffer[0] = 1;
  au8Buffer[1] = 3;
  au8Buffer[2] = 0;
  au8Buffer[3] = 0;
  au8Buffer[4] = 0;
  au8Buffer[5] = 0xA;
  
/*
  au8Buffer[0] = 1;
  au8Buffer[1] = 3;
  au8Buffer[2] = highByte(u16RegAdd);
  au8Buffer[3] = lowByte(u16RegAdd);
  au8Buffer[4] = highByte(u16CoilsNo);
  au8Buffer[5] = lowByte(u16CoilsNo);
  */

  // uint16_t u16crc = 50187;
  uint16_t u16crc = calcCRC(u8BufferSize);

  Serial.print("---- calc CRC : HEX :  ");

  Serial.print(u16crc, DEC);
  Serial.print(" - HEX : ");
  Serial.print(u16crc, HEX);
  Serial.print(" - ");
  Serial.print(u8BufferSize);
  Serial.print("\n");

  au8Buffer[u8BufferSize] = u16crc >> 8;
  u8BufferSize++;
  au8Buffer[u8BufferSize] = u16crc & 0x00ff;
  u8BufferSize++;

  Serial.print("Send Hex : ");
  for(int i = 0 ; i < u8BufferSize ; i++){
    Serial.print(au8Buffer[i], HEX);
    Serial.print(" : ");
  }
  Serial.println("");

  Serial3.write(au8Buffer, u8BufferSize);
  Serial3.flush(); // wait until the trasmission is complete
  Controllino_RS485RxEnable();
}

void setup()
{

  /* Initialize serial port for debug messages. */
  Serial.begin(19200);

  /* Initialize CONTROLLINO RS485 direction control DE/RE pins and Serial3 */
  Controllino_RS485Init(19200);
  write();
}
boolean bBuffOverflow = false;
void loop()
{

  // send data only when you receive data:
  uint8_t ru8BufferSize = 0;
  while (Serial3.available())
  {
    au8RBuffer[ru8BufferSize] = Serial3.read();
    Serial.print("got data : ");
    Serial.println(au8RBuffer[ru8BufferSize]);
    ru8BufferSize++;

    if (ru8BufferSize >= MAX_BUFFER)
    {
      bBuffOverflow = true;
      break;
    }
  }

  if (ru8BufferSize > 0)
  {
    // Serial.println("got data");
    // Serial.println(ru8BufferSize);
  }

  /*

  if (Serial3.available() > 0)
  {
    Serial.println("data");
    
    // read the incoming byte from RS485
    byte incomingByte = Serial3.read();
    // say what you got:
    Serial.print("I received (DEC): ");
    Serial.println(incomingByte, DEC);
    // and send it back to RS485
    // Controllino_RS485TxEnable();    
  }
  */

  delay(1000);
  write();
}