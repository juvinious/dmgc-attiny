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

#define NUMPIXELS 8 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, OUT, NEO_GRB + NEO_KHZ800);

volatile uint8_t brightness;
volatile uint8_t color_type;
const uint8_t    COLOR_QTY  = 9;
const uint8_t    DEBOUNCE_DELAY = 50;
const uint8_t    BRIGHTNESS_INC = 5;
const uint8_t    MIN_BRIGHTNESS = 5;
const uint8_t    MAX_BRIGHTNESS = 50;

volatile uint8_t brightness_change_flag = 0;

// cycles through different colors:    red  org  ylw  grn  cyan blue purp wht  off
volatile uint8_t red[COLOR_QTY]   =  { 255, 255, 255, 0,   0,   0,   255, 255, 0};
volatile uint8_t green[COLOR_QTY] =  { 0,   100, 200, 255, 255, 0,   0,   255, 0};
volatile uint8_t blue[COLOR_QTY]  =  { 0,   0,   0,   0,   255, 255, 255, 255, 0};

void setup() {

  // Add buttons
  // Inputs *MUST NOT* use ATTINY's pull-ups. These will rely on the GBC CPU's internal pull-ups to 3.3V. The ATTINY is powered by 5V!!
  NAVIGATION = buttons.add(PCINT3);
  LEFT = buttons.add(PCINT2);
  RIGHT = buttons.add(PCINT1);
  SELECT = buttons.add(PCINT0);

  pixels.begin();

  EEPROM.get(0, brightness);
  EEPROM.get(1, color_type);

  // Check if EPROM data was corrupted, set brightness to max or min values
  if (brightness>MAX_BRIGHTNESS){
    brightness=MAX_BRIGHTNESS;
  }
  if (brightness<MIN_BRIGHTNESS){
    brightness=MIN_BRIGHTNESS;
  }
  
  pixels.setBrightness(brightness);

  // Poll buttons
  buttons.poll(HIGH);

  // Skip intro if nav switch is pushed in
  //if (digitalRead(pushbtn)==HIGH){
  if (NAVIGATION->getState() == DMGC_UTILS::Button::UP)
  {
    DMGC_UTILS::dmgc_intro(pixels, red, green, blue);
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
  outputLED(color_type);

  // Poll the buttons
  buttons.poll(HIGH);

  // Down or hold
  if (NAVIGATION->isDown()){
    if (LEFT->isDown()) {
      brightness_change_flag = 1;
      if(brightness-BRIGHTNESS_INC<MIN_BRIGHTNESS){
        brightness=MIN_BRIGHTNESS;
      }else{
        brightness=brightness-BRIGHTNESS_INC;
      }
      pixels.setBrightness(brightness);
      outputLED(color_type);
    }
    if (RIGHT->isDown()) {
      brightness_change_flag = 1;
      if(brightness+BRIGHTNESS_INC>MAX_BRIGHTNESS){
        brightness=MAX_BRIGHTNESS;
      }else{
        brightness=brightness+BRIGHTNESS_INC;
      }
      pixels.setBrightness(brightness);
      outputLED(color_type);
    }
  }
  // Released
  if (NAVIGATION->isClicked()){
    if (brightness_change_flag != 1)
    {
      color_type += 1;
      if (color_type >= COLOR_QTY-1){
        color_type = 0;
      }
      outputLED(color_type);
    }
    brightness_change_flag = 0;

    // Save brightness and color setting to EPROM
    EEPROM.put(0,brightness);
    EEPROM.put(1,color_type);
  }
  delay(DEBOUNCE_DELAY);
}

void outputLED(uint8_t x){
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(red[x],green[x],blue[x]));
    pixels.show();   // Send the updated pixel colors to the hardware.
  }  
}
