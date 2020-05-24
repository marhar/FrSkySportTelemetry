/*
  FrSky ESC sensor class for Teensy 3.x/4.0/LC, ESP8266, ATmega2560 (Mega) and ATmega328P based boards (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 202000503
  Not for commercial use
*/

#ifndef _FRSKY_SPORT_SENSOR_ESC_H_
#define _FRSKY_SPORT_SENSOR_ESC_H_

#include "FrSkySportSensor.h"

#define ESC_DEFAULT_ID ID17
#define ESC_DATA_COUNT 4

#define ESC_POWER_DATA_ID 0x0B50
#define ESC_RPM_CONS_DATA_ID 0x0B60
#define ESC_TEMP_DATA_ID 0x0B70
#define ESC_SBEC_DATA_ID 0x0E50

#define ESC_POWER_DATA_PERIOD 300
#define ESC_RPM_CONS_DATA_PERIOD 300
#define ESC_TEMP_DATA_PERIOD 300
#define ESC_SBEC_DATA_PERIOD 300

class FrSkySportSensorEsc : public FrSkySportSensor
{
  public:
    FrSkySportSensorEsc(SensorId id = ESC_DEFAULT_ID);
    void setData(float volt, float curr, uint32_t rpm, uint16_t cons, float temp, float sbecVolt = 0.0, float sbecCurr = 0.0);
    virtual uint16_t send(FrSkySportSingleWireSerial& serial, uint8_t id, uint32_t now);
    virtual uint16_t decodeData(uint8_t id, uint16_t appId, uint32_t data);
    float getVoltage();
    float getCurrent();
    uint32_t getRpm();
    uint16_t getConsumption();
    int32_t getTemp();
    float getSbecVoltage();
    float getSbecCurrent();

  private:
    uint32_t powerData;
    int32_t rpmConsData;
    int32_t tempData;
    uint32_t sbecData;
    uint32_t powerTime;
    uint32_t rpmConsTime;
    uint32_t tempTime;
    uint32_t sbecTime;
    float volt;
    float curr;
    uint32_t rpm;
    uint16_t cons;
    int32_t temp;
    float sbecVolt;
    float sbecCurr;
};

#endif // _FRSKY_SPORT_SENSOR_ESC_H_
