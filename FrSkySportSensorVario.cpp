/*
  FrSky Variometer (high precision) sensor class for Teensy 3.x/4.0/LC, ESP8266, ATmega2560 (Mega) and ATmega328P based boards (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 202000503
  Not for commercial use
*/

#include "FrSkySportSensorVario.h" 

FrSkySportSensorVario::FrSkySportSensorVario(SensorId id) : FrSkySportSensor(id) { }

void FrSkySportSensorVario::setData(float altitude, float vsi)
{
  altitudeData = (int32_t)(altitude * 100);
  vsiData = (int32_t)(vsi * 100);
}

uint16_t FrSkySportSensorVario::send(FrSkySportSingleWireSerial& serial, uint8_t id, uint32_t now)
{
  uint16_t dataId = SENSOR_NO_DATA_ID;
  if(sensorId == id)
  {
    switch(sensorDataIdx)
    {
      case 0:
        dataId = VARIO_ALT_DATA_ID;
        if(now > altitudeTime)
        {
          altitudeTime = now + VARIO_ALT_DATA_PERIOD;
          serial.sendData(dataId, altitudeData);
        }
        else
        {
          serial.sendEmpty(dataId);
          dataId = SENSOR_EMPTY_DATA_ID;
        }
        break;
      case 1:
        dataId = VARIO_VSI_DATA_ID;
        if(now > vsiTime)
        {
          vsiTime = now + VARIO_VSI_DATA_PERIOD;
          serial.sendData(dataId, vsiData);
        }
        else
        {
          serial.sendEmpty(dataId);
          dataId = SENSOR_EMPTY_DATA_ID;
        }
        break;
    }
    sensorDataIdx++;
    if(sensorDataIdx >= VARIO_DATA_COUNT) sensorDataIdx = 0;
  }
  return dataId;
}

uint16_t FrSkySportSensorVario::decodeData(uint8_t id, uint16_t appId, uint32_t data)
{
  if((sensorId == id) || (sensorId == FrSkySportSensor::ID_IGNORE))
  {
    switch(appId)
    {
      case VARIO_ALT_DATA_ID:
        altitude = ((int32_t)data) / 100.0;
        return appId;
      case VARIO_VSI_DATA_ID:
        vsi = ((int32_t)data) / 100.0;
        return appId;
    }
  }
  return SENSOR_NO_DATA_ID;
}

float FrSkySportSensorVario::getAltitude() { return altitude; }
float FrSkySportSensorVario::getVsi() { return vsi; }
