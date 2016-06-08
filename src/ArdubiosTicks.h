/**
 * ArdubiosTicks.h - Ticks tracker and generator.
 * @version 1.0
 * @author Gabriele Falcioni <foss.dev@falcioni.net>
 */
#ifndef ArdubiosTicks_h
#define ArdubiosTicks_h

#include <inttypes.h>

#define TICKS_PER_SECOND 50
#define MILLIS_PER_TICK (1000 / TICKS_PER_SECOND)

/**
 * Elapsed ticks tracker.
 */
class ArdubiosTicks {

  public:
    ArdubiosTicks();

    /**
     * Resets the elapsed ticks counter.
     */
    void reset();

    /**
     * Gets the elapsed ticks since last call.
     * Call this function at least every 10 milliseconds.
     * If called less than every 10 ms, this function can track delays up to 1 sec
     * but clients such as timers and led blinks will be delayed and key presses may be lost.
     * @returns the elapsed ticks since last call.
     */
    uint8_t elapsed();

  private:
   uint16_t _lastMillis;
   uint8_t  _lastTicks;

};

#endif

