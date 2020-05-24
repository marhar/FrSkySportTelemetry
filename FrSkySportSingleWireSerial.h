/*
  FrSky single wire serial class for Teensy 3.x/4.0/LC, ESP8266, ATmega2560 (Mega) and ATmega328P based boards (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 202000503
  Not for commercial use
*/

#ifndef _FRSKY_SPORT_SINGLE_WIRE_SERIAL_
#define _FRSKY_SPORT_SINGLE_WIRE_SERIAL_

#include "Arduino.h"
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MKL26Z64__) || defined(__MK66FX1M0__) || defined(__MK64FX512__) || defined(__IMXRT1062__)
#define TEENSY_HW
#endif

#if !defined(TEENSY_HW)
#include "SoftwareSerial.h"
#endif

#define FRSKY_TELEMETRY_START_FRAME 0x7E
#define FRSKY_SENSOR_DATA_FRAME 0x10
#define FRSKY_SENSOR_EMPTY_FRAME 0x00
#define FRSKY_STUFFING 0x7D

class FrSkySportSingleWireSerial
{
  public:
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MKL26Z64__)
    enum SerialId { SERIAL_USB = 0, SERIAL_1 = 1, SERIAL_2 = 2, SERIAL_3 = 3 };
#elif defined(__MK66FX1M0__) || defined(__MK64FX512__) 
    enum SerialId { SERIAL_USB = 0, SERIAL_1 = 1, SERIAL_2 = 2, SERIAL_3 = 3, SERIAL_4 = 4 , SERIAL_5 = 5 , SERIAL_6 = 6 };
#elif defined(__IMXRT1062__)
    enum SerialId { SERIAL_USB = 0, SERIAL_1 = 1, SERIAL_2 = 2, SERIAL_3 = 3, SERIAL_4 = 4 , SERIAL_5 = 5 , SERIAL_6 = 6 , SERIAL_7 = 7 };
#elif defined(ESP8266) 
    enum SerialId { SOFT_SERIAL_PIN_4 = 4, SOFT_SERIAL_PIN_D2 = 4, SOFT_SERIAL_PIN_5 = 5, SOFT_SERIAL_PIN_D1 = 5, SOFT_SERIAL_PIN_12 = 12, SOFT_SERIAL_PIN_D6 = 12,
                    SOFT_SERIAL_PIN_13 = 13, SOFT_SERIAL_PIN_D7 = 13, SOFT_SERIAL_PIN_14 = 14, SOFT_SERIAL_PIN_D5 = 14, SOFT_SERIAL_PIN_15 = 15 , SOFT_SERIAL_PIN_D8 = 15 };
#elif defined(__AVR_ATmega2560__)
    enum SerialId { SOFT_SERIAL_PIN_10 = 10, SOFT_SERIAL_PIN_11 = 11, SOFT_SERIAL_PIN_12 = 12, SOFT_SERIAL_PIN_13 = 13, SOFT_SERIAL_PIN_14 = 14, SOFT_SERIAL_PIN_15 = 15,
                    SOFT_SERIAL_PIN_50 = 50, SOFT_SERIAL_PIN_51 = 51, SOFT_SERIAL_PIN_52 = 52, SOFT_SERIAL_PIN_53 = 53 };
#else
    enum SerialId { SOFT_SERIAL_PIN_2 = 2, SOFT_SERIAL_PIN_3 = 3, SOFT_SERIAL_PIN_4 = 4, SOFT_SERIAL_PIN_5 = 5, SOFT_SERIAL_PIN_6 = 6, SOFT_SERIAL_PIN_7 = 7,
                     SOFT_SERIAL_PIN_8 = 8, SOFT_SERIAL_PIN_9 = 9, SOFT_SERIAL_PIN_10 = 10, SOFT_SERIAL_PIN_11 = 11, SOFT_SERIAL_PIN_12 = 12 };
#endif
    FrSkySportSingleWireSerial();
    void begin(SerialId id);
    void sendHeader(uint8_t id);
    void sendData(uint16_t dataTypeId, uint32_t id);
    void sendEmpty(uint16_t dataTypeId);
    Stream* port;

  private:
    enum SerialMode { RX = 0, TX = 1 };
    void setMode(SerialMode mode);
    void sendByte(uint8_t byte);
    void sendCrc();
#if defined(TEENSY_HW)
#if defined(__IMXRT1062__)
    volatile uint32_t *uartCtrl;
#else
    volatile uint8_t *uartCtrl;   
#endif
#else
    SoftwareSerial* softSerial;
    SerialId softSerialId;
#endif
    uint16_t crc;
};

#endif // _FRSKY_SPORT_SINGLE_WIRE_SERIAL_
