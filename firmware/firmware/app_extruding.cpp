#include <Vector.h>
#include <Streaming.h>
#include <Arduino.h>
#include "app.h"
#include "features.h"

// #define HAS_EXTRUDE_DEBUG

#ifdef HAS_EXTRUDE_DEBUG
    #define EXTRUDE_DEBUG(A) Serial.println(A);
#else
    #define EXTRUDE_DEBUG(A)
#endif

short App::setOverload(short val)
{
    
}

ushort App::loop_auto_reverse()
{

#ifdef HAS_POWER
    if (!powerSwitch->isOn(POWER_PRIMARY))
    {
        return E_POWER;
    }
#endif

    return E_OK;
}
short App::extrude(short value)
{
    
}

ushort App::loopExtrude()
{

}
