/**
 * ArdubiosTicks.cpp - Ticks tracker and generator.
 * @version 1.0
 * @author Gabriele Falcioni <foss.dev@falcioni.net>
 */
#include "Arduino.h"
#include "ArdubiosTicks.h"

ArdubiosTicks::ArdubiosTicks() {
   reset();
}

void ArdubiosTicks::reset() {
  _lastMillis = (uint16_t)millis(); // LSW
  _lastTicks  = 0;
}

uint8_t ArdubiosTicks::elapsed() {
  uint16_t ms = (uint16_t)millis(); // LSW

  uint16_t elapsed = ms - _lastMillis;
  uint8_t ticks    = elapsed / MILLIS_PER_TICK;
  _lastMillis += ticks * MILLIS_PER_TICK;
  _lastTicks  += ticks;

  return _lastTicks;
}

