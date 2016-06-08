/**
 * ArdubiosKey.cpp - Key handlers.
 * @version 1.0
 * @author Gabriele Falcioni <foss.dev@falcioni.net>
 */
#include "Arduino.h"
#include "ArdubiosKey.h"
#include "ArdubiosTicks.h"

#define KEY_TYPE(x) \
    ((x) & (Ardubios_KeyDownHigh | Ardubios_KeyDownLow | Ardubios_KeyDownLowPullup))

#define KEY_STATE(x) \
    ((x) & (Ardubios_KeyStateUp | Ardubios_KeyStateDown | Ardubios_KeyStateDownLong))

#define KEY_SET_STATE(x,y) \
    do { (x) &= ~KEY_STATE(0xFF); (x) |= KEY_STATE(y); } while (0)

#define KEY_WAS_DOWN(x) \
    (((x) & (Ardubios_KeyWasUp | Ardubios_KeyWasDown)) == Ardubios_KeyWasDown)

#define KEY_WAS_UP(x) \
    (((x) & (Ardubios_KeyWasUp | Ardubios_KeyWasDown)) == Ardubios_KeyWasUp)

#define KEY_SET_DOWN(x) \
    do { (x) &= ~(Ardubios_KeyWasUp | Ardubios_KeyWasDown); (x) |= Ardubios_KeyWasDown; } while (0)

#define KEY_SET_UP(x) \
    do { (x) &= ~(Ardubios_KeyWasUp | Ardubios_KeyWasDown); (x) |= Ardubios_KeyWasUp; } while (0)

void ArdubiosKey::define(id_t keyId,uint8_t keyType,int hwPin) {
    _id    = keyId;
    _pin   = hwPin;
    _flags = KEY_TYPE(keyType);
    _debounceTicks = _keypressTicks = 0;

    pinMode( _pin, KEY_TYPE(_flags) == Ardubios_KeyDownLowPullup ? INPUT_PULLUP : INPUT );
}

uint8_t ArdubiosKey::readKey() {
  return (digitalRead(_pin) ?
    (KEY_TYPE(_flags) & Ardubios_KeyDownLow ? Ardubios_KeyStateUp   : Ardubios_KeyStateDown ) :
    (KEY_TYPE(_flags) & Ardubios_KeyDownLow ? Ardubios_KeyStateDown : Ardubios_KeyStateUp   ) );
}

state_t ArdubiosKey::getStateImp() {
  return (_id ? KEY_STATE(_flags) : Ardubios_KeyStateUp);
}

boolean ArdubiosKey::processImp(uint8_t ticks) {
  if (_id && _debounceTicks != ticks) {
    uint8_t newState = readKey();

    if (KEY_WAS_UP(_flags)) {
      if (newState == Ardubios_KeyStateDown) {
        _debounceTicks = ticks;
        KEY_SET_DOWN(_flags);
        return false;
      }
    } else {  // if (KEY_WAS_DOWN(_flags))
      if (newState == Ardubios_KeyStateUp) {
        _debounceTicks = ticks;
        KEY_SET_UP(_flags);
        return false;
      }
    }

    switch ( KEY_STATE(_flags) ) {
      case Ardubios_KeyStateUp:
        if (newState == Ardubios_KeyStateDown) {
          KEY_SET_STATE(_flags,Ardubios_KeyStateDown);
          _keypressTicks = ticks;
          return true;
        }
        break;
      case Ardubios_KeyStateDown:
        if (newState == Ardubios_KeyStateDown) {
          if ((uint8_t)(ticks - _keypressTicks) >= TICKS_PER_SECOND) {
            KEY_SET_STATE(_flags,Ardubios_KeyStateDownLong);
            return true;
          }
        } else {
          KEY_SET_STATE(_flags,Ardubios_KeyStateUp);
          return true;
        }
        break;
      case Ardubios_KeyStateDownLong:
        if (newState == Ardubios_KeyStateUp) {
          KEY_SET_STATE(_flags,Ardubios_KeyStateUp);
          return true;
        }
        break;
    }
  }

  return false;
}

