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

// Settings changed
bool settingsChanged = false;

void setup() {

  // Setup initial items
  magic.setup();

  // Get previous saved mode if exists otherwise start at 0
  volatile uint8_t mode = 0, increment = 0;

  EEPROM.get(0, mode);
  EEPROM.get(1, increment);

  magic.setMode(mode);
  magic.setIncrementSpeed(increment);
  

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

void loop()
{
  // Update current mode
  magic.update();

  // Poll the buttons
  buttons.poll(HIGH);

  if (NAVIGATION->isDown()){
     if (LEFT->isClicked()) {
      magic.decreaseIncrement();
      settingsChanged = true;
    }
    if (RIGHT->isClicked()) {
      magic.increaseIncrement();
      settingsChanged = true;
    }  
    if (SELECT->isClicked()){
      magic.toggleSolidColors();
      settingsChanged = true;
    }
  } 
  if (NAVIGATION->isClicked() && !settingsChanged)
  {
    magic.nextMode();
    settingsChanged = true;
  }

  // Update eeprom only when we release the navigation button 
  // so we don't move to the next mode if changing increment
  if (settingsChanged && !NAVIGATION->isDown())
  {
    volatile uint8_t mode = magic.getCurrentMode();
    volatile uint8_t increment = magic.getIncrementSpeed();
    EEPROM.put(0, mode);
    EEPROM.put(1, increment);
    settingsChanged = false;
  }
  
  // Update display
  magic.show();

  delay(DEBOUNCE_DELAY);
}
