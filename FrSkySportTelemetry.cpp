/*
  FrSky telemetry class for Teensy 3.x/4.0/LC, ESP8266, ATmega2560 (Mega) and ATmega328P based boards (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 202000503
  Not for commercial use
*/

#include "FrSkySportPollingSimple.h"
#include "FrSkySportTelemetry.h"

// This constructor is obsolete and kept for backward compatibility only
FrSkySportTelemetry::FrSkySportTelemetry(bool polling)
{
  if(polling == true) FrSkySportTelemetry(new FrSkySportPollingSimple());
  else FrSkySportTelemetry();
}

FrSkySportTelemetry::FrSkySportTelemetry(FrSkySportPolling *polling)
{
  pollingClass = polling;
  prevData = FrSkySportSensor::ID_IGNORE;
}

void FrSkySportTelemetry::begin(FrSkySportSingleWireSerial::SerialId id,
                                FrSkySportSensor* sensor1,  FrSkySportSensor* sensor2,  FrSkySportSensor* sensor3,
                                FrSkySportSensor* sensor4,  FrSkySportSensor* sensor5,  FrSkySportSensor* sensor6,
                                FrSkySportSensor* sensor7,  FrSkySportSensor* sensor8,  FrSkySportSensor* sensor9,
                                FrSkySportSensor* sensor10, FrSkySportSensor* sensor11, FrSkySportSensor* sensor12,
                                FrSkySportSensor* sensor13, FrSkySportSensor* sensor14, FrSkySportSensor* sensor15,
                                FrSkySportSensor* sensor16, FrSkySportSensor* sensor17, FrSkySportSensor* sensor18,
                                FrSkySportSensor* sensor19, FrSkySportSensor* sensor20, FrSkySportSensor* sensor21,
                                FrSkySportSensor* sensor22, FrSkySportSensor* sensor23, FrSkySportSensor* sensor24,
                                FrSkySportSensor* sensor25, FrSkySportSensor* sensor26, FrSkySportSensor* sensor27,
                                FrSkySportSensor* sensor28)
{
  // Store sensor references in array
  sensors[0] = sensor1;
  sensors[1] = sensor2;
  sensors[2] = sensor3;
  sensors[3] = sensor4;
  sensors[4] = sensor5;
  sensors[5] = sensor6;
  sensors[6] = sensor7;
  sensors[7] = sensor8;
  sensors[8] = sensor9;
  sensors[9] = sensor10;
  sensors[10] = sensor11;
  sensors[11] = sensor12;
  sensors[12] = sensor13;
  sensors[13] = sensor14;
  sensors[14] = sensor15;
  sensors[15] = sensor16;
  sensors[16] = sensor17;
  sensors[17] = sensor18;
  sensors[18] = sensor19;
  sensors[19] = sensor20;
  sensors[20] = sensor21;
  sensors[21] = sensor22;
  sensors[22] = sensor23;
  sensors[23] = sensor24;
  sensors[24] = sensor25;
  sensors[25] = sensor26;
  sensors[26] = sensor27;
  sensors[27] = sensor28;

  // Count sensors (stops at first NULL)
  for(sensorCount = 0; sensorCount < FRSKY_TELEMETRY_MAX_SENSORS; sensorCount++)
  {
    if(sensors[sensorCount] == NULL) break;
  }

  FrSkySportTelemetry::serial.begin(id);
}

uint16_t FrSkySportTelemetry::send()
{
  uint16_t dataId = SENSOR_NO_DATA_ID;
  if(serial.port != NULL)
  {
    uint8_t polledId = FrSkySportSensor::ID_IGNORE;
    uint32_t now = millis();

    if(pollingClass != NULL)
    {
      polledId = pollingClass->pollData(serial, now);
      if(polledId != FrSkySportSensor::ID_IGNORE) prevData = polledId;
    }

    if(serial.port->available())
    {
      uint8_t data = serial.port->read();
      if(pollingClass != NULL)
      {
        // If an external sensor is responding (with actual data or empty frame) and polling is enabled, notify the polling class
        if((prevData != FrSkySportSensor::ID_IGNORE) && ((data == FRSKY_SENSOR_DATA_FRAME) || (data == FRSKY_SENSOR_EMPTY_FRAME))) pollingClass->sensorActive((FrSkySportSensor::SensorId)prevData);
        prevData = FrSkySportSensor::ID_IGNORE;
      }
      else
      {
        if(prevData == FRSKY_TELEMETRY_START_FRAME) polledId = data;
        prevData = data;
      }
    }

    if(polledId != FrSkySportSensor::ID_IGNORE) 
    {
      // Send the actual data
      for(uint8_t i = 0; i < sensorCount; i++)
      {
        dataId = sensors[i]->send(serial, polledId, now);
        // If an internal sensor is is responding (with actual data or empty frame) and polling is enabled, notify the polling class
        if((pollingClass != NULL) && (dataId != SENSOR_NO_DATA_ID)) pollingClass->sensorActive((FrSkySportSensor::SensorId)polledId);
      }
    }
  }
  if(dataId == SENSOR_EMPTY_DATA_ID) dataId = SENSOR_NO_DATA_ID; // If empty frame was sent we return SENSOR_NO_DATA_ID as no actual data has been sent
  return dataId;
}

void FrSkySportTelemetry::setData(uint8_t rssi, float rxBatt)
{
  if (pollingClass != NULL) pollingClass->setData(rssi, rxBatt);
}
