#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#ifdef DMGC_SDL_ARDUINO_BUILD
#include "dmgc-utils.h"
#else
#include "src/dmgc-utils/dmgc-utils.h"
#endif

#include "pixel-magic.h"

/* Button mapper and poller */
DMGC_UTILS::ButtonHandler buttons;
DMGC_UTILS::Button * NAVIGATION = NULL, 
                   * LEFT = NULL,
                   * RIGHT = NULL,
                   * SELECT = NULL;

// Delay time
const uint8_t    DEBOUNCE_DELAY = 170;
PixelMagic magic;

void setup() {

  // Setup initial items
  magic.setup();

  // Add buttons
  NAVIGATION = buttons.add(PCINT3);
  LEFT = buttons.add(PCINT2);
  RIGHT = buttons.add(PCINT1);
  SELECT = buttons.add(PCINT0);

  buttons.poll(HIGH);

  // Skip intro if nav switch is pushed in
  //if (digitalRead(pushbtn)==HIGH){
  if (NAVIGATION->getState() == DMGC_UTILS::Button::UP)
  {
#if DMGC_SDL_ARDUINO_BUILD
    // Skip intro/outro delays
    DMGC_UTILS::dmgc_intro(magic.getPixels(), true);
#else
    DMGC_UTILS::dmgc_intro(magic.getPixels());
#endif
  }else{
    while(1){      
#if DMGC_SDL_ARDUINO_BUILD
    // So we can get out of this mess if pressed
    buttons.poll(HIGH);
    if (NAVIGATION->isClicked()) exit(0);
    delay(DEBOUNCE_DELAY);
#endif
    };
  }
}
void loop() {
  // Update display
  magic.show();

  // Poll the buttons
  buttons.poll(HIGH);

  // Update current mode
  magic.update();

  delay(DEBOUNCE_DELAY);
}
