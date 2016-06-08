#ifndef ArdubiosTimer_h
#define ArdubiosTimer_h

#include <inttypes.h>

#include "ArdubiosEventGen.h"

/**
 * Timer's states.
 * Ardubios_TimerStateReset - Timer is idle.
 * Ardubios_TimerStateRunning - Timer is running.
 * Ardubios_TimerStateExpired - Timer has expired.
 *    An expired timer may also be either idle or running.
 */
#define Ardubios_TimerStateReset   0x00
#define Ardubios_TimerStateRunning 0x01
#define Ardubios_TimerStateExpired 0x02

/**
 * Timer's types.
 * Ardubios_TimerTypeOneShot - The timer reverts to idle state when expires.
 * Ardubios_TimerTypeAutoReset - The timer keeps running when expires.
 *    The same interval is automatically reloaded.
 */
#define Ardubios_TimerTypeOneShot   0x00
#define Ardubios_TimerTypeAutoReset 0x10

/**
 * Implements virtual timers.
 * This class adopts the CRTP (Curiously recurring template pattern)
 * to avoid the memory cost of a virtual function table imposed by
 * C++ inehritance.
 */
class ArdubiosTimer :
  public ArdubiosEventGen<ArdubiosTimer> {
  friend class ArdubiosEventGen<ArdubiosTimer>;

  public:

   /**
     * Constructor for an undefined timer.
     * Ensure that define() is called before using the timer.
     */
    inline ArdubiosTimer() { _id = 0; }

    /**
     * Constructor for a timer.
     * Same as a define() call.
     * @param timerId Bitmask that identifies the timer.
     * @param timerType Value that identifies the broad category of the timer (see above).
     */
    inline ArdubiosTimer(id_t timerId,uint8_t timerType) { define(timerId,timerType); }

    /**
     * Defines the timer.
     * @param timerId Bitmask that identifies the timer.
     * @param timerType Value that identifies the broad category of the timer (see above).
     */
    void define(id_t timerId,uint8_t timerType);

    /**
     * Sets the timer's interval.
     * If the timer is running, it's stopped first.
     * The timer triggered bit is cleared.
     * The internal counters are reset.
     * The timer changes state when the specified interval has elapsed.
     * @param ticks The elapsed ticks counter.
     */
    void setInterval(uint16_t ticks);

    /**
     * Starts the timer.
     * If the timer is running, does nothing.
     * @param ticks The elapsed ticks counter. Used to initialise internal counters.
     */
    void start(uint8_t ticks);

    /**
     * Stops the timer.
     * If the timer is running, it's stopped.
     * The internal counters are unchanged.
     */
    void stop();

    /**
     * Resets the timer.
     * If the timer is running, it's stopped first.
     * The timer triggered bit is cleared.
     * The internal counters are reset.
     */
    void reset();

    /**
     * Resets the timer's state.
     * The timer triggered bit is cleared.
     * The internal counters are unchanged.
     */
    void resetState();

    /**
     * Checks if the timer is idle.
     * @return true if the timer is idle, false otherwise.
     */
    inline boolean idle() { return ((_flags & Ardubios_TimerStateRunning) == 0); }

    /**
     * Checks if the timer is running.
     * @return true if the timer is running, false otherwise.
     */
    inline boolean running() { return ((_flags & Ardubios_TimerStateRunning) != 0); }

    /**
     * Checks if the timer has expired.
     * @return true if the timer has expired, false otherwise.
     */
    inline boolean expired() { return ((_flags & Ardubios_TimerStateExpired) != 0); }

  private:
    uint8_t  _flags;
    uint8_t  _ticks;
    uint16_t _interval;
    uint16_t _running;

    /**
     * Gets the timer's state (implementation).
     * State is updated when process() is called.
     * @return The timer's state (see above).
     */
    state_t getStateImp();

    /**
     * Updates a running timer (implementation).
     * Call this function at least every 10 milliseconds.
     * @param ticks The elapsed ticks counter.
     * @returns true if the timer's state has changed, false otherwise.
     *  The timer's state may be retrieved calling getState().
     */
    boolean processImp(uint8_t ticks);

};

#endif

