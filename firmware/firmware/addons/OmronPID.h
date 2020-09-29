#ifndef OMRON_PID_H
#define OMRON_PID_H

#ifdef HAS_STATES
#include <ArduinoJson.h>
#endif

#include <Streaming.h>
#include "../Addon.h"
#include "../config.h"
#include "../common/macros.h"

// Addon to detect when there is something in the hopper
class OmronPID : public Addon
{
public:

#ifdef HAS_STATES
  String HopperLoaded::state()
  {
    const int capacity = JSON_OBJECT_SIZE(2);
    StaticJsonDocument<capacity> doc;
    doc["0"] = id;
    doc["1"] = ok() ? 1 : 0;
    return doc.as<String>();
  }
#endif


  Addon(OMRON_PID_STR, OMRON_PID, ADDON_STATED)
  {
    // this->setFlag(DEBUG);
  }

  virtual short loop()
  {
    
  }

  void debug(Stream *stream)
  {
    // *stream << this->name << ":" << this->ok() << "\n\t" << SPACE("h ts " << this->sensor0.highTS) << "\n\t" << SPACE("l ts " << this->sensor0.lowTS) << "\n\t" << SPACE("dt  " << this->sensor0.dt);
  }
  void info(Stream *stream)
  {

  }
#ifdef TESTING
  bool test;
#endif
};

#endif