#ifndef ArdubiosLed_h
#define ArdubiosLed_h

#include <inttypes.h>

#include "ArdubiosEventGen.h"

/**
 * LED's state.
 * Ardubios_LedStateOff - The LED is off.
 * Ardubios_LedStateOn - The LED is on.
 * Ardubios_LedStateBlinkSlow - The LED blinks.
 * Ardubios_LedStateBlinkFast - The LED blinks fast.
 * Ardubios_LedStateHeartbeat - The LED lights up briefly every 3-4 seconds.
 */
#define Ardubios_LedStateOff        0x00
#define Ardubios_LedStateOn         0x01
#define Ardubios_LedStateBlinkSlow  0x02
#define Ardubios_LedStateBlinkFast  0x03
#define Ardubios_LedStateHeartbeat  0x04

/**
 * LED's type.
 * Ardubios_LedOnHigh - The LED lights up when the port is driven high.
 * Ardubios_LedOnLow - The LED lights up when the port is driven low.
 */
#define Ardubios_LedOnHigh 0x00
#define Ardubios_LedOnLow  0x40

/**
 * Internal LED state.
 */
#define Ardubios_LedOff 0x00
#define Ardubios_LedOn  0x80

/**
 * Implements animated LEDs.
 * This class adopts the CRTP (Curiously recurring template pattern)
 * to avoid the memory cost of a virtual function table imposed by
 * C++ inehritance.
 */
class ArdubiosLed :
  public ArdubiosEventGen<ArdubiosLed> {
  friend class ArdubiosEventGen<ArdubiosLed>;

  public:

    /**
     * Constructor for an undefined LED.
     * Ensure that define() is called before using the LED.
     */
    inline ArdubiosLed() { _id = 0; }

    /**
     * Constructor for a LED.
     * Same as a define() call.
     * @param ledId Bitmask that identifies the LED.
     * @param ledType Value that identifies the broad category of LED light (see above).
     * @param hwPin Value that identifies the hardware pin tied to the LED.
     */
    inline ArdubiosLed(id_t ledId,uint8_t ledType,int hwPin) { define(ledId,ledType,hwPin); }

    /**
     * Defines a LED.
     * @param ledId Bitmask that identifies the LED.
     * @param ledType Value that identifies the broad category of LED light (see above).
     * @param hwPin Value that identifies the hardware pin tied to the LED.
     */
    void define(id_t ledId,uint8_t ledType,int hwPin);

    /**
     * Sets the LED's state.
     * State is updated when process() is called.
     * @param ledState The LED's new state.
     */
    void setState(uint8_t ledState);

    //
    // Utilities
    //

    inline void off()       { setState( Ardubios_LedStateOff       ); }
    inline void on()        { setState( Ardubios_LedStateOn        ); }
    inline void blinkSlow() { setState( Ardubios_LedStateBlinkSlow ); }
    inline void blinkFast() { setState( Ardubios_LedStateBlinkFast ); }
    inline void heartbeat() { setState( Ardubios_LedStateHeartbeat ); }

  private:
    uint8_t _pin;
    uint8_t _flags;

    void ledOn();
    void ledOff();

    /**
     * Gets the LED's state (implementation).
     * @return The LED's state (see above).
     */
    state_t getStateImp();

    /**
     * Animates the LED (implementation).
     * Call this function at least every 10 milliseconds.
     * @param ticks The elapsed ticks counter.
     * @returns always false (by design).
     */
    boolean processImp(uint8_t ticks);

};

#endif

