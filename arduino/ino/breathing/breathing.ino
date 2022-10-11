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

  buttons.poll(HIGH);

  // Skip intro if nav switch is pushed in
  //if (digitalRead(pushbtn)==HIGH){
  if (NAVIGATION->getState() == DMGC_UTILS::Button::UP)
  {
    DMGC_UTILS::dmgc_intro(pixels, red, green, blue);
  }else{
    while(1){};
  }
//  while(!digitalRead(pushbtn)){
//    delay(10);
//  }
}
void loop() {
  outputLED(color_type);

  // Poll the buttons
  buttons.poll(HIGH);

  delay(DEBOUNCE_DELAY);

}

void outputLED(uint8_t x){
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(red[x],green[x],blue[x]));
    pixels.show();   // Send the updated pixel colors to the hardware.
  }  
}
