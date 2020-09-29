#ifndef ADDON_H
#define ADDON_H

#include <WString.h>
#include <Vector.h>

#include "enums.h"
#include "common/macros.h"

#define ADDON_NORMAL 1 << LOOP | 1 << SETUP
#ifdef HAS_STATES
    #define ADDON_STATED ADDON_NORMAL | 1 << STATE
#else
    #define ADDON_STATED ADDON_NORMAL
#endif

class Stream;

class Addon
{

public:
    const String name;
    const short id;
    millis_t now;
    millis_t last;
    millis_t dt;
    Addon(String _name, short _id) : name(_name),
                                     id(_id),
                                     now(0),
                                     last(0),
                                     dt(0)
    {
        flags = ADDON_NORMAL;
    }

    Addon(String _name, short _id, short _flags) : name(_name),
                                                   id(_id),
                                                   flags(_flags)
    {
    }

    virtual void debug(Stream *stream);
    virtual void info(Stream *stream);
    virtual short setup(){};
    virtual short loop(){};
    virtual short ok(){};
    virtual bool pause(){};
    virtual bool resume(){};
    virtual bool destroy(){};
    virtual String state() { return ""; };

    int flags;
    void setFlag(uchar flag);
    bool hasFlag(uchar flag);
    void clearFlag(uchar flag);
    void enable();
    void disable();
    bool enabled();
};

typedef Vector<Addon *> Addons;
Addon *byId(Addons addons, uchar id);
typedef short (Addon::*AddonFnPtr)(short);

#endif