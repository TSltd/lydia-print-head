#ifndef MODBUS_BRIDGE_H
#define MODBUS_BRIDGE_H

#include "Addon.h"
#include <Controllino.h>

class ModbusBridge : public Addon
{
public:
    ModbusBridge() : Addon("ModbusBridge", 50, ADDON_NORMAL)
    {
        setFlag(DEBUG);
    }
    

    // This uint16 array specified internal registers in the Modbus slave device.
    // Each Modbus device has particular internal registers that are available for the Modbus master.
    // In this example sketch internal registers are defined as follows:
    // (ModbusSlaveRegisters 0 - 3 read only and ModbusSlaveRegisters 4 - 7 write only from the Master perspective):
    // ModbusSlaveRegisters[0] - Read an analog value from the CONTROLLINO_A0 - returns value in the range from 0 to 1023.
    // ModbusSlaveRegisters[1] - Read an digital value from the CONTROLLINO_D0 - returns only the value 0 or 1.
    // ModbusSlaveRegisters[2] - Read the number of incoming messages - Communication diagnostic.
    // ModbusSlaveRegisters[3] - Read the number of number of outcoming messages - Communication diagnostic.
    // ModbusSlaveRegisters[4] - Sets the Relay output CONTROLLINO_R0 - only the value 0 or 1 is accepted.
    // ModbusSlaveRegisters[5] - Sets the Relay output CONTROLLINO_R1 - only the value 0 or 1 is accepted.
    // ModbusSlaveRegisters[6] - Sets the Relay output CONTROLLINO_R2 - only the value 0 or 1 is accepted.
    // ModbusSlaveRegisters[7] - Sets the Relay output CONTROLLINO_R3 - only the value 0 or 1 is accepted.
    uint16_t ModbusSlaveRegisters[8];

    // This is an structe which contains a query to an slave device
    

    void debug(Stream *stream);
    void info(Stream *stream);
    short setup();
    short loop();
    short setFn(short val);
    short setAddr(short val);
    short setNb(short val);
    short run(short val);
    short fn;
    short addr;
    short nb;
};

#endif