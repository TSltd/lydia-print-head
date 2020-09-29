// ------------------------------------------------ --------------------------------
// Description: Omron MX2 inverter operation class
// Public class methods are unified for all inverters
// ------------------------------------------------ --------------------------------
#ifndef OmronFC_h
#define OmronFC_h
#include <Arduino.h>
#include "ModbusMaster.h"

// #include "Constant.h " // All configuration and project constants Must be first !!!!
#define GETBIT(b, f) ((b & (1 << (f))) ? true : false)
#define SETBIT1(b, f) (b |= (1 << (f)))
#define SETBIT0(b, f) (b &= ~(1 << (f)))

#define ERR_LINK_FC 0xFF  // Inertor state - no communication.
#define fFC 0             // flag the presence of an inverter
#define fFC_RetOil 1      // Oil Return
#define fPower 2          // flag power limitation mode (reserve - limitation is always now)
#define fOnOff 3          // enable / disable flag
#define fErrFC 4          // flag global inverter error - inverter operation is prohibited
#define fAutoResetFault 5 // flag Auto- reset of non-critical inverter failure
#define fLogWork 6        // flag log parameters during operation
#define fFC_RetOilSt 7    // Work oil return
#define FC_SAVED_FLAGS ((1 << fAutoResetFault) | (1 << fLogWork) | (1 << fFC_RetOil))


#ifndef FC_VACON


#define WARNING_VALUE 1          // Attempt to set a value outside the range of the query type SET \
                                 // Icon for heating hot water
#define OK 0                     // No errors
#define ERR_MINTEMP -1           // Exit the lower limit of the temperature sensor
#define ERR_MAXTEMP -2           // Exit the upper limit of the temperature sensor
#define ERR_MINPRESS -3          // Exit the lower limit of the pressure sensor
#define ERR_MAXPRESS -4          // Go beyond the upper limit of the pressure sensor
#define ERR_SENSOR -5            // Sensor is not allowed in the current configuration
#define ERR_ADDRESS -6           // Temperature sensor address not set
#define ERR_DINPUT -7            // Contact sensor trips - alarm
#define ERR_MAX_EEV -8           // Going out of range (in steps) at the top
#define ERR_MIN_EEV -9           // Out of range (in steps) at the bottom
#define ERR_DEVICE -10           // The device is prohibited in the current configuration
#define ERR_ONEWIRE -11          // Reset error on the OneWire bus (open or short)
#define ERR_OVERHEAT -12         // ERV received a negative overheat
#define ERR_MEM_FREERTOS -13     // Free RTOS cannot create a task - not enough memory
#define ERR_PEVA_EEV -14         // There is no pressure sensor, and the ERV algorithm that uses it is selected
#define ERR_SAVE_EEPROM -15      // Error writing settings to eeprom I2C
#define ERR_LOAD_EEPROM -16      // Error reading settings from eeprom I2C
#define ERR_CRC16_EEPROM -17     // Checksum error for settings
#define ERR_BAD_LEN_EEPROM -18   // Data size mismatch while reading settings
#define ERR_HEADER_EEPROM -19    // Settings data not found in eeprom I2C
#define ERR_SAVE1_EEPROM -20     // Error writing status to eeprom I2C
#define ERR_LOAD1_EEPROM -21     // Error reading state from eeprom I2C
#define ERR_HEADER1_EEPROM -22   // Status data not found in eeprom I2C
#define ERR_SAVE2_EEPROM -23     // Error writing counters to eeprom I2C
#define ERR_LOAD2_EEPROM -24     // Error reading counters from eeprom I2C
#define ERR_WRONG_HARD_STATE -25 // Invalid VT state
#define ERR_DTEMP_CON -26        // The temperature difference on the condenser has been increased
#define ERR_DTEMP_EVA -27        // Excessive temperature difference on the evaporator
#define ERR_PUMP_CON -28         // There is no pump on the condenser, check the configuration
#define ERR_PUMP_EVA -29         // There is no pump on the evaporator, check the configuration
#define ERR_READ_PRESS -30       // Error reading pressure sensor (data not ready)
#define ERR_NO_COMPRESS -31      // There is no compressor, check the configuration
#define ERR_NO_WORK -32          // Everything is off and VT is on
#define ERR_COMP_ERR -33         // Attempt to turn on the compressor on error (contact the developer)
#define ERR_CONFIG -34           // Internal configuration failed (contact the developer)
#define ERR_SD_INIT -35          // Error initializing the SD card
#define ERR_SD_INDEX -36         // Index.xxx file not found on SD card
#define ERR_SD_READ -37          // Error reading file from SD card
#define ERR_TYPE_OVERHEAT -38    // The rule for calculating overheating does not match the sensors (contact the developer)
#define ERR_485_INIT -39         // Inverter on Modbus was not found (inverter operation is prohibited)
#define ERR_485_BUZY -40         // When accessing port 485, the waiting time for its release was exceeded
// Errors described in the modbus protocol
#define ERR_MODBUS_0x01 -41       // Modbus 0x01 protocol illegal function exception
#define ERR_MODBUS_0x02 -42       // Modbus 0x02 protocol illegal data address exception
#define ERR_MODBUS_0x03 -43       // Modbus 0x03 protocol illegal data value exception
#define ERR_MODBUS_0x04 -44       // Modbus 0x4 protocol slave device failure exception
#define ERR_MODBUS_0xe0 -45       // Modbus 0xe0 Master invalid response slave ID exception
#define ERR_MODBUS_0xe1 -46       // Modbus 0xe1 Master invalid response function exception
#define ERR_MODBUS_0xe2 -47       // Modbus 0xe2 Master response timed out exception
#define ERR_MODBUS_0xe3 -48       // Modbus 0xe3 Master invalid response CRC exception
#define ERR_MODBUS_MX2_0x01 -49   // Omron mx2 Exception code 0x01 The specified function is not supported
#define ERR_MODBUS_MX2_0x02 -50   // Omron mx2 Exception code 0x02 The specified function was not found.
#define ERR_MODBUS_MX2_0x03 -52   // Omron mx2 Exception code 0x03 Unacceptable data format
#define ERR_MODBUS_MX2_0x05 -52   // Omron mx2 communication error over Modbus (function communication check 0x08 Omron mx2)
#define ERR_MODBUS_MX2_0x21 -53   // Omron mx2 Exception code 0x21 Data written to the storage register is outside the inverter
#define ERR_MODBUS_MX2_0x22 -54   // Omron mx2 Exception code 0x22 These functions are not available for the inverter
#define ERR_MODBUS_MX2_0x23 -55   // Omron mx2 Exception code 0x23 The register (bit) into which the value should be written is read-only
#define ERR_MODBUS_UNKNOW -56     // Modbus unknown error (protocol failure)
#define ERR_MODBUS_STATE -57      // Forbidden ( invalid ) state of the inverter
#define ERR_MODBUS_BLOCK -58      // Attempt to turn on the VT with the inverter locked
#define ERR_PID_FEED -59          // PID algorithm - reaching the maximum flow temperature (protection) The flow is the objective function, the protection is higher, and this should not be
#define ERR_OUT_OF_MEMORY -60     // Not enough memory to allocate arrays
#define ERR_SAVE_PROFILE -61      // Error writing profile in eeprom I2C
#define ERR_LOAD_PROFILE -62      // Error reading profile from eeprom I2C
#define ERR_CRC16_PROFILE -63     // Checksum error for profile
#define ERR_BAD_LEN_PROFILE -64   // Data size mismatch while reading profile
#define ERR_DS2482_NOT_FOUND -65  // DS2482 master was not found on the bus, I2C bus error is possible
#define ERR_DS2482_ONEWIRE -66    // DS2482 master cannot reset the OneWire bus PPD bit is 0
#define ERR_I2C_BUZY -67          // When accessing the I2C bus, the waiting time for its release was exceeded
#define ERR_DRV_EEV -68           // Driver L9333 ERV failed (driver protection worked)
#define ERR_HEADER2_EEPROM -69    // Counter header error in eeprom I2C
#define ERR_OPEN_I2C_JOURNAL -70  // Error opening the log in I2C memory (chip initialization)
#define ERR_READ_I2C_JOURNAL -71  // Error reading the log in I2C memory
#define ERR_WRITE_I2C_JOURNAL -72 // Error writing log in I2C memory
// #define ERR_ -73 //
#define ERR_MIN_FLOW -74       // The flow in the VET is below the set level
#define ERR_MAX_VOLTAGE -75    // Mains voltage too high (SDM120 data)
#define ERR_MAX_POWER -76      // Too much portable power (SDM120 data)
#define ERR_NO_MODBUS -77      // Modbus is required but not in the configuration
#define ERR_RESET_FC -78       // Failed to reset the inverter after an error
#define ERR_SEVA_FLOW -79      // There is no flow in the evaporator (SEVA operation)
#define ERR_COMP_NO_PUMP -80   // Attempt to turn on the compressor when the circuit pumps are not working.
#define ERR_DEFROST_R4WAY -81  // Erroneous configuration - attempt to defrost if it is not possible to switch to cooling (no R4WAY)
#define ERR_DEFROST -82        // Defrosting is required (there are conditions) during cooling
#define ERR_FC_CONF_ANALOG -83 // Error using analog control of the inverter without output
#define ERR_READ_TEMP -84      // Error reading the temperature sensor (read limit exceeded )
// #define ERR_ -85 //
#define ERR_ONEWIRE_CRC -86 // CRC error while reading OneWire
#define ERR_ONEWIRE_RW -87  // error while reading / writing OneWire
#define ERR_FC_FAULT -88    // inverter failure
#define ERR_FC_ERROR -89    // inverter control program error
#define ERR_SD_WRITE -90    // error writing to SD card
#define ERR_FC_RCOMP -91    // It is not possible to stop the inverter using RCOMP

#define ERR_ERRMAX -91 // Last error

// Global parameters of the inverter inverter on modbas depend on the compressor !!!!!!!!!
#define FC_MODBUS_ADR 1         // Address of the frequency converter on the bus must not match SMD_MODBUS_ADR
#define FC_TIME_READ (8 * 1000) // Inverter polling time in Moscow time (it was 6)
#define FC_NUM_READ 4           // The number of attempts to read the inverter (in a row) by modbass before it stopped the VT in error
#define FC_DELAY_REPEAT 40      // ms The time between REPEAT read attempts was 100
#define FC_DELAY_READ 5         // ms The time between consecutive requests was 20
#define FC_WRITE_READ 10        // ms Time between sequential recording

// SPECIFIC INVERTER OMRON MX2 -------------------------------------------- -------------------------------------------------- ----------------
// Control is frequency in hertz. Inside the frequency is stored in hundredths of hertz, the maximum possible frequency is 650 Hz !!
// Power stored in 0.1 kW
// Voltage at 0.1 V

// Omron MX2 Registers
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

// Setting the inverter for a specific compressor Registers Hxxx Permanent magnet motor (PM motor)
#define MX2_b171 0x13ae // b171 Inverter selection b171 read / write 0 (off), 1 (IM mode), 2 (high frequency mode), 3 (PM mode) = 03
#define MX2_b180 0x13b7 // b180 Initialization trigger = 01
#define MX2_H102 0x1571 // H102 Setting the PM engine code 00 (standard Omron data) 01 (auto-tuning data) = 1
#define MX2_H103 0x1572 // H103 PM engine power (0.1 / 0.2 / 0.4 / 0.55 / 0.75 / 1.1 / 1.5 / 2.2 / 3.0 / 3, 7 / 4.0 / 5.5 / 7.5 / 11.0 / 15.0 / 18.5) = 7
#define MX2_H104 0x1573 // H104 Setting the number of poles of the PM motor = 4
#define MX2_H105 0x1574 // H105 Rated current of the PM motor = 1000 (this is 11A)
#define MX2_H106 0x1575 // H106 PM motor constant R From 0.001 to 65.535 Ohms = 0.55
#define MX2_H107 0x1576 // H107 PM Engine Ld Constant From 0.01 to 655.35 mH = 2.31
#define MX2_H108 0x1577 // H108 Lq constant of PM engine From 0.01 to 655.35 mH = 2.7
#define MX2_H109 0x1578 // H109 Ke Engine Constant PM-motor 0.0001 ... 6.5535 Vmax ./ (rad / s) = 750 must be selected it affects consumption and noise
#define MX2_H110 0x1579 // (4 bytes) H110 PM motor constant J From 0.001 to 9999,000 kg / m² = 0.01
#define MX2_H111 0x157B // H111 Auto tuning constant R From 0.001 to 65.535 Ohms
#define MX2_H112 0x157C // H112 Auto-tuning constant Ld From 0.01 to 655.35 mH
#define MX2_H113 0x157D // H113 Auto tuning constant Lq From 0.01 to 655.35 mH
#define MX2_H116 0x1581 // H116 The response of the PM motor at a speed of 1 ... 1000 = 100 (default)
#define MX2_H117 0x1582 // H117 Starting current of the PM motor From 20.00 to 100.00% = 70 (default)
#define MX2_H118 0x1583 // H118 Starting time of the PM motor 0.01 ... 60.00 s = 1 (default)
#define MX2_H119 0x1584 // H119 Engine PM stabilization constant From 0 to 120% s = 100
#define MX2_H121 0x1586 // H121 Minimum frequency of the PM motor From 0.0 to 25.5% = 0
#define MX2_H122 0x1587 // H122 Idling current PM motor From 0.00 to 100.00% = 50 (default)
#define MX2_H123 0x1588 // H123 Choice of PM engine start method 00 (off) 01 (on) = 0 (default)
#define MX2_H131 0x158A // H131 Estimation of the initial position of the rotor of the PM motor: standby time 0 V 0 ... 255 = 10 (default)
#define MX2_H132 0x158B // H132 Assessment of the initial position of the rotor of the PM motor: waiting time for determination 0 ... 255 = 10 (default)
#define MX2_H133 0x158C // H133 Assessment of the initial position of the rotor of the PM motor: determination time 0 ... 255 = 30 (default)
#define MX2_H134 0x158D // H134 Assessment of the initial position of the rotor of the PM motor: voltage gain 0 ... 200 = 100 (default)
#define MX2_C001 0x1401 // C001 Input function [1] 0 (FW: go forward) = 0
#define MX2_C004 0x1404 // C004 Input function [4] 18 (RS: reset) = 18
#define MX2_C005 0x1405 // C005 Input function [5] [also input “PTC”] = 19 PTC Thermistor with positive TCS for thermal protection (only C005)
#define MX2_C026 0x1404 // C026 Relay output function 5 (AL: error signal) = 05
#define MX2_b091 0x135E // b091 Choice of stopping method 0 (braking to a complete stop), 1 (coasting stop) = 1
#define MX2_b021 0x1316 // b021 Operating mode with overload limitation 0 (off), 1 (enabled during acceleration and rotation at a constant speed),          \
                        //       2 (enabled during rotation at a constant speed), 3 (enabled during acceleration and rotation at a constant speed [increase \
                        //       speed in generator mode]) = 1
#define MX2_b022 0x1317 // b022 Overload restriction level 200 ... 2000 (0.1%) =
#define MX2_b023 0x1318 // b023 Braking time with overload limitation (0.1 sec) = 10
#define MX2_F002 0x1103 // (4 bytes) F002 Acceleration time (1) Standard, default acceleration, range from 0.001 to 3600 s (0.01 sec) = 20 * 100
#define MX2_F003 0x1105 // (4 bytes) F003 Deceleration time (1) Standard, default acceleration, range from 0.001 to 3600 s (0.01 sec) = 20 * 100
#define MX2_A001 0x1201 // A001 Frequency reference source 00 ... Potent. on external panels 01 ... Control terminals 02 ... Setting parameter F001 \
                        //       03 ... Input via ModBus network 04 ... Add. card 06 ... Entrance imp. after 07 ... via EzSQ 10 ... Result of arithmetic operation = 03
#define MX2_A002 0x1202 // A002 Source of the “Run” command 01 .. Control terminals 02 ... “Run” key on the keypad or digital panel 03 ... Input via the ModBus network 04 ... Add. card = 01
#define MX2_A003 0x1203 // A003 Main frequency Can be set in the range from 30 Hz to the maximum frequency (A004) (0.1 Hz) = 120 * 10
#define MX2_A004 0x1204 // A004 Maximum frequency Can be set in the range from the fundamental frequency to 400 Hz (0.1 Hz) = 120 * 10

// Omron MX2 Bits
#define MX2_START 0x0001     // (bit) Run command 1: Run, 0: Stop (valid with A002 = 03)
#define MX2_SET_DIR 0x0002   // (bit) Command of direction of rotation 1: Reverse rotation, 0: Rotation in the forward direction (valid with A002 = 03)
#define MX2_RESET 0x0004     // (bit) Reset emergency shutdown (RS) 1: Reset
#define MX2_READY 0x0011     // (bit) Ready IF 1: Ready, 0: Not ready
#define MX2_DIRECTION 0x0010 // (bit) Direction of rotation 1: Reverse rotation, 0: Rotation in the forward direction (deadlock with "d003")

#define TEST_NUMBER 1234 // Verification code for function 0x08

// Default register values ​​for NC (needed for programming the inverter (progFC method), required for a specific inverter and compressor)
#define valH102 0x01 // H102 Setting the PM engine code 00 (standard Omron data) 01 (auto-tuning data) = 1
#define valH103 7    // H103 PM engine power (0.1 / 0.2 / 0.4 / 0.55 / 0.75 / 1.1 / 1.5 / 2.2 / 3.0 / 3, 7 / 4.0 / 5.5 / 7.5 / 11.0 / 15.0 / 18.5) = 7
#define valH104 0x04 // H104 Setting the number of poles of the PM motor = 4
#define valH105 1000 // H105 Rated current of the PM motor = 1000 (this is 11A)
#define valH106 550  // H106 PM motor constant R From 0.001 to 65.535 Ohms = 0.55 * 1000
#define valH107 231  // H107 Constant Ld of the PM engine From 0.01 to 655.35 mH = 2.31 * 100
#define valH108 270  // H108 Lq constant of PM engine From 0.01 to 655.35 mH = 2.7 * 100
#define valH109 810  // H109 Constant Ke of the PM motor 0.0001 ... 6.5535 Vmax ./ (rad / s) = 750 must be selected influences consumption and noise
#define valH110 10   // H110 PM motor constant J From 0.001 to 9999,000 kg / m² = 0.01
#define valH119 100  // H119 Engine PM stabilization constant From 0 to 120% s = 100
#define valH121 10   // H121 Minimum frequency of the PM engine From 0.0 to 25.5% = 10 (default) (no need to change)
#define valH122 10   // H122 Idling current PM motor From 0.00 to 100.00% = 50 (default) (no need to change)
#define valC001 0    // C001 Input function [1] 0 (FW: go forward) = 0
#define valC004 18   // C004 Input function [4] 18 (RS: reset) = 18
#define valC005 19   // C005 Input function [5] [also PTC input] = 19 PTC Thermistor with positive TCS for thermal protection (only C005) (no need to change)
#define valC026 5    // C026 Relay output function 5 (AL: error signal) = 05
#define valb091 1    // b091 Choice of stopping method 0 (braking to a complete stop), 1 (coasting stop) = 1 (no need to change)
#define valb021 1    // b021 Operating mode with overload limitation = 1 (no need to change)
#define valb022 1000 // b022 Overload restriction level 200 ... 2000 (0.1%) =
#define valb023 10   // b023 Braking time with overload limitation (0.1 sec) = 10
#define valA001 3    // A001 Frequency reference source = 03 (no need to change)
#define valA002 1    // A002 Source of the “Move” command = 1 (no need to change)

#define DEF_FC_DT_COMP_TEMP (5 * 100)       // Защита по температуре компрессора - сколько градусов не доходит до максимальной (TCOMP) и при этом происходит уменьшение частоты
#define DEF_FC_UPTIME (10)                  // Время обновления алгоритма пид регулятора (мсек) Основной цикл управления
#define DEF_FC_PID_FREQ_STEP (2 * 100)      // Максимальный шаг (на увеличение) изменения частоты при ПИД регулировании в 0.01 Гц Необходимо что бы ЭРВ успевал
#define DEF_FC_PID_STOP 90                  // Проценты от уровня защит (мощность, ток, давление, темпеартура) при которой происходит блокировка роста частоты пидом (плавный подход к границе)
#define DEF_FC_START_FREQ (55 * 100)        // Стартовая частота частота инвертора отопление/охлаждение (см компрессор) в 0.01 Гц
#define DEF_FC_START_FREQ_BOILER (45 * 100) // Стартовая частота частота инвертора ГВС (см компрессор) в 0.01 Гц ГВС
#define DEF_FC_MIN_FREQ (40 * 100)          // Минимальная  частота инвертора при нагреве (см компрессор) в 0.01 Гц
#define DEF_FC_MIN_FREQ_COOL (60 * 100)     // Минимальная  частота инвертора при охлаждении в 0.01 Гц
#define DEF_FC_MIN_FREQ_BOILER (40 * 100)   // Минимальная  частота инвертора при нагреве ГВС в 0.01 Гц
#define DEF_FC_MIN_FREQ_USER (35 * 100)     // Минимальная  частота инвертора РУЧНОЙ РЕЖИМ (см компрессор) в 0.01 Гц
#define DEF_FC_MAX_FREQ (110 * 100)         // Максимальная частота инвертора при нагреве (см компрессор) в 0.01 Гц
#define DEF_FC_MAX_FREQ_COOL (100 * 100)    // Максимальная частота инвертора в режиме охлаждения  в 0.01 Гц
#define DEF_FC_MAX_FREQ_BOILER (50 * 100)   // Максимальная частота инвертора в режиме ГВС в 0.01 Гц поглощение бойлера обычно меньше чем СО
#define DEF_FC_MAX_FREQ_USER (120 * 100)    // Максимальная частота инвертора РУЧНОЙ РЕЖИМ (см компрессор) в 0.01 Гц
#define DEF_FC_STEP_FREQ (2 * 100)          // Шаг уменьшения частоты инвертора при достижении максимальной температуры, мощности и тока (см компрессор) в 0.01 Гц
#define DEF_FC_STEP_FREQ_BOILER (5 * 100)   // Шаг уменьшения частоты инвертора при достижении максимальной температуры, мощности и тока ГВС в 0.01 Гц
#define DEF_FC_DT_TEMP (1 * 100)            // Привышение температуры от уставок (подача) при которой срабатыват защита (уменьшается частота) в сотых градуса
#define DEF_FC_DT_TEMP_BOILER (2 * 100)     // Привышение температуры от уставок (подача) при которой срабатыват защита ГВС в сотых градуса

#define FC_DT_CON_PRESS 50                 // Защита по давлению компрессора - сколько сотых бара не доходит до максимальной (PCON) и при этом происходит уменьшение частоты
#define FC_MAX_POWER (1.8 * 1000)          // Максимальная мощность инвертора (см компрессор) в 0.1 кВт
#define FC_MAX_POWER_BOILER (1.3 * 1000)   // Максимальная мощность инвертора в режиме ГВС (см компрессор) в 0.1 кВт
#define FC_MAX_CURRENT (10.0 * 100)        // Максимальный ток инвертора (см компрессор) в 0.01 А
#define FC_MAX_CURRENT_BOILER (11.0 * 100) // Максимальный ток инвертора для ГВС в 0.01 А
#define FC_BASE_FREQ (120 * 100)           // Основная частота инвертора (см компрессор) в 0.01 Гц
#define FC_ACCEL_TIME (60 * 100)           // Время разгона (см компрессор) в 0.01 сек
#define FC_DEACCEL_TIME (60 * 100)         // Время торможения (см компрессор) в 0.01 сек
#define FC_START_PID_DELAY (30 * 100)      // Задержка ПИД после старта компрессора

// Значения по умолчанию (начало DEF_) не Константы
#define DEF_FC_DT_COMP_TEMP (5 * 100)       // Защита по температуре компрессора - сколько градусов не доходит до максимальной (TCOMP) и при этом происходит уменьшение частоты
#define DEF_FC_UPTIME (10)                  // Время обновления алгоритма пид регулятора (мсек) Основной цикл управления
#define DEF_FC_PID_FREQ_STEP (2 * 100)      // Максимальный шаг (на увеличение) изменения частоты при ПИД регулировании в 0.01 Гц Необходимо что бы ЭРВ успевал
#define DEF_FC_PID_STOP 90                  // Проценты от уровня защит (мощность, ток, давление, темпеартура) при которой происходит блокировка роста частоты пидом (плавный подход к границе)
#define DEF_FC_START_FREQ (55 * 100)        // Стартовая частота частота инвертора отопление/охлаждение (см компрессор) в 0.01 Гц
#define DEF_FC_START_FREQ_BOILER (45 * 100) // Стартовая частота частота инвертора ГВС (см компрессор) в 0.01 Гц ГВС
#define DEF_FC_MIN_FREQ (40 * 100)          // Минимальная  частота инвертора при нагреве (см компрессор) в 0.01 Гц
#define DEF_FC_MIN_FREQ_COOL (60 * 100)     // Минимальная  частота инвертора при охлаждении в 0.01 Гц
#define DEF_FC_MIN_FREQ_BOILER (40 * 100)   // Минимальная  частота инвертора при нагреве ГВС в 0.01 Гц
#define DEF_FC_MIN_FREQ_USER (35 * 100)     // Минимальная  частота инвертора РУЧНОЙ РЕЖИМ (см компрессор) в 0.01 Гц
#define DEF_FC_MAX_FREQ (130 * 100)         // Максимальная частота инвертора при нагреве (см компрессор) в 0.01 Гц
#define DEF_FC_MAX_FREQ_COOL (100 * 100)    // Максимальная частота инвертора в режиме охлаждения  в 0.01 Гц
#define DEF_FC_MAX_FREQ_BOILER (50 * 100)   // Максимальная частота инвертора в режиме ГВС в 0.01 Гц поглощение бойлера обычно меньше чем СО
#define DEF_FC_MAX_FREQ_USER (120 * 100)    // Максимальная частота инвертора РУЧНОЙ РЕЖИМ (см компрессор) в 0.01 Гц
#define DEF_FC_STEP_FREQ (2 * 100)          // Шаг уменьшения частоты инвертора при достижении максимальной температуры, мощности и тока (см компрессор) в 0.01 Гц
#define DEF_FC_STEP_FREQ_BOILER (5 * 100)   // Шаг уменьшения частоты инвертора при достижении максимальной температуры, мощности и тока ГВС в 0.01 Гц
#define DEF_FC_DT_TEMP (1 * 100)            // Привышение температуры от уставок (подача) при которой срабатыват защита (уменьшается частота) в сотых градуса
#define DEF_FC_DT_TEMP_BOILER (2 * 100)     // Привышение температуры от уставок (подача) при которой срабатыват защита ГВС в сотых градуса

struct type_errorMX2 // error structure
{
    uint16_t code;   // reason
    uint16_t status; // Inverter status on shutdown
    uint16_t noUse;  // Not used
    uint16_t fr;     // IF frequency during shutdown
    uint16_t cur;    // IF current on shutdown
    uint16_t vol;    // IF voltage when disconnected
    uint32_t time1;  // Total running time in STROKE mode when disconnected
    uint32_t time2;  // Total operating time of the inverter with the power on at the time of shutdown
};

union union_errorFC // Omron Error Translation
{
    type_errorMX2 MX2;
    uint16_t inputBuf[10];
};


typedef void *QueueHandle_t;


class devOmronMX2                                // Class frequency converter Omron MX2
{
    enum TEST_MODE
    {
        NORMAL = 0,
        SAFE_TEST,
        TEST,
        HARD_TEST // Обязательно должен быть последним, добавляем ПЕРЕД!!!
    };

public:
    int8_t initFC(); // Initialize Frequency
    int8_t progFC(); // Programming the inverter for a specific compressor

    __attribute__((always_inline)) inline boolean get_present() { return GETBIT(flags, fFC); } // The presence of the drive in the current configuration
    int8_t get_err() { return err; }                                                           // Get the last chastotnik error
    uint16_t get_numErr() { return numErr; }                                                   // Get the number of read errors
    void get_paramFC(char *var, char *ret);                                                    // Get the inverter parameter as a string
    boolean set_paramFC(char *var, float x);                                                   // Set the inverter parameter from the string

    // Get individual values
    uint16_t get_Uptime() { return _data.Uptime; }                   // Update time of the pid controller algorithm (sec) Main control loop
    uint16_t get_PidFreqStep() { return _data.PidFreqStep; }         // The maximum step (to increase) the frequency change with PID control of 0.01 Hz.
    uint16_t get_PidStop() { return _data.PidStop; }                 // Percentage of the level of protection (power, current, pressure, temperature) at which the frequency increase is blocked by the pid
    uint16_t get_dtCompTemp() { return _data.dtCompTemp; }           // Protection by compressor temperature - how many degrees does not reach the maximum (TCOMP) and at the same time the frequency decreases
    uint16_t get_startFreq() { return _data.startFreq; }             // Inverter start frequency (see compressor) at 0.01
    uint16_t get_startFreqBoiler() { return _data.startFreqBoiler; } // Starting frequency of the inverter (cm compressor) in 0.01 d.h.w.
    uint16_t get_minFreq() { return _data.minFreq; }                 // The minimum frequency of the inverter (see compressor) is 0.01
    uint16_t get_minFreqCool() { return _data.minFreqCool; }         // Minimum frequency of the inverter during cooling at 0.01
    uint16_t get_minFreqBoiler() { return _data.minFreqBoiler; }     // The minimum frequency of the inverter when heating the hot water in 0.01
    uint16_t get_minFreqUser() { return _data.minFreqUser; }         // Minimum frequency of the inverter MANUAL MODE (see compressor) in 0.01
    uint16_t get_maxFreq() { return _data.maxFreq; }                 // The maximum frequency of the inverter (see compressor) is 0.01
    uint16_t get_maxFreqCool() { return _data.maxFreqCool; }         // The maximum frequency of the inverter in cooling mode is 0.01
    uint16_t get_maxFreqBoiler() { return _data.maxFreqBoiler; }     // The maximum frequency of the inverter in the DHW mode at 0.01 Hz the absorption of the boiler is usually less than CO
    uint16_t get_maxFreqUser() { return _data.maxFreqUser; }         // Maximum frequency of the inverter MANUAL MODE (cm compressor) in 0.01
    uint16_t get_stepFreq() { return _data.stepFreq; }               // Step to decrease the inverter when reaching the maximum temperature, power and current (see compressor) in 0.01
    uint16_t get_stepFreqBoiler() { return _data.stepFreqBoiler; }   // Step to decrease the inverter when reaching the maximum temperature, power and DHW current of 0.01
    uint16_t get_dtTemp() { return _data.dtTemp; }                   // Temperature increase from the settings (feed) at which protection is activated (frequency decreases) in hundredths of a degree
    uint16_t get_dtTempBoiler() { return _data.dtTempBoiler; }       // Temperature increase from the settings (feed) at which the DHW protection in hundredths of a degree is triggered

    // Modbas control Common for all chastotnikov
    int16_t get_target() { return FC; }                                                // Get the target frequency of 0.01 hertz
    int8_t set_target(int16_t x, boolean show, int16_t _min, int16_t _max);            // Set the target frequency to 0.01 hertz show - show message or not + borders
    __attribute__((always_inline)) inline uint16_t get_power() { return power * 100; } // Get the current power. Watt unit
    __attribute__((always_inline)) inline uint16_t get_current() { return current; }   // Get the current current at 0.01A
    char *get_infoFC(char *bufn);                                                      // Get information about the chastotnik
    boolean reset_errorFC();                                                           // Reset Inverter Errors
    boolean reset_FC();                                                                // Reset the inverter via modbas
    int16_t read_stateFC();                                                            // Current state of the inverter
    int16_t read_tempFC();                                                             // Radiator temperature

    int16_t get_frequency() { return freqFC; }           // Get the current frequency of 0.01 Hz
    uint32_t get_startTime() { return startCompressor; } // Get the compressor start time
    int8_t get_readState();                              // Read (internal variables are updated) the state of the Inverter, returns either OK or an error
    int8_t start_FC();                                   // Command move to inverter (target frequency set)
    int8_t stop_FC();                                    // Stop command to the inverter
    boolean isfOnOff() { return GETBIT(flags, fOnOff); } // get the inverter status on or off

    void check_blockFC();                                   // Set the ban on the use of the inverter
    boolean get_blockFC() { return GETBIT(flags, fErrFC); } // Get the inverter lock flag
    union_errorFC get_errorFC() { return error; }           // Get structure with an error

    // Analog control
    int16_t get_DAC() { return dac; };          // Get the set DAC value
    int16_t get_level0() { return level0; }     // Get DAC samples              corresponding to 0 frequency
    int16_t get_level100() { return level100; } // Get DAC samples          corresponding to the maximum frequency
    int16_t get_levelOff() { return levelOff; } // Get The minimum frequency at which the frequency converter is turned off (minimum power limit)
    int8_t set_level0(int16_t x);               // Set DAC readings corresponding to 0 power
    int8_t set_level100(int16_t x);             // Set DAC readings corresponding to 100 power
    int8_t set_levelOff(int16_t x);             // Set the minimum power at which the chastotnik turns off (limiting the minimum frequency)
    uint8_t get_pinA() { return pin; }          // Leg where FC is attached

    // Service
    TEST_MODE get_testMode() { return testMode; };             // Get the current mode of operation
    void set_testMode(TEST_MODE t) { testMode = t; };          // Set the value to the current mode of operation
    char *get_note() { return note; }                          // Get a description
    char *get_name() { return name; }                          // Get the name
    uint8_t *get_save_addr(void) { return (uint8_t *)&_data; } // Address of the save structure
    uint16_t get_save_size(void) { return sizeof(_data); }     // Size of the save structure

private:
    int8_t err;         // error chastotnika (work) with an error stop TN
    uint16_t numErr;    // number of errors reading by modbass
    uint8_t number_err; // The number of communication errors when exceeding FC_NUM_READ inverter lock
                        // 485 control
    uint16_t FC;        // Inverter target frequency in 0.01 hertz
    uint16_t freqFC;    // Read: current inverter frequency in 0.01 hertz
    uint16_t power;     // Read: Current inverter power in 100 watt units (3 is 300 watts)
    uint16_t current;   // Read: Current inverter current in 0.01 Amp units

    int16_t state;            // Read: Status of the inverter register MX2_STATE
    uint32_t startCompressor; // compressor start time
    union_errorFC error;      // Structure for decoding the inverter error

    // Analog control
    int16_t dac;      // Current DAC value
    int16_t level0;   // DAC samples corresponding to 0 frequency
    int16_t level100; // DAC readings corresponding to the maximum frequency
    int16_t levelOff; // Minimum power at which the chastotnik turns off (minimum power limit)
    uint8_t pin;      // Leg where FC is attached

    TEST_MODE testMode; // The value of the test mode
    char *note;         // Description
    char *name;         // Inverter name

    // Structure for saving settings, Uptime is always the first
    struct
    {
        uint16_t Uptime;         // Pid controller update algorithm time (sec) Main control loop
        uint16_t PidFreqStep;    // The maximum step (to increase) the frequency change with PID control of 0.01 Hz It is necessary that the ERV be in time
        uint16_t PidStop;        // Percentage of the level of protection (power, current, pressure, temperature) at which the frequency increase is blocked by the pid
        uint16_t dtCompTemp;     // Protection by compressor temperature - how many degrees does not reach the maximum (TCOMP) and at the same time there is a decrease in frequency
        int16_t startFreq;       // Starting frequency of the inverter (cm compressor) in 0.01
        int16_t startFreqBoiler; // Starting frequency of the inverter (cm compressor) in 0.01 d.h.w.
        int16_t minFreq;         // The minimum frequency of the inverter (cm compressor) is 0.01
        int16_t minFreqCool;     // The minimum frequency of the inverter during cooling is 0.01
        int16_t minFreqBoiler;   // The minimum frequency of the inverter when heating the hot water in 0.01
        int16_t minFreqUser;     // The minimum frequency of the inverter MANUAL MODE (see compressor) in 0.01
        int16_t maxFreq;         // The maximum frequency of the inverter (cm compressor) is 0.01
        int16_t maxFreqCool;     // The maximum frequency of the inverter in cooling mode is 0.01
        int16_t maxFreqBoiler;   // The maximum frequency of the inverter in the DHW mode at 0.01 Hz the absorption of the boiler is usually less than CO
        int16_t maxFreqUser;     // Maximum inverter frequency MANUAL MODE (see compressor) at 0.01
        int16_t stepFreq;        // Step to decrease the inverter when reaching the maximum temperature, power and current (see compressor) in 0.01
        int16_t stepFreqBoiler;  // Step to decrease the inverter when reaching the maximum temperature, power and current of hot water in 0.01
        uint16_t dtTemp;         // Temperature increase from the settings (feed) at which the protection is activated (frequency decreases) in hundredths of a degree
        uint16_t dtTempBoiler;   // Temperature increase from the settings (supply) at which the DHW protection in hundredths of a degree is triggered
#ifdef FC_ANALOG_CONTROL
        int16_t level0;   // DAC samples corresponding to 0 frequency
        int16_t level100; // DAC readings corresponding to the maximum frequency
        int16_t levelOff; // Minimum power at which the chastotnik turns off (minimum power limit)
#endif
        uint8_t setup_flags; // setting flags
    } _data;                 // Structure for saving settings, setup_flags is always the last!
    uint8_t flags;           // setting flags

// Functions with OMRON MX2 Reading Registers
#ifndef FC_ANALOG_CONTROL                                              // NOT ANALOGUE MANAGEMENT
    int8_t write_0x10_32(uint16_t cmd, uint32_t data);                 // Writing data (4 bytes) to the cmd register returns an error code
    int16_t read_0x03_16(uint16_t cmd);                                // Modbus function 0x03 read 2 bytes
    uint32_t read_0x03_32(uint16_t cmd);                               // Modbus function 0x03 read 4 bytes
    int16_t read_0x03_error(uint16_t cmd);                             // Modbus function 0x03 error description num
    int8_t write_0x05_bit(uint16_t cmd, boolean f);                    // Writing a single bit to the cmd register returns an error code
    boolean read_0x01_bit(uint16_t cmd);                               // Reading a single bit into the cmd register returns an error code
    int8_t write_0x06_16(uint16_t cmd, uint16_t data);                 // Writing data (2 bytes) to the cmd register returns an error code
    int8_t progReg16(uint16_t adrReg, char *nameReg, uint16_t valReg); // Programming a separate inverter register with writing to the results log
    int8_t progReg32(uint16_t adrReg, char *nameReg, uint32_t valReg); // Programming two inverter registers with writing to the results log
#endif
};

class devModbus
{
public:
  int8_t initModbus();                                                                           // Инициализация Modbus и проверка связи возвращает ошибку
  __attribute__((always_inline)) inline boolean get_present() { return GETBIT(flags, 0); } // Наличие Modbus в текущей конфигурации
  int8_t readInputRegisters16(uint8_t id, uint16_t cmd, uint16_t *ret);
  int8_t readInputRegisters32(uint8_t id, uint16_t cmd, uint32_t *ret);                 // LITTLE-ENDIAN!
  int8_t readInputRegistersFloat(uint8_t id, uint16_t cmd, float *ret);                 // Получить значение 2-x (Modbus function 0x04 Read Input Registers) регистров (4 байта) в виде float возвращает код ошибки данные кладутся в ret
  int8_t readHoldingRegisters16(uint8_t id, uint16_t cmd, uint16_t *ret);               // Получить значение регистра (2 байта) МХ2 в виде целого  числа возвращает код ошибки данные кладутся в ret
  int8_t readHoldingRegisters32(uint8_t id, uint16_t cmd, uint32_t *ret);               // Получить значение 2-x регистров (4 байта) МХ2 в виде целого числа возвращает код ошибки данные кладутся в ret
  int8_t readHoldingRegistersFloat(uint8_t id, uint16_t cmd, float *ret);               // Получить значение 2-x регистров (4 байта) в виде float возвращает код ошибки данные кладутся в ret
  int8_t readHoldingRegistersNN(uint8_t id, uint16_t cmd, uint16_t num, uint16_t *buf); // Получить значение N регистров (2*N байта) МХ2 (положить в buf) возвращает код ошибки
  int8_t writeSingleCoil(uint8_t id, uint16_t cmd, uint8_t u8State);                    // установить битовый вход, возвращает код ошибки Modbus function 0x05 Write Single Coil.
  int8_t readCoil(uint8_t id, uint16_t cmd, boolean *ret);                              // прочитать отдельный бит, возвращает ошибку Modbus function 0x01 Read Coils.
  int8_t writeHoldingRegisters16(uint8_t id, uint16_t cmd, uint16_t data);              // Установить значение регистра (2 байта) МХ2 в виде целого  числа возвращает код ошибки данные data
  int8_t writeHoldingRegistersFloat(uint8_t id, uint16_t cmd, float dat);               // Записать float как 2 регистра числа возвращает код ошибки данные data
#ifndef FC_VACON
  int8_t LinkTestOmronMX2(); // Тестирование связи c МХ2 (актуально только с omronom) возвращает код ошибки
#endif
  int8_t writeHoldingRegisters32(uint8_t id, uint16_t cmd, uint32_t data); // Записать 2 регистра подряд возвращает код ошибки
  int8_t get_err() { return err; }                                         // Получить код ошибки
private:
  // Переменные
  int8_t flags;                        // Флаги
  int8_t err;                          // Ошибки модбас
  ModbusMaster RS485;                  // Класс модбас 485
  int8_t translateErr(uint8_t result); // Перевод ошибки протокола Модбас в ошибки ТН
};                                     // End class

#endif

#endif