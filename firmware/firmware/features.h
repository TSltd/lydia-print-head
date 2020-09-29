#ifndef FEATURES_H
#define FEATURES_H

#ifdef HAS_BRIDGE
  #include "bridge.h"
#endif

#ifdef HAS_SERIAL
  #include "serial.h"
#endif

#ifdef HAS_TEMPERTURE
#include "temperature.h"
#endif

#ifdef HAS_EXTRUDER_TEMPERATUR
#include "ex_temperature.h"
#endif

#ifdef HAS_SOUND
#include "alarm.h"
#endif

#ifdef HAS_HALL
#include "hall.h"
#endif

#ifdef HAS_STATUS
  #include "addons/Status.h"
#endif

#ifdef HAS_RESET
#include "reset.h"
#endif


#ifdef HAS_AUTOREVERSE
#include "auto-reverse.h"
#ifdef HAS_IR
#include "IRSensor.h"
#endif
#endif


#if defined(ENCLOSURE_SENSOR_PIN_1) || defined(ENCLOSURE_SENSOR_PIN_2)
#include "EnclosureSensor.h"
#define HAS_ENCLOSURE_SENSOR
#endif

#if defined(POWER_0) || defined(POWER_1)
  #include "addons/Power.h"
  #define HAS_POWER
#endif

#ifdef MOTOR_LOAD_PIN
  #include "addons/MotorLoad.h"
#endif

#ifdef MOTOR_HAS_TEMPERTURE
#include "addons/MotorTemperature.h"
#endif

#ifdef HAS_DIP
#include "addons/_DipSwitch.h"
#endif

#ifdef HAS_MOTOR_IR_SENSOR
#include "addons/MotorSpeed.h"
#endif


#if defined(OP_MODE_1_PIN)
  #include "addons/OperationModeSwitch.h"
  #define HAS_OP_MODE_SWITCH
#endif

#if defined(FWD_PIN) && defined(REV_PIN)
  #include "addons/DirectionSwitch.h"
  #define HAS_DIRECTION_SWITCH
#endif


#if defined(FWD_PIN) && defined(REV_PIN)
  #include "VFD.h"
  #define HAS_VFD
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Plastic Hub - Addons
//

// Plastic Hub Studio  - remote control
#ifdef USE_FIRMATA
#include "firmata_link.h"
#endif

#ifdef HAS_PLUNGER
  #include "Plunger.h"
#endif

#endif