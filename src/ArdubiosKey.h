#ifndef ArdubiosKey_h
#define ArdubiosKey_h

#include <inttypes.h>

#include "ArdubiosEventGen.h"

/**
 * Key's states.
 * Ardubios_KeyStateUp - The key is up (not pressed).
 * Ardubios_KeyStateDown - The key is down (pressed).
 * Ardubios_KeyStateDownLong - The key was pressed down for at least a second.
 */
#define Ardubios_KeyStateUp       0x00
#define Ardubios_KeyStateDown     0x01
#define Ardubios_KeyStateDownLong 0x03

/**
 * Key's types.
 * Ardubios_KeyDownHigh - The key drives the input port high when pressed.
 * Ardubios_KeyDownLow - The key drives the input port low when pressed,
 *    disable the port's internal pullup resistor.
 * Ardubios_KeyDownLowPullup - The key drives the input port low when pressed,
 *    enable the port's internal pullup resistor.
 */
#define Ardubios_KeyDownHigh      0x00
#define Ardubios_KeyDownLow       0x40
#define Ardubios_KeyDownLowPullup (0x40 | 0x20)

/**
 * Internal state for debouncing algorithm.
 */
#define Ardubios_KeyWasUp         0x00
#define Ardubios_KeyWasDown       0x80

/**
 * Implements debounced keys.
 * Also detects long keypresses (about 1 second).
 * This class adopts the CRTP (Curiously recurring template pattern)
 * to avoid the memory cost of a virtual function table imposed by
 * C++ inehritance.
 */
class ArdubiosKey :
  public ArdubiosEventGen<ArdubiosKey> {
  friend class ArdubiosEventGen<ArdubiosKey>;

  public:

   /**
     * Constructor for an undefined key.
     * Ensure that define() is called before using the key.
     */
    inline ArdubiosKey() { _id = 0; }

    /**
     * Constructor for a key.
     * Same as a define() call.
     * @param keyId Bitmask that identifies the key.
     * @param keyType Value that identifies the broad category of the key (see above).
     * @param hwPin Value that identifies the hardware pin tied to the key.
     */
    inline ArdubiosKey(id_t keyId,uint8_t keyType,int hwPin) { define(keyId,keyType,hwPin); }

    /**
     * Defines a key.
     * @param keyId Bitmask that identifies the key.
     * @param keyType Value that identifies the broad category of the key (see above).
     * @param hwPin Value that identifies the hardware pin tied to the key.
     */
    void define(id_t keyId,uint8_t keyType,int hwPin);

  private:
    uint8_t _pin;
    uint8_t _flags;
    uint8_t _debounceTicks;
    uint8_t _keypressTicks;

    uint8_t readKey();

    /**
     * Gets the key's state (implementation).
     * State is updated when process() is called.
     * @return The key's state (see above).
     */
    state_t getStateImp();

    /**
     * Computes the key's new state (implementation).
     * Call this function at least every 10 milliseconds.
     * @param ticks The elapsed ticks counter.
     * @returns true if the key's state has changed, false otherwise.
     *  The key's state may be retrieved calling getState().
     */
    boolean processImp(uint8_t ticks);

};

#endif

