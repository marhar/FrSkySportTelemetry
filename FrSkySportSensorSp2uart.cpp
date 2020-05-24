/*
  FrSky S.Port to UART Remote (Type B) converter class for Teensy 3.x/4.0/LC, ESP8266, ATmega2560 (Mega) and ATmega328P based boards (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 202000503
  Not for commercial use
  
  Note that only analog ports ADC3 and ADC4 are implemented, not the UART part.
*/

#include "FrSkySportSensorSp2uart.h" 

FrSkySportSensorSp2uart::FrSkySportSensorSp2uart(SensorId id) : FrSkySportSensor(id) { }

void FrSkySportSensorSp2uart::setData(float adc3, float adc4)
{
  adc3Data = (uint32_t)(adc3 * 100);
  adc4Data = (uint32_t)(adc4 * 100);
}

uint16_t FrSkySportSensorSp2uart::send(FrSkySportSingleWireSerial& serial, uint8_t id, uint32_t now)
{
  uint16_t dataId = SENSOR_NO_DATA_ID;
  if(sensorId == id)
  {
    switch(sensorDataIdx)
    {
      case 0:
        dataId = SP2UARTB_ADC3_DATA_ID;
        if(now > adc3Time)
        {
          adc3Time = now + SP2UARTB_ADC3_DATA_PERIOD;
          serial.sendData(dataId, adc3Data);
        }
        else
        {
          serial.sendEmpty(dataId);
          dataId = SENSOR_EMPTY_DATA_ID;
        }
        break;
      case 1:
        dataId = SP2UARTB_ADC4_DATA_ID;
        if(now > adc4Time)
        {
          adc4Time = now + SP2UARTB_ADC4_DATA_PERIOD;
          serial.sendData(dataId, adc4Data);
        }
        else
        {
          serial.sendEmpty(dataId);
          dataId = SENSOR_EMPTY_DATA_ID;
        }
        break;
    }
    sensorDataIdx++;
    if(sensorDataIdx >= SP2UARTB_DATA_COUNT) sensorDataIdx = 0;
  }
  return dataId;
}

uint16_t FrSkySportSensorSp2uart::decodeData(uint8_t id, uint16_t appId, uint32_t data)
{
  if((sensorId == id) || (sensorId == FrSkySportSensor::ID_IGNORE))
  {
    switch(appId)
    {
      case SP2UARTB_ADC3_DATA_ID:
        adc3 = data / 100.0;
        return appId;
      case SP2UARTB_ADC4_DATA_ID:
        adc4 = data / 100.0;
        return appId;
    }
  }
  return SENSOR_NO_DATA_ID;
}

float FrSkySportSensorSp2uart::getAdc3() { return adc3; }
float FrSkySportSensorSp2uart::getAdc4() { return adc4; }
