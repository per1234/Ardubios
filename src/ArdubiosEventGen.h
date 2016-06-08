#ifndef ArdubiosEventGen_h
#define ArdubiosEventGen_h

#include <inttypes.h>

typedef uint8_t id_t;
typedef uint8_t state_t;

/**
 * Event descriptor.
 */
struct ArdubiosEvent {
  id_t    id;
  state_t state;
};

/**
 * Base class for event generators.
 * This class adopts the CRTP (Curiously recurring template pattern)
 * to avoid the memory cost of a virtual function table imposed by
 * C++ inehritance.
 * See: https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
 */
template<class G>
class ArdubiosEventGen {

  public:

    /**
     * Gets the generator's ID.
     */
    inline id_t getId() { return _id; }

    /**
     * Gets the generator's state.
     * State is updated when process() is called.
     * @return The generator's state (see the generator's header file).
     */
    inline state_t getState() { return static_cast<G*>(this)->getStateImp(); }

    /**
     * Computes the generator's new state.
     * Call this function at least every 10 milliseconds.
     * @param ticks The elapsed ticks counter.
     * @returns true if the generator's state has changed, false otherwise.
     *  The generator's state may be retrieved calling getState().
     */
    inline boolean process(uint8_t ticks) { return static_cast<G*>(this)->processImp(ticks); }

  protected:
    id_t _id;

};

#endif

