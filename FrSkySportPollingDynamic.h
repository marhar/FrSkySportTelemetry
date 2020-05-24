/*
  FrSky sensor dynamic (FrSky-like) data polling class for Teensy 3.x/4.0/LC, ESP8266, ATmega2560 (Mega) and ATmega328P based boards (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 202000503
  Not for commercial use
*/

#ifndef _FRSKY_SPORT_POLLING_DYNAMIC_H_
#define _FRSKY_SPORT_POLLING_DYNAMIC_H_

#include "FrSkySportPolling.h"

#define POLLING_DYNAMIC_ID_MAX_RETRY 4

class FrSkySportPollingDynamic : public FrSkySportPolling
{
  public:
    FrSkySportPollingDynamic();
    virtual void sensorActive(FrSkySportSensor::SensorId id);

  private:
    virtual FrSkySportSensor::SensorId getNextId();
    uint8_t nextPollStaticIdIdx;
    uint8_t nextPollDynamicIdIdx;
    bool pollDynamicId;
    uint8_t dynamicIdsCount;
    uint8_t dynamicIdsTable[FRSKY_POLLED_ID_COUNT];
};

#endif // _FRSKY_SPORT_POLLING_DYNAMIC_H_

