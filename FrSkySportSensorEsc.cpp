/*
  FrSky ESC sensor class for Teensy 3.x/4.0/LC, ESP8266, ATmega2560 (Mega) and ATmega328P based boards (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 202000503
  Not for commercial use
*/

#include "FrSkySportSensorEsc.h" 

FrSkySportSensorEsc::FrSkySportSensorEsc(SensorId id) : FrSkySportSensor(id) { }

void FrSkySportSensorEsc::setData(float volt, float curr, uint32_t rpm, uint16_t cons, float temp, float sbecVolt, float sbecCurr)
{
  powerData = (((uint32_t)round(curr * 100)) << 16) + (uint32_t)round(volt * 100);
  rpmConsData = (((uint32_t)cons) << 16) + (rpm / 100);
  tempData = (int32_t)round(temp);
  sbecData = (((uint32_t)round(sbecCurr * 1000)) << 16) + (uint32_t)round(sbecVolt * 1000);
}

uint16_t FrSkySportSensorEsc::send(FrSkySportSingleWireSerial& serial, uint8_t id, uint32_t now)
{
  uint16_t dataId = SENSOR_NO_DATA_ID;
  if(sensorId == id)
  {
    switch(sensorDataIdx)
    {
      case 0:
        dataId = ESC_POWER_DATA_ID;
        if(now > powerTime)
        {
          powerTime = now + ESC_POWER_DATA_PERIOD;
          serial.sendData(dataId, powerData);
        }
        else
        {
          serial.sendEmpty(dataId);
          dataId = SENSOR_EMPTY_DATA_ID;
        }
        break;
      case 1:
        dataId = ESC_RPM_CONS_DATA_ID;
        if(now > rpmConsTime)
        {
          rpmConsTime = now + ESC_RPM_CONS_DATA_PERIOD;
          serial.sendData(dataId, rpmConsData);
        }
        else
        {
          serial.sendEmpty(dataId);
          dataId = SENSOR_EMPTY_DATA_ID;
        }
        break;
      case 2:
        dataId = ESC_TEMP_DATA_ID;
        if(now > tempTime)
        {
          tempTime = now + ESC_TEMP_DATA_PERIOD;
          serial.sendData(dataId, tempData);
        }
        else
        {
          serial.sendEmpty(dataId);
          dataId = SENSOR_EMPTY_DATA_ID;
        }
        break;
      case 3:
        dataId = ESC_SBEC_DATA_ID;
        if(now > sbecTime)
        {
          sbecTime = now + ESC_SBEC_DATA_PERIOD;
          serial.sendData(dataId, sbecData);
        }
        else
        {
          serial.sendEmpty(dataId);
          dataId = SENSOR_EMPTY_DATA_ID;
        }
        break;
    }
    sensorDataIdx++;
    if(sensorDataIdx >= ESC_DATA_COUNT) sensorDataIdx = 0;
  }
  return dataId;
}

uint16_t FrSkySportSensorEsc::decodeData(uint8_t id, uint16_t appId, uint32_t data)
{
  if((sensorId == id) || (sensorId == FrSkySportSensor::ID_IGNORE))
  {
    switch(appId)
    {
      case ESC_POWER_DATA_ID:
        volt = (data & 0xFFFF) / 100.0;
        curr = (data >> 16) / 100.0;
        return appId;
      case ESC_RPM_CONS_DATA_ID:
        rpm = (data & 0xFFFF) * 100;
        cons = data >> 16;
        return appId;
      case ESC_TEMP_DATA_ID:
        temp = (int32_t)data;
        return appId;
      case ESC_SBEC_DATA_ID:
        sbecVolt = (data & 0xFFFF) / 1000.0;
        sbecCurr = (data >> 16) / 1000.0;
        return appId;
    }
  }
  return SENSOR_NO_DATA_ID;
}

float FrSkySportSensorEsc::getVoltage() { return volt; }
float FrSkySportSensorEsc::getCurrent() { return curr; }
uint32_t FrSkySportSensorEsc::getRpm() { return rpm; }
uint16_t FrSkySportSensorEsc::getConsumption() { return cons; }
int32_t FrSkySportSensorEsc::getTemp() { return temp; }
float FrSkySportSensorEsc::getSbecVoltage() { return sbecVolt; }
float FrSkySportSensorEsc::getSbecCurrent() { return sbecCurr; }
