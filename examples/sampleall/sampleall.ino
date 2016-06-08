#include <Ardubios.h>

//
// H/W MAP
//

#define PIN_LED_STATE    13 // D13, digital output
#define PIN_KEY_CONTINUE 3  // D3, digital input
#define PIN_RND_INIT     14 // A0, analog input

//
// Globals
//

ArdubiosTicks tickBase;
ArdubiosKey   theKey(0x01,Ardubios_KeyDownLowPullup,PIN_KEY_CONTINUE);
ArdubiosLed   theLed(0x02,Ardubios_LedOnHigh,12);
ArdubiosLed   arduinoLed(0x04,Ardubios_LedOnHigh,PIN_LED_STATE);
ArdubiosTimer theTimer(0x08,Ardubios_TimerTypeOneShot);

void setup() {
  tickBase.reset();
  arduinoLed.heartbeat();
  theTimer.setInterval(3 * TICKS_PER_SECOND);
}

//
// MAIN LOOP
//
void loop() {
  uint8_t ticks = tickBase.elapsed();
  if (theKey.process(ticks)) {
    switch (theKey.getState()) {
      case Ardubios_KeyStateUp:
        theTimer.reset();
        theLed.off();
        break;
      case Ardubios_KeyStateDown:
        theLed.blinkFast();
        break;
      case Ardubios_KeyStateDownLong:
        theLed.blinkSlow();
        theTimer.start(ticks);
        break;
    }
  }
  if (theTimer.process(ticks)) {
    if (theTimer.expired()) {
      theTimer.reset();
      theLed.on();
    }
  }
  (void)theLed.process(ticks);
  (void)arduinoLed.process(ticks);
}
