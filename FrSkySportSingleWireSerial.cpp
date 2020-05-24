/*
  FrSky single wire serial class for Teensy 3.x/4.0/LC, ESP8266, ATmega2560 (Mega) and ATmega328P based boards (e.g. Pro Mini, Nano, Uno)
  (c) Pawelsky 202000503
  Not for commercial use
*/

#include "FrSkySportSingleWireSerial.h"

FrSkySportSingleWireSerial::FrSkySportSingleWireSerial()
{
  port = NULL;
#if defined(TEENSY_HW)
  uartCtrl = NULL;
#else
  softSerial = NULL;
#endif
}

void FrSkySportSingleWireSerial::begin(SerialId id)
{
#if defined(TEENSY_HW)
  if(id == SERIAL_USB) // Not really single wire, but added for debug purposes via USB
  {
    port = &Serial;
    Serial.begin(57600);
  }
  else if(id == SERIAL_1)
  {
    port = &Serial1;
    Serial1.begin(57600, SERIAL_8N1_RXINV_TXINV); // Start Serial1 with RX and TX inverted
    // Put Serial1 into single wire mode
#if defined(__IMXRT1062__)
    uartCtrl = &LPUART6_CTRL;
    LPUART6_CTRL |= (LPUART_CTRL_LOOPS | LPUART_CTRL_RSRC);
    IOMUXC_LPUART6_TX_SELECT_INPUT = 1;
#else
    uartCtrl = &UART0_C3;
    UART0_C1 |= (UART_C1_LOOPS | UART_C1_RSRC);  
#endif
  }
  else if(id == SERIAL_2)
  {
    port = &Serial2;
    Serial2.begin(57600, SERIAL_8N1_RXINV_TXINV); // Start Serial2 with RX and TX inverted
    // Put Serial2 into single wire mode
#if defined(__IMXRT1062__)
    uartCtrl = &LPUART4_CTRL;
    LPUART4_CTRL |= (LPUART_CTRL_LOOPS | LPUART_CTRL_RSRC);
    IOMUXC_LPUART4_TX_SELECT_INPUT = 2;
#else
    uartCtrl = &UART1_C3;
    UART1_C1 |= (UART_C1_LOOPS | UART_C1_RSRC);
#endif
  }
  else if(id == SERIAL_3)
  {
    port = &Serial3;
    Serial3.begin(57600, SERIAL_8N1_RXINV_TXINV); // Start Serial3 with RX and TX inverted
    // Put Serial3 into single wire mode
#if defined(__IMXRT1062__)
    uartCtrl = &LPUART2_CTRL;
    LPUART2_CTRL |= LPUART_CTRL_LOOPS | LPUART_CTRL_RSRC;
    IOMUXC_LPUART2_TX_SELECT_INPUT = 1;
#else
    uartCtrl = &UART2_C3;
    UART2_C1 |= (UART_C1_LOOPS | UART_C1_RSRC);
#endif
  }
  #if defined(__MK66FX1M0__) || defined(__MK64FX512__) || defined(__IMXRT1062__)
  else if(id == SERIAL_4)
  {
    port = &Serial4;
    Serial4.begin(57600, SERIAL_8N1_RXINV_TXINV); // Start Serial4 with RX and TX inverted
    // Put Serial4 into single wire mode
#if defined(__IMXRT1062__)
    uartCtrl = &LPUART3_CTRL;
    LPUART3_CTRL |= (LPUART_CTRL_LOOPS | LPUART_CTRL_RSRC);
    IOMUXC_LPUART3_TX_SELECT_INPUT = 0;
#else
    uartCtrl = &UART3_C3;
    UART3_C1 |= (UART_C1_LOOPS | UART_C1_RSRC);
#endif
  }
  else if(id == SERIAL_5)
  {
    port = &Serial5;
    Serial5.begin(57600, SERIAL_8N1_RXINV_TXINV); // Start Serial5 with RX and TX inverted
    // Put Serial5 into single wire mode
#if defined(__IMXRT1062__)
    uartCtrl = &LPUART8_CTRL;
    LPUART8_CTRL |= (LPUART_CTRL_LOOPS | LPUART_CTRL_RSRC);
    IOMUXC_LPUART8_TX_SELECT_INPUT = 1;
#else
    uartCtrl = &UART4_C3;
    UART4_C1 |= (UART_C1_LOOPS | UART_C1_RSRC);
#endif
  }
  else if(id == SERIAL_6)
  {
    port = &Serial6;
    Serial6.begin(57600, SERIAL_8N1_RXINV_TXINV); // Start Serial6 with RX and TX inverted
    // Put Serial6 into single wire mode
#if defined(__IMXRT1062__)
    uartCtrl = &LPUART1_CTRL;
    LPUART1_CTRL |= (LPUART_CTRL_LOOPS | LPUART_CTRL_RSRC);
#else
    uartCtrl = &UART5_C3;
    UART5_C1 |= (UART_C1_LOOPS | UART_C1_RSRC);
#endif
  }
#endif
#if defined(__IMXRT1062__)
  else if(id == SERIAL_7)
  {
    port = &Serial7;
    Serial7.begin(57600, SERIAL_8N1_RXINV_TXINV); // Start Serial7 with RX and TX inverted
    // Put Serial7 into single wire mode
    uartCtrl = &LPUART7_CTRL;
    LPUART7_CTRL |= (LPUART_CTRL_LOOPS | LPUART_CTRL_RSRC);
    IOMUXC_LPUART7_TX_SELECT_INPUT = 1;
  }
#endif
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(ESP8266)
  if(softSerial != NULL)
  {
    delete softSerial;
    softSerial = NULL;
  }
  softSerialId = id;
  softSerial = new SoftwareSerial(softSerialId, softSerialId, true);
  port = softSerial;
  softSerial->begin(57600);
#else
  #error "Unsupported processor! Only Teesny 3.x/4.0/LC, ESP8266, ATmega2560 and ATmega328P based boards are supported.";
#endif
  crc = 0;
  setMode(RX);
}

void FrSkySportSingleWireSerial::setMode(SerialMode mode)
{
#if defined(TEENSY_HW)
  if((port != NULL) && (uartCtrl != NULL))
  {
    if(mode == TX)
    {
#if defined(__IMXRT1062__)
        *uartCtrl |= LPUART_CTRL_TXDIR;
#else
        *uartCtrl |= UART_C3_TXDIR;
#endif
    }
    else if(mode == RX)
    {
#if defined(__IMXRT1062__)
        *uartCtrl &= ~LPUART_CTRL_TXDIR;
#else
        *uartCtrl &= ~UART_C3_TXDIR;
#endif
    }
  }
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(ESP8266)
  if(port != NULL)
  {
    if(mode == TX)
    {
        pinMode(softSerialId, OUTPUT);
    }
    else if(mode == RX)
    {
        pinMode(softSerialId, INPUT);
    }
  }
#endif
}

void FrSkySportSingleWireSerial::sendHeader(uint8_t id)
{
  if(port != NULL)
  {
    setMode(TX);
    port->write(FRSKY_TELEMETRY_START_FRAME);
    port->write(id);
    port->flush();
    setMode(RX);
  }
}

void FrSkySportSingleWireSerial::sendByte(uint8_t byte)
{
  if(port != NULL)
  {
    if(byte == 0x7E)
    {
      port->write(FRSKY_STUFFING);
      port->write(0x5E); // 0x7E xor 0x20
    }
    else if(byte == 0x7D)
    {
      port->write(FRSKY_STUFFING);
      port->write(0x5D); // 0x7D xor 0x20
    }
    else
    {
      port->write(byte);
    }
    crc += byte;
    crc += crc >> 8; crc &= 0x00ff;
  }
}

void FrSkySportSingleWireSerial::sendCrc()
{
  // Send and reset CRC
  sendByte(0xFF - crc);
  crc = 0;
}

void FrSkySportSingleWireSerial::sendData(uint16_t dataTypeId, uint32_t data)
{
  if(port != NULL)
  {
    setMode(TX);
    sendByte(FRSKY_SENSOR_DATA_FRAME);
    uint8_t *bytes = (uint8_t*)&dataTypeId;
    sendByte(bytes[0]);
    sendByte(bytes[1]);
    bytes = (uint8_t*)&data;
    sendByte(bytes[0]);
    sendByte(bytes[1]);
    sendByte(bytes[2]);
    sendByte(bytes[3]);
    sendCrc();
    port->flush();
    setMode(RX);
  }
}

void FrSkySportSingleWireSerial::sendEmpty(uint16_t dataTypeId)
{
  if(port != NULL)
  {
    setMode(TX);
    sendByte(FRSKY_SENSOR_EMPTY_FRAME);
    uint8_t *bytes = (uint8_t*)&dataTypeId;
    sendByte(bytes[0]);
    sendByte(bytes[1]);
    for(uint8_t i = 0; i < 4; i++) sendByte(0x00);
    sendCrc();
    port->flush();
    setMode(RX);
  }
}
