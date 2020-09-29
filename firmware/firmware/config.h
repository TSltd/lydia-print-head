#ifndef CONFIG_H
#define CONFIG_H

#include "enums.h"
#include "common/macros.h"
#include <Controllino.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Core settings
//

#define LOOP_DELAY              500         // Our frame time, exluding delays in some places
#define BOOT_DELAY              1000        // Wait at least this amount in ms after boot before doing anything

// Please consider to set this to false for production - especially with the full feature set since this is requiring extra
// time for the serial communication and will affect the overall framerate/performance
// #define DEBUG true

#define DEBUG_INTERVAL          1000
#define DEBUG_BAUD_RATE         19200       // Serial port speed

#define RELAY_ON 0                          // The relay bank's on value (eg: normally closed) 
#define RELAY_OFF 255                       // The relay bank's off value (eg: normally closed) 

#define DIR_SWITCH_UP_PIN       8           // The 3 position's up output
#define DIR_SWITCH_DOWN_PIN     9           // The 3 position's down output
// #define IGNORE_FIRST_DIRECTION              // Uncomment to ignore the 3pos switch (forward/reverse) after booting. This prevents surprises but possibly also accidents.
//#define DIR_SWITCH_DELAY        500      // If defined, add this as blocking delay between direction changes. Needed for some types of relays.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Machine settings
//

#define USE_CONTROLLINO
// Power, if defined, it enables the power addon which currently can control 2 contactors via relay

#define POWER_0 CONTROLLINO_R10 // Primary power circuit
#define POWER_1 CONTROLLINO_R11 // Secondary power circuit

// the relay type, please toggle if needed
#define POWER_NC true

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    VFD related
//

// #define HAS_MODBUS_BRIDGE

// VFD FWD Pin. This goes into the relay and then into the VFD.
#define FWD_PIN                             CONTROLLINO_R12

// VFD REV Pin. This goes into the relay and then into the VFD.
#define REV_PIN                             CONTROLLINO_R13

// Motor overload pin, if defined, this will be used to detect jamming
// On an OmronM2X, set the output via C21 (for output pin 11) to 03 for 
// 'Overload' (which is set in C41 )
#define MOTOR_LOAD_PIN                      CONTROLLINO_A2

// Some VFDs have an error signal. Consume this to set FATAL state - Not impl.  - uncomment to activate
// #define VFD_ERROR_PIN 13

// Some VFDs have a reset signal. Consume this in case we have a reset button - Not impl.  - uncomment to activate
// #define VFD_RESET_PIN 12

// Many VFDs accept speed regulation between 0 - 10V, uncomment - activate. This makes 
// only sense when a local quick speed dial or a remote operator is present 
// #define VFD_VSI_PIN         A4


// the time needed to boot up the VDF, only used when we control also the power via POWER_0
#define VFD_BOOT_TIME   500

// Some VFDs have outputs for 'run', 'power up'. If uncommented, we use this set the state 'ready' 
#define VFD_UP_PIN      46

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Motor related
//
// IR sensor ( LM393 ) used together with a rotary encoder disc - uncomment to activate
// #define HAS_MOTOR_IR_SENSOR
#define IR_PIN 2                            // Arduino has fixed pins for using interrupts, don't change this
#define IR_INTERVAL         500             // Recompute values in this interval
#define IR_TIMEOUT          1600            // Max. delta time between the sensor's on/off. This is used to recognize a jamming
// #define IR_SPEED                            // Calculate also the speed - needed for addons: injection & printer extension - uncomment to activate

// Motor temperature sensor - uncomment to activate. If the TEMPERTURE_MAX has been exceeded, it will stop the motor and goes into fatal mode.
// #define MOTOR_HAS_TEMPERTURE
#define MOTOR_TEMPERTURE_SCK_PIN  5
#define MOTOR_TEMPERTURE_CS_PIN   6
#define MOTOR_TEMPERTURE_SO_PIN   7

#define MOTOR_TEMPERTURE_INTERVAL 1000            // Sensor read interval, MAX6675 wants it over 1000
#define MOTOR_TEMPERTURE_MAX      50              // Max. motor temperature, in C

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Extrusion related
//

// Extrusion temperature sensor - uncomment to activate. If the EX_TEMPERTURE_MIN isn't reached yet, it won't allow the motor to run and goes into fatal mode.
// This will be only evaluated if the AUTO_REVERSE_MODE is set to AR_MODE::EXTRUSION;
// #define HAS_EXTRUDER_TEMPERATUR
#define EX_TEMPERTURE_SCK_PIN  5            
#define EX_TEMPERTURE_CS_PIN   6
#define EX_TEMPERTURE_SO_PIN   7
#define EX_TEMPERTURE_INTERVAL 1000         // Sensor read interval, MAX6675 wants it over 1000
#define EX_TEMPERTURE_MIN      170          // Min. extrusion temperature, in C
#define EX_TEMPERTURE_MIN_TIME MIN_MS * 5   // Min. time to have EX_TEMPERTURE_MIN present, second safety check

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Auto-Reverse
//
static int AUTO_REVERSE_MODE = AR_MODE::NONE;    // See enums.h :: AR_MODE. This value will be controlled by a 2 or 3 state switch.

// Threshold, if after this time the motor doesn't spin, abort!
// That can happen when the shredder or extrusion heavily jammed. Uncomment to activate this feature.
#define STARTING_MIN_TIME       1800        
// Threshold, time needed to have the motor on target speed but incl. some time to have shredded some material. 
// This may depend also on the VFDs primary acceleration time.
#define STARTING_TIMEOUT        3000        

// Threshold, if after that time the motor doesn't spin, abort ! 
// That can happen when the shredder or extrusion heavily jammed. Uncomment to activate this feature.
#define REVERSE_MIN_TIME        1800        
// Threshold, time needed to have the motor on target speed but also rotated for some time.
#define REVERSING_TIMEOUT       3200

// When shredding, stop all after this time. Uncomment to activate.
// This will be only used when AUTO_REVERSE_MODE is not AR_MODE::EXTRUSION.
// That means of course that shredding will always stop after one hour. 
// I considered this a safety feature and will be of importance when the operator is absent unexpectly.
// @TODO: in case there is a VFD speed sensor, or a hall sensor : augment mean deltas to determine 'activity' which
// will then extend the time. It may make sense to apply this feature also for extruding then.
#define MAX_SHRED_TIME          MIN_MS
#define MAX_IDLE_TIME           SECS * 20

#define MAX_REVERSE_TRIALS      5           // Max. trials to reverse before aborting (goes in FATAL state, unlock via 3pos switch = 0 or reset button)

#define AUTO_REVERSE_DELAY      1000        // When instructing direction changes, put this as delay in between. This is needed with certain relays which may require a delay.

// In some cases it makes sense to disable extrusion reverse entirely. Set this to false otherwise.
// This setting will be only used if AUTO_REVERSE_MODE is set to AR_MODE::EXTRUSION. 
// In later versions this will be refined with the MachineFeatures flags.
#define ALLOW_EXTRUSION_REVERSE     true 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    General switches

// Reset button - uncomment to activate. This will unlock the fatal state in case of jamming or other problems.
//#define HAS_RESET
#define RESET_PIN           3
#define RESET_DELAY         1000            // Time to wait when reset has been hit or hold. This is good to give the system some time
#define RESET_FATAL         true            // Makes the reset button act as emergency button. User has to go into stop positon first to unlock it again otherwise it keeps spinning after the RESET_DELAY
#define RESET_INTERVAL      300             // Interval to read the NC reset button
#define RESET_NC            true            // Toggles the reset button type between NC and NO

// Operation Mode Switch

#define OP_MODE_1_PIN       CONTROLLINO_A0
#define OP_MODE_ANALOG      
// #define OP_MODE_2_PIN       39
// #define OP_MODE_3_PIN       40

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    Feedback

// Status LEDS (ok, error) - uncomment to activate
// #define HAS_STATUS
// #define STATUS_OK_PIN       A3              // This goes into the relay
// #define STATUS_ERROR_PIN    A2              // This goes into the relay


// Alarm sound - uncomment to activate
// #define HAS_SOUND
// #define SOUND_PIN           4

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Plastic Hub Studio - internals : used by external controller setups
// Make sure it's matching 
#define FIRMATA_BAUD_RATE 19200
#define FIRMWARE_VERSION 0.1

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Plastic Hub related sensors and addons

////////////////////////////////////////////////////////////////
//
// HMI Bridge 
//
// uncomment to activate HMI uplink, this will process messages sent over serial
#define HAS_BRIDGE

// select serial (currently the only supported protocol)
#define HAS_SERIAL
// uncomment to send states, only needed for HMI 
#define BRIDGE_ONLINE_PIN                CONTROLLINO_D17

#ifdef HAS_BRIDGE
    // #define HAS_STATES
#endif

////////////////////////////////////////////////////////////////
//
// Instrumentation
//
// #define MEARSURE_PERFORMANCE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    externals
//

// pull in internal constants
#include "constants.h"

// pull in internal configs
#include "config_adv.h"

// The user_config.h is initially added to the github repository but changes will be ignored via .gitignore. Please keep this file safe and possibly
// on a per tenant base stored. You can override parameters in this file by using #undef SOME_PARAMETER and then re-define again if needed, otherwise disable 
// default features by using #undef FEATURE_OR_PARAMETER.
// This presents the possibilty to play with the code whilst staying in the loop with latest updates.
#include "user_config.h"

// At last we check all configs and spit compiler errors
#include "config_validator.h"

#endif
