#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#ifdef DMGC_SDL_ARDUINO_BUILD
#include "dmgc-utils.h"
#else
#include "src/dmgc-utils/dmgc-utils.h"
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define OUT           PCINT4 // PB4

/* Button mapper and poller */
DMGC_UTILS::ButtonHandler buttons;
DMGC_UTILS::Button * NAVIGATION = NULL, 
       * LEFT = NULL,
       * RIGHT = NULL,
       * SELECT = NULL;


const uint8_t NUMPIXELS = 8;

Adafruit_NeoPixel pixels(NUMPIXELS, OUT, NEO_GRB + NEO_KHZ800);

volatile uint8_t brightness;
volatile uint8_t color_type;
const uint8_t    COLOR_QTY  = 9;
const uint8_t    DEBOUNCE_DELAY = 150;
const uint8_t    BRIGHTNESS_INC = 5;
const uint8_t    MIN_BRIGHTNESS = 5;
const uint8_t    MAX_BRIGHTNESS = 50;

volatile uint8_t brightness_change_flag = 0;

volatile uint8_t red = 128, green = 128, blue = 128;

void setup() {

  // Add buttons
  NAVIGATION = buttons.add(PCINT3);
  LEFT = buttons.add(PCINT2);
  RIGHT = buttons.add(PCINT1);
  SELECT = buttons.add(PCINT0);

  pixels.begin();

  EEPROM.get(0, brightness);
  EEPROM.get(1, color_type);

  // Check if EPROM data was corrupted, set brightness to max or min values
  /*if (brightness>MAX_BRIGHTNESS){
    brightness=MAX_BRIGHTNESS;
  }
  if (brightness<MIN_BRIGHTNESS){
    brightness=MIN_BRIGHTNESS;
  }*/

  // Start brightness in the middle
  brightness = MAX_BRIGHTNESS / 2;
  
  pixels.setBrightness(brightness);

  buttons.poll(HIGH);

  // Skip intro if nav switch is pushed in
  //if (digitalRead(pushbtn)==HIGH){
  if (NAVIGATION->getState() == DMGC_UTILS::Button::UP)
  {
#if DMGC_SDL_ARDUINO_BUILD
    // Skip intro/outro delays
    DMGC_UTILS::dmgc_intro(pixels, true);
#else
    DMGC_UTILS::dmgc_intro(pixels);
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
//  while(!digitalRead(pushbtn)){
//    delay(10);
//  }
}
void loop() {
  outputLED();

  // Poll the buttons
  buttons.poll(HIGH);

  if (brightness_change_flag == 0){
    if (brightness > MIN_BRIGHTNESS){
      brightness -= BRIGHTNESS_INC;
    }
    if (brightness <= MIN_BRIGHTNESS){
      brightness_change_flag = 1;
    }
  } else if (brightness_change_flag){
    if (brightness < MAX_BRIGHTNESS){
      brightness += BRIGHTNESS_INC;
    }
    if (brightness >= MAX_BRIGHTNESS){
      brightness_change_flag = 0;
    }
  }
  delay(DEBOUNCE_DELAY);
}

void outputLED(){
  pixels.setBrightness(brightness);
  pixels.show();
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    //pixels.setPixelColor(i, pixels.Color(red[x],green[x],blue[x]));
    pixels.setPixelColor(i, red, green, blue);
    pixels.show();   // Send the updated pixel colors to the hardware.
  }  
}
