#include <SPI.h>
#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#define POWER_0 CONTROLLINO_R10 // Primary power circuit
#define POWER_1 CONTROLLINO_R11 // Secondary power circuit

/*
  CONTROLLINO - Demonstration of RS485 interface usage, Version 02.00

  Compatible with CONTROLLINO MAXI and MEGA.

  IMPORTANT INFORMATION!
  Please, select proper target board in Tools->Board->Controllino MAXI/MEGA before Upload to your CONTROLLINO.
  
  (Refer to https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library if you do not see the CONTROLLINOs in the Arduino IDE menu Tools->Board.)

  This example sketch requires external 12V or 24V power supply for your CONTROLLINO and RS485 communication partner device (e.g. RS485-USB converter and PC). 

  Created 12 Jan 2017, Updated 20.09.2018
  by Lukas

  https://controllino.biz/
  
  (Check https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library for the latest CONTROLLINO related software stuff.)
*/

int incomingByte = 0;   // for incoming serial data

void setup() {
  
  
  digitalWrite(POWER_0, HIGH);
  digitalWrite(POWER_1, HIGH);
  delay(2000);

  /* Initialize serial port for debug messages. */
  Serial.begin(19200);
  
  /* Initialize CONTROLLINO RS485 direction control DE/RE pins and Serial3 */
  Controllino_RS485Init(19200);
  Controllino_RS485RxEnable();
  Controllino_RS485TxEnable();

  Serial.println("Recieving RS485...");
}

void loop() {
  
  // send data only when you receive data:
  if (Serial3.available() > 0)
  {
    // read the incoming byte from RS485
    incomingByte = Serial3.readString();
    // say what you got:
    Serial.print("I received (DEC): ");
    Serial.println(incomingByte, DEC);

    // and send it back to RS485
    
    Serial3.write(incomingByte);
    Serial3.flush(); // wait until the trasmission is complete
  }
  
}

/* 2018-09-20: The sketch was successfully tested with Arduino 1.8.5, Controllino Library 3.0.2 and CONTROLLINO MAXI and MEGA. */
