/*

  Basic.pde - example using ModbusMaster library

  Library:: ModbusMaster
  Author:: Doc Walker <4-20ma@wvfans.net>

  Copyright:: 2009-2016 Doc Walker

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

*/

#include <ModbusMaster.h>
#include <Controllino.h>

/*
#define CONTROLLINO_RS485_TX 14  
#define CONTROLLINO_RS485_RX 15  
*/
// instantiate ModbusMaster object
#define POWER_0 CONTROLLINO_R10 // Primary power circuit
#define POWER_1 CONTROLLINO_R11 // Secondary power circuit

ModbusMaster node;

void setup()
{
  // use Serial (port 0); initialize Modbus communication baud rate
  Serial.begin(19200);

  
  digitalWrite(POWER_0, HIGH);
  digitalWrite(POWER_1, HIGH);
  
  
  
  delay(5000);
  
  Serial.println("test");
  Controllino_RS485Init();
  Controllino_RS485RxEnable();
  Controllino_RS485TxEnable();

  
  // communicate with Modbus slave ID 2 over Serial (port 0)
  node.begin(1, Serial3);
}

void loop()
{
  static uint32_t i;
  uint8_t j, result;
  uint16_t data[6];

  i++;
  
  Serial.println("0");
  // set word 0 of TX buffer to least-significant word of counter (bits 15..0)
   node.setTransmitBuffer(0, lowWord(i));

  // set word 1 of TX buffer to most-significant word of counter (bits 31..16)
  node.setTransmitBuffer(1, highWord(i));

  // slave: write TX buffer to (2) 16-bit registers starting at register 0
  result = node.writeMultipleRegisters(0, 2);

  // slave: read (6) 16-bit registers starting at register 2 to RX buffer
  result = node.readHoldingRegisters(2, 6);
  Serial.println("1");
  
  // do something with data if read is successful
  if (result == node.ku8MBSuccess)
  {
    Serial.println("ok!!");
    for (j = 0; j < 6; j++)
    {
      data[j] = node.getResponseBuffer(j);
    }
  }
  else
  {
    Serial.println("not ok");
    Serial.println(result);
  }

  Serial.println("ok ::: ");
  Serial.println(result);
}
