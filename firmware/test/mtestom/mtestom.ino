#include <Controllino.h> /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include "OmronMx2.h"

#define POWER_0 CONTROLLINO_R10 // Primary power circuit
#define POWER_1 CONTROLLINO_R11 // Secondary power circuit

// devOmronMX2 dFC;
static devModbus Modbus;

void setup()
{
  Serial.begin(19200);
  digitalWrite(POWER_0, HIGH);
  digitalWrite(POWER_1, HIGH);

  delay(4000);
  
  Modbus.initModbus();
}

void loop()
{
  Modbus.LinkTestOmronMX2();
  delay(1000);
  // dFC.initFC();  
}