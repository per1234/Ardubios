/**
 * ArdubiosTimer.cpp - Timer handlers.
 * @version 1.0
 * @author Gabriele Falcioni <foss.dev@falcioni.net>
 */
#include "Arduino.h"
#include "ArdubiosTimer.h"

#define TIMER_TYPE(x) \
    ((x) & (Ardubios_TimerTypeOneShot | Ardubios_TimerTypeAutoReset))

#define TIMER_STATE(x) \
    ((x) & (Ardubios_TimerStateReset | Ardubios_TimerStateRunning | Ardubios_TimerStateExpired))

void ArdubiosTimer::define(id_t timerId,uint8_t timerType) {
  _id       = timerId;
  _flags    = TIMER_TYPE(timerType);
  _interval = 0;
   reset();
}

void ArdubiosTimer::setInterval(uint16_t ticks) {
  _interval = ticks;
  reset();
}

void ArdubiosTimer::start(uint8_t ticks) {
  if (_id && !running()) {
    if (_running == 0) _running = _interval;
    _ticks  = ticks;
    _flags |= Ardubios_TimerStateRunning;
  }
}

void ArdubiosTimer::stop() {
  if (_id && running()) {
    _flags &= ~Ardubios_TimerStateRunning;
  }
}

void ArdubiosTimer::reset() {
  if (_id) {
    _running = 0;
    _flags &= ~TIMER_STATE(0xFF);
  }
}

void ArdubiosTimer::resetState() {
  if (_id) {
    _flags &= ~Ardubios_TimerStateExpired;
  }
}

state_t ArdubiosTimer::getStateImp() {
  return (_id ? TIMER_STATE(_flags) : Ardubios_TimerStateReset);
}

boolean ArdubiosTimer::processImp(uint8_t ticks) {
  boolean rc = false;

  if (_id && running()) {
    uint8_t elapsed = ticks - _ticks;
    while (elapsed) {
      if (elapsed < _running) {
        _running -= elapsed;
        elapsed = 0;
      } else {
        elapsed -= _running;
        _running = 0;
      }

      if (_running == 0) {
        _flags |= Ardubios_TimerStateExpired;
        rc = true;
        if (TIMER_TYPE(_flags) == Ardubios_TimerTypeOneShot) {
          _flags &= ~Ardubios_TimerStateRunning;
          break;
        } else {
          _running = _interval;
        }
      }
    }

    _ticks = ticks;
  }

  return rc;
}

