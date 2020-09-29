#ifndef ENUMS_H
#define ENUMS_H

enum BOARD
{
  MEGA
};

enum POS3_DIRECTION
{
  UP = 1,
  MIDDLE = 0,
  DOWN = 2,
  INVALID = -1
};

enum MODE
{
};

enum AR_MODE
{
  NORMAL = 1,    // Shredding
  EXTRUSION = 2, // Extrusion (no reverse)
  NONE = 3,      // Disable any jamming detection
  REMOTE = 4     // User land, ie: Firmata, I2C or PlasticHub-Studio
};

enum ADDON_FLAGS
{
  DEBUG = 1,
  INFO = 2,
  LOOP = 3,
  DISABLED = 4,
  SETUP = 5,
  MAIN = 6,
  STATE = 7
};

enum ADDONS
{
  MOTOR_IDLE = 1,
  MOTOR_LOAD = 2,
  MOTOR_TEMPERATURE = 3,
  VFD_CONTROL = 4,
  DIRECTION_SWITCH = 5,
  ENCLOSURE_SENSOR = 6,
  MOTOR_SPEED = 7,
  POWER = 11,
  OPERATION_MODE_SWITCH = 20,
  SERIAL_BRIDGE = 23,
  APP = 25,
  MODBUS_BRIDGE = 26,
  RMOTOR_CONTROL = 30,
  OMRON_PID = 31,
  LAST = 64
};

enum POWER_CIRCUIT
{
  POWER_PRIMARY = 0,
  POWER_SECONDARY = 1
};

enum OPERATION_MODE
{
  OP_NONE = 0,
  OP_NORMAL = 1,
  OP_DEBUG = 2,
  OP_SERVICE = 3,
  OP_TEST
};
enum ERROR
{
  ERROR_OK = 0,
  ERROR_WARNING = 1,
  ERROR_FATAL = 2
};

#endif
