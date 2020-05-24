/*
  FrSky sensor dynamic (FrSky-like) data polling class for Teensy 3.x/4.0/LC, ESP8266, ATmega2560 (Mega) and ATmega328P based boards (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 202000503
  Not for commercial use
*/

#include "FrSkySportPollingDynamic.h"

FrSkySportPollingDynamic::FrSkySportPollingDynamic() : FrSkySportPolling()
{
  nextPollStaticIdIdx = 0;
  nextPollDynamicIdIdx = 0;
  pollDynamicId = false;
  dynamicIdsCount = 0;
  for(uint8_t i = 0; i < FRSKY_POLLED_ID_COUNT; i++) dynamicIdsTable[i] = 0;
}

void FrSkySportPollingDynamic::sensorActive(FrSkySportSensor::SensorId id)
{
  uint8_t idIdx = id & 0x1F;
  if((idIdx >= 0) && (idIdx < FRSKY_POLLED_ID_COUNT) && (POLLED_ID_TABLE[idIdx] == id)) dynamicIdsTable[idIdx] = POLLING_DYNAMIC_ID_MAX_RETRY;
}

FrSkySportSensor::SensorId FrSkySportPollingDynamic::getNextId()
{
  uint8_t nextIdIdx = FRSKY_POLLED_ID_COUNT;
  if(pollDynamicId == true)
  {
    while(nextPollDynamicIdIdx < FRSKY_POLLED_ID_COUNT)
    {
      if(dynamicIdsTable[nextPollDynamicIdIdx] > 0)
      {
        dynamicIdsTable[nextPollDynamicIdIdx] -= 1;
        nextIdIdx = nextPollDynamicIdIdx;
        nextPollDynamicIdIdx++;
        dynamicIdsCount++;
        break;
      }
      nextPollDynamicIdIdx++;
    }
    if(nextPollDynamicIdIdx >= FRSKY_POLLED_ID_COUNT)
    {
      if(dynamicIdsCount < FRSKY_POLLED_ID_COUNT) pollDynamicId = false;
      nextPollDynamicIdIdx = 0;
      dynamicIdsCount = 0;
    }
  }
  if(nextIdIdx == FRSKY_POLLED_ID_COUNT)
  {
    for(uint8_t checkedStaticIds = 0; checkedStaticIds < FRSKY_POLLED_ID_COUNT; checkedStaticIds++)
    {
      if(nextPollStaticIdIdx >= FRSKY_POLLED_ID_COUNT) nextPollStaticIdIdx = 0;
      if(dynamicIdsTable[nextPollStaticIdIdx] == 0)
      {
        nextIdIdx = nextPollStaticIdIdx;
        nextPollStaticIdIdx++;
        break;
      }
      nextPollStaticIdIdx++;
    }
    pollDynamicId = true;
  }
  return POLLED_ID_TABLE[nextIdIdx];
}