#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#ifdef DMGC_SDL_ARDUINO_BUILD
#include "dmgc-utils.h"
  // Forward declarations
  void shiftColors();
  void outputLED();
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
const uint8_t    DEBOUNCE_DELAY = 170;
const uint8_t    BRIGHTNESS_INC = 5;
const uint8_t    MIN_BRIGHTNESS = 5;
const uint8_t    MAX_BRIGHTNESS = 50;

volatile bool brightness_change_flag = false;
volatile uint8_t red = 128, green = 128, blue = 128;

void setup() {

  // Add buttons
  NAVIGATION = buttons.add(PCINT3);
  LEFT = buttons.add(PCINT2);
  RIGHT = buttons.add(PCINT1);
  SELECT = buttons.add(PCINT0);

  pixels.begin();

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
}
void loop() {
  outputLED();

  // Poll the buttons
  buttons.poll(HIGH);

  if (!brightness_change_flag){
    if (brightness > MIN_BRIGHTNESS){
      brightness -= BRIGHTNESS_INC;
    }
    if (brightness <= MIN_BRIGHTNESS){
      brightness_change_flag = !brightness_change_flag;
      shiftColors();
    }
  } else if (brightness_change_flag){
    if (brightness < MAX_BRIGHTNESS){
      brightness += BRIGHTNESS_INC;
    }
    if (brightness >= MAX_BRIGHTNESS){
      brightness_change_flag = !brightness_change_flag;
    }
  }
  delay(DEBOUNCE_DELAY);
}

void outputLED(){
  pixels.setBrightness(brightness);
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    //pixels.setPixelColor(i, pixels.Color(red[x],green[x],blue[x]));
    pixels.setPixelColor(i, red, green, blue);
  }  
  pixels.show();   // Send the updated pixel colors to the hardware.
}

void shiftColors()
{
  red = random(25, 255);
  green = random(25, 255);
  blue = random(25, 255);
  // outputLED();
  // printf("Red: %d, Green: %d, Blue: %d\n", red, green, blue);
}