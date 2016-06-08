/**
 * ArdubiosLed.cpp - LED handlers.
 * @version 1.0
 * @author Gabriele Falcioni <foss.dev@falcioni.net>
 */
#include "Arduino.h"
#include "ArdubiosLed.h"

#define LED_TYPE(x) \
    ((x) & (Ardubios_LedOnHigh | Ardubios_LedOnLow))

#define LED_STATE(x) \
    ((x) & (Ardubios_LedStateOff | Ardubios_LedStateOn | Ardubios_LedStateBlinkSlow \
      | Ardubios_LedStateBlinkFast | Ardubios_LedStateHeartbeat))

#define LED_ON(x) \
    (((x) & (Ardubios_LedOn | Ardubios_LedOff)) == Ardubios_LedOn)

#define LED_OFF(x) \
    (((x) & (Ardubios_LedOn | Ardubios_LedOff)) == Ardubios_LedOff)

void ArdubiosLed::define(id_t ledId,uint8_t ledType,int hwPin) {
    _id    = ledId;
    _pin   = hwPin;
    _flags = LED_TYPE(ledType);

    pinMode(_pin,OUTPUT);
    digitalWrite( _pin, (LED_TYPE(_flags) == Ardubios_LedOnLow ? HIGH : LOW) ); // LED OFF
}

void ArdubiosLed::setState(uint8_t ledState) {
  if (_id) {
    _flags &= ~LED_STATE(0xFF);
    _flags |=  LED_STATE(ledState);
  }
}

void ArdubiosLed::ledOn() {
  if (_id && LED_OFF(_flags)) {
    digitalWrite( _pin, (LED_TYPE(_flags) == Ardubios_LedOnLow ? LOW : HIGH) ); // LED ON
    _flags |= Ardubios_LedOn;
  }
}

void ArdubiosLed::ledOff() {
  if (_id && LED_ON(_flags)) {
    digitalWrite( _pin, (LED_TYPE(_flags) == Ardubios_LedOnLow ? HIGH : LOW) ); // LED OFF
    _flags &= ~Ardubios_LedOn;
  }
}

state_t ArdubiosLed::getStateImp() {
  return LED_STATE(_flags);
}

boolean ArdubiosLed::processImp(uint8_t ticks) {
  if (_id) {
    switch ( LED_STATE(_flags) ) {
      case Ardubios_LedStateOff:
        ledOff();
        break;
      case Ardubios_LedStateOn:
        ledOn();
        break;
      case Ardubios_LedStateBlinkSlow:
        if (ticks & 0x10) ledOn(); else ledOff();
        break;
      case Ardubios_LedStateBlinkFast:
        if (ticks & 0x04) ledOn(); else ledOff();
        break;
      case Ardubios_LedStateHeartbeat:
        if ((ticks & 0xFE) == 0x02) ledOn(); else ledOff();
        break;
    }
  }
  return false;
}

