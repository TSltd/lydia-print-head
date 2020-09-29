#ifndef CONFIG_ADV_H
#define CONFIG_ADV_H

#ifdef HAS_DIP

#ifdef USE_MEGA
#define DIP_START 30
#else
#ifdef USE_UNO
#error "No Dip support for now"
#endif
#endif // HAS_DIP

#endif

// enable bridge debugging
//#define BRIDGE_DEBUG

////////////////////////////////////////////////////////////////////////////////
//  
//  Auto reverse time settings

// the time used to reverse after jamming
#define AR_REVERSE_TIME                                 2000

// after reversing came to stop, this is the delay 
// before it's forwarding
#define AR_FORWARD_WAIT_TIME                            1000

// after the delay AR_FORWARD_WAIT_TIME, we set the VFD to forward
// by this time, motor / VFD specifc
#define AR_FORWARDING_TIME                              1500


////////////////////////////////////////////////////////////////////////////////
//  
// Power settings

// optional current sensor to validate primary power is there
// #define POWER_CSENSOR_PRIMARY                           CONTROLLINO_A15

// optional current sensor to validate primary power is there
#define POWER_CSENSOR_SECONDARY                         CONTROLLINO_A14

////////////////////////////////////////////////////////////////////////////////
//  
// Shredding cycle timings
//  this times are mainly delays to switch between 
//  different cycle states:
//
//    enum SHRED_STATE
//    {
//        WAITING = 0,
//        INIT = 1,           --> cycle start
//        POWERED = 2,
//        STARTED = 3,
//        HOMED = 4,
//        PLUNGED = 5,
//        SHREDDED = 6,
//        UNPOWERED = 7,
//        DONE = 8,           --> cycle end
//        CANCELLING = 10,
//        JAMMED = 11,          --> auto reverse begin
//        REVERSING = 12,
//        REVERSED = 13,       
//        STOPPING = 14,
//        FORWARDING = 15,
//        CANCELLED = 16
//    }; 

// the time to wait before checking the power status
// if you experiencing weird behavior, please increase this 
// time, there can be spikes when powering and unpowering the VFD
#define SWT_INIT                                        1500

// after the VFD powered up, wait this time to proceed with shredding 
// or start the homing procedure if there is a plunger present
#define SWT_POWERED                                     2500

// the time interval to use for checking whether the plunger is homed
#define SWT_HOMING                                      2500

// the time interval to use for checking whether the plunger is homed
#define SWT_HOMING                                      2500


// after everything is ready, eg:
//  1. powered
//  2. homed (optinal, plunger present)
//  3. vfd is set to forward
// this is the time to wait before we activate the plunger
#define SWT_STARTED                                     2000

// the interval used to check the 'PLUNGED' state (fixed or limit switches) 
#define SWT_PLUNGED                                     2500

// as soon it's plunged - if plunger present - this is the time to wait 
// before it's stops the VFD. this time depends on the plunger distance to
// the shredder blades. in some cases it's good to leave it spinning by exactly
// this time 
#define SWT_UNPOWERED                                   2000

// the interval to use to checking the HOMED state of the plunger after 
// it stopped the shredder; when it reaches the 'homed' state, the shredding cycle
// is complete
#define SWT_SHREDDED 3000

////////////////////////////////////////////////////////////////////////////////
//
//  Plunger settings and timings
//
//  Times are base on observations rather than calculations. Calculating it
//  will affect plunger speed drasitcally even more lower spec boards.

// interval before reading the limit switch
// this setting will affect your plunging speed since analogRead will 
// consume some time and we do have only one thread.
#define LIMIT_SWITCH_INTERVAL                           400

// base speed and use as factor to scale settings to your hardware
#define PLUNGER_BASE_SPEED                              1000

// manual move speed
#define PLUNGER_MOVE_SPEED                              PLUNGER_BASE_SPEED * 450

// homing speed
#define PLUNGER_HOMING_SPEED                            PLUNGER_BASE_SPEED * 200

// retract speed
#define PLUNGER_RETRACT_SPEED                           PLUNGER_BASE_SPEED * 200

// plunge speed
#define PLUNGER_PLUNGE_SPEED                            PLUNGER_BASE_SPEED * 1600

// plunge/homing travel
#define PLUNGE_PLUNG_DISTANCE                           145000

// inverted travel
#define PLUNGE_HOME_DISTANCE                            -PLUNGE_PLUNG_DISTANCE

// retract travel
#define PLUNGER_RETRACE_DISTANCE                        1500

// in case limit switch broke
#define PLUNGE_MAX_MOVE_TIME                            SECS * 12


///     Optional : current monitor for the plunger stepper motor

// the current when the drives have no power
#define PLUNGER_STEPPER_OFFLINE_1                       163
// the current when the drives have power
#define PLUNGER_STEPPER_ONLINE_1                        166

/////////////////////////////////////////////////////////////
//
// Motor load settings, this requires a current sensor or can be
// taken from the VFD's output. 

// the interval to read the current
#define MOTOR_LOAD_READ_INTERVAL                        100

// the current measured when the motor runs idle, min - max range
#define MOTOR_IDLE_LOAD_RANGE_MIN                       30
#define MOTOR_IDLE_LOAD_RANGE_MAX                       50

// the current measured when the motor is under load, min - max range
#define MOTOR_SHREDDING_LOAD_RANGE_MIN                  50
#define MOTOR_SHREDDING_LOAD_RANGE_MAX                  99

// the current measured when the motor is overloaded, min - max range
#define MOTOR_OVERLOAD_RANGE_MIN                        100
#define MOTOR_OVERLOAD_RANGE_MAX                        400

#define MOTOR_MIN_DT                                    2500

/////////////////////////////////////////////////////////////
//
//  Bridge related
#define STATE_RESPONSE_CODE                             1000
// #define BRIDGE_HAS_RESPONSE
/////////////////////////////////////////////////////////////
//
//  Error codes
//
#define E_MSG_OK            "Ok"
#define E_MSG_STUCK         "Shredder is stuck"

// common operating failures
#define E_OK                0                 //all good
#define E_STUCK             100               //Shredder stuck

// power failures

#define E_POWER_PRIM_ON     145               // Power is on whilst it shouldn't be
#define E_POWER_PRIM_OFF    146               // Power is off whilst it should be

#define E_POWER_SEC_ON      147               // Power is on whilst it shouldn't be
#define E_POWER_SEC_OFF     148               // Power is off whilst it should be
#define E_POWER             150               // Nothing is online

#define E_VFD_OFFLINE       E_POWER_PRIM_OFF  // VFD should be online
#define E_PLUNGER_OFFLINE   E_POWER_SEC_OFF   // Plunger should be online


// sensor failures
#define E_VFD_CURRENT       200               // VFD current abnormal: below or above average
#define E_PLUNGER_CURRENT   210               // Plunger current abnormal: below or above average
#define E_OPERATING_SWITCH  220               // Operating switch invalid value

#define E_PLUNGER_LIMIT     230               // Something is wrong with the limit switches, should
#define E_PLUNGER_OVERLOAD  235               // Stepper above average

#define E_CARTRIDGE_OPEN    240               // Cartridge open sensor doesn't work 
#define E_SERVICE_OPEN      241               // Sensor open sensor doesn't work 

////////////////////////////
//
// sub system failures
//
#define E_USER_START        1000                // base offset for sub system errors

// vfd
#define E_VFD_RUN           300                 // Motor should spin but doesnt
#define E_VFD_LOSS          301                 // Motor should not spin but does
#define E_VFD_CUSTOM(A)     E_USER_START+A      // Custom VFD error

// plunger
#define E_PLUNGER_RUN 400   310                 // Plungers runs with weird limit/homing switch activity

// motor
#define E_MOTOR_DT_IDLE     320                 // Motor runs idle longer as defined
#define E_MOTOR_DT_OVERLOAD 321                 // Motor runs overloaded longer as defined

#define E_BRIDGE_LOSS       400                 // bridge poll timeout

// bridge
#define E_BRIDGE_START      2000                // base offset for custom bridge errors
#define E_BRIDGE_CUSTOM(A)  E_USER_START+A      // Custom bridge error
#define E_BRIDGE_PARITY     E_BRIDGE_CUSTOM(1)  // @todo, parity check failure 
#define E_BRIDGE_CRC        E_BRIDGE_CUSTOM(2)  // @todo, crc  failure
#define E_BRIDGE_FLOOD      E_BRIDGE_CUSTOM(3)  // @todo, msg queue

// extrusion
#define E_EX_BASE           3000                // base offset extruder
#define E_EX_CUSTOM(A)      E_EX_BASE+A         // Custom bridge error

#ifdef CONTROLLINO_MEGA_H

#define CONTROLLINO_ETHERNET_CHIP_SELECT 70
#define CONTROLLINO_ETHERNET_INTERRUPT 71
#define CONTROLLINO_RTC_CHIP_SELECT 72
#define CONTROLLINO_RTC_INTERRUPT 73
#define CONTROLLINO_OVERLOAD 74
#define CONTROLLINO_RS485_DE 75
#define CONTROLLINO_RS485_nRE 76

#define CONTROLLINO_PIN_HEADER_PWM_00 2
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_00 2
#define CONTROLLINO_SCREW_TERMINAL_PWM_00 2
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_OUT_00 2

#define CONTROLLINO_PIN_HEADER_PWM_01 3
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_01 3
#define CONTROLLINO_SCREW_TERMINAL_PWM_01 3
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_OUT_01 3

#define CONTROLLINO_PIN_HEADER_PWM_02 4
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_02 4
#define CONTROLLINO_SCREW_TERMINAL_PWM_02 4
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_OUT_02 4

#define CONTROLLINO_PIN_HEADER_PWM_03 5
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_03 5
#define CONTROLLINO_SCREW_TERMINAL_PWM_03 5
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_OUT_03 5

#define CONTROLLINO_PIN_HEADER_PWM_04 6
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_04 6
#define CONTROLLINO_SCREW_TERMINAL_PWM_04 6
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_OUT_04 6

#define CONTROLLINO_PIN_HEADER_PWM_05 7
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_05 7
#define CONTROLLINO_SCREW_TERMINAL_PWM_05 7
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_OUT_05 7

#define CONTROLLINO_PIN_HEADER_PWM_06 8
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_06 8
#define CONTROLLINO_SCREW_TERMINAL_PWM_06 8
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_OUT_06 8

#define CONTROLLINO_PIN_HEADER_PWM_07 9
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_07 9
#define CONTROLLINO_SCREW_TERMINAL_PWM_07 9
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_OUT_07 9

#define CONTROLLINO_PIN_HEADER_PWM_08 10
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_08 10
#define CONTROLLINO_SCREW_TERMINAL_PWM_08 10
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_OUT_08 10

#define CONTROLLINO_PIN_HEADER_PWM_09 11
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_09 11
#define CONTROLLINO_SCREW_TERMINAL_PWM_09 11
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_OUT_09 11

#define CONTROLLINO_PIN_HEADER_PWM_10 12
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_10 12
#define CONTROLLINO_SCREW_TERMINAL_PWM_10 12
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_OUT_10 12

#define CONTROLLINO_PIN_HEADER_PWM_11 13
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_11 13
#define CONTROLLINO_SCREW_TERMINAL_PWM_11 13
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_OUT_11 13

#define CONTROLLINO_RS485_TX 14

#define CONTROLLINO_RS485_RX 15

#define CONTROLLINO_UART_TX 16

#define CONTROLLINO_UART_RX 17

#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_16 18
#define CONTROLLINO_PIN_HEADER_INT_00 18
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_IN_16 18
#define CONTROLLINO_SCREW_TERMINAL_INT_00 18

#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_17 19
#define CONTROLLINO_PIN_HEADER_INT_01 19
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_IN_17 19
#define CONTROLLINO_SCREW_TERMINAL_INT_01 19

#define CONTROLLINO_PIN_HEADER_SDA 20
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_25 20

#define CONTROLLINO_PIN_HEADER_SCL 21
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_24 21

#define CONTROLLINO_RELAY_00 22

#define CONTROLLINO_RELAY_01 23

#define CONTROLLINO_RELAY_02 24

#define CONTROLLINO_RELAY_03 25

#define CONTROLLINO_RELAY_04 26

#define CONTROLLINO_RELAY_05 27

#define CONTROLLINO_RELAY_06 28

#define CONTROLLINO_RELAY_07 29

#define CONTROLLINO_RELAY_08 30

#define CONTROLLINO_RELAY_09 31

#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_12 42

#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_13 43

#define CONTROLLINO_PIN_HEADER_PWM_12 44
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_14 44

#define CONTROLLINO_PIN_HEADER_PWM_13 45
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_15 45

#define CONTROLLINO_PIN_HEADER_MISO 50
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_29 50

#define CONTROLLINO_PIN_HEADER_MOSI 51
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_28 51

#define CONTROLLINO_PIN_HEADER_SCK 52
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_27 52

#define CONTROLLINO_PIN_HEADER_SS 53
#define CONTROLLINO_PIN_HEADER_DIGITAL_OUT_26 53

#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_00 54
#define CONTROLLINO_PIN_HEADER_DIGITAL_ADC_IN_00 54
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_ADC_IN_00 54
#define CONTROLLINO_SCREW_TERMINAL_ANALOG_ADC_IN_00 54
#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_00 54
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_IN_00 54

#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_01 55
#define CONTROLLINO_PIN_HEADER_DIGITAL_ADC_IN_01 55
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_ADC_IN_01 55
#define CONTROLLINO_SCREW_TERMINAL_ANALOG_ADC_IN_01 55
#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_01 55
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_IN_01 55

#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_02 56
#define CONTROLLINO_PIN_HEADER_DIGITAL_ADC_IN_02 56
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_ADC_IN_02 56
#define CONTROLLINO_SCREW_TERMINAL_ANALOG_ADC_IN_02 56
#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_02 56
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_IN_02 56

#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_03 57
#define CONTROLLINO_PIN_HEADER_DIGITAL_ADC_IN_03 57
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_ADC_IN_03 57
#define CONTROLLINO_SCREW_TERMINAL_ANALOG_ADC_IN_03 57
#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_03 57
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_IN_03 57

#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_04 58
#define CONTROLLINO_PIN_HEADER_DIGITAL_ADC_IN_04 58
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_ADC_IN_04 58
#define CONTROLLINO_SCREW_TERMINAL_ANALOG_ADC_IN_04 58
#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_04 58
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_IN_04 58

#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_05 59
#define CONTROLLINO_PIN_HEADER_DIGITAL_ADC_IN_05 59
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_ADC_IN_05 59
#define CONTROLLINO_SCREW_TERMINAL_ANALOG_ADC_IN_05 59
#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_05 59
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_IN_05 59

#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_06 60
#define CONTROLLINO_PIN_HEADER_DIGITAL_ADC_IN_06 60
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_ADC_IN_06 60
#define CONTROLLINO_SCREW_TERMINAL_ANALOG_ADC_IN_06 60
#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_06 60
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_IN_06 60

#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_07 61
#define CONTROLLINO_PIN_HEADER_DIGITAL_ADC_IN_07 61
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_ADC_IN_07 61
#define CONTROLLINO_SCREW_TERMINAL_ANALOG_ADC_IN_07 61
#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_07 61
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_IN_07 61

#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_08 62
#define CONTROLLINO_PIN_HEADER_DIGITAL_ADC_IN_08 62
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_ADC_IN_08 62
#define CONTROLLINO_SCREW_TERMINAL_ANALOG_ADC_IN_08 62
#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_08 62
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_IN_08 62

#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_09 63
#define CONTROLLINO_PIN_HEADER_DIGITAL_ADC_IN_09 63
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_ADC_IN_09 63
#define CONTROLLINO_SCREW_TERMINAL_ANALOG_ADC_IN_09 63
#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_09 63
#define CONTROLLINO_SCREW_TERMINAL_DIGITAL_IN_09 63

#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_10 64
#define CONTROLLINO_PIN_HEADER_DIGITAL_ADC_IN_10 64
#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_10 64

#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_11 65
#define CONTROLLINO_PIN_HEADER_DIGITAL_ADC_IN_11 65
#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_11 65

#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_12 66
#define CONTROLLINO_PIN_HEADER_DIGITAL_ADC_IN_12 66
#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_12 66

#define CONTROLLINO_PIN_HEADER_ANALOG_ADC_IN_13 67
#define CONTROLLINO_PIN_HEADER_DIGITAL_ADC_IN_13 67
#define CONTROLLINO_PIN_HEADER_DIGITAL_IN_13 67

#define CONTROLLINO_D0 2
#define CONTROLLINO_D1 3
#define CONTROLLINO_D2 4
#define CONTROLLINO_D3 5
#define CONTROLLINO_D4 6
#define CONTROLLINO_D5 7
#define CONTROLLINO_D6 8
#define CONTROLLINO_D7 9
#define CONTROLLINO_D8 10
#define CONTROLLINO_D9 11
#define CONTROLLINO_D10 12
#define CONTROLLINO_D11 13
#define CONTROLLINO_D12 42
#define CONTROLLINO_D13 43
#define CONTROLLINO_D14 44
#define CONTROLLINO_D15 45
#define CONTROLLINO_D16 46
#define CONTROLLINO_D17 47
#define CONTROLLINO_D18 48
#define CONTROLLINO_D19 49
#define CONTROLLINO_D20 77
#define CONTROLLINO_D21 78
#define CONTROLLINO_D22 79
#define CONTROLLINO_D23 80

#define CONTROLLINO_A0 54
#define CONTROLLINO_A1 55
#define CONTROLLINO_A2 56
#define CONTROLLINO_A3 57
#define CONTROLLINO_A4 58
#define CONTROLLINO_A5 59
#define CONTROLLINO_A6 60
#define CONTROLLINO_A7 61
#define CONTROLLINO_A8 62
#define CONTROLLINO_A9 63
#define CONTROLLINO_A10 64
#define CONTROLLINO_A11 65
#define CONTROLLINO_A12 66
#define CONTROLLINO_A13 67
#define CONTROLLINO_A14 68
#define CONTROLLINO_A15 69
#define CONTROLLINO_I16 38
#define CONTROLLINO_I17 39
#define CONTROLLINO_I18 40
#define CONTROLLINO_IN0 18
#define CONTROLLINO_IN1 19

#define CONTROLLINO_MINUS 14
#define CONTROLLINO_PLUS 15

#define CONTROLLINO_R0 22
#define CONTROLLINO_R1 23
#define CONTROLLINO_R2 24
#define CONTROLLINO_R3 25
#define CONTROLLINO_R4 26
#define CONTROLLINO_R5 27
#define CONTROLLINO_R6 28
#define CONTROLLINO_R7 29
#define CONTROLLINO_R8 30
#define CONTROLLINO_R9 31
#define CONTROLLINO_R10 32
#define CONTROLLINO_R11 33
#define CONTROLLINO_R12 34
#define CONTROLLINO_R13 35
#define CONTROLLINO_R14 36
#define CONTROLLINO_R15 37

#endif

#endif