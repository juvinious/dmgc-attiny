#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#include <iostream>
#include <map>

class Button 
{
public:
  enum BUTTON_TYPE
  {
    NAVIGATION,
    SELECT,
    LEFT,
    RIGHT
  };
  enum STATE
  {
    UP,
    DOWN,
    HOLD,
    PRESS,
    RELEASE
  };

  Button(enum BUTTON_TYPE type, int pin):
  pin(pin),
  type(type),
  state(UP)
  {
    // Inputs *MUST NOT* use ATTINY's pull-ups. These will rely on the GBC CPU's internal pull-ups to 3.3V. The ATTINY is powered by 5V!!
    pinMode(pin, INPUT);
  }
  ~Button()
  {

  }

  enum STATE getState() const {
    return state;
  }

  void poll(int pull)
  {
    switch (digitalRead(pin)){
      case HIGH:
        if (pull == LOW)
        {
          if (state == UP){
            state = DOWN;
          } else if (state == DOWN){
            state = HOLD;
          }
        } else if (pull == HIGH)
        {
          if (state == DOWN){
            state = PRESS;
          } else if (state == HOLD){
            state = RELEASE;
          } else {
            state = UP;
          }
        }
        break;
      case LOW:
        if (pull == HIGH)
        {
          if (state == UP){
            state = DOWN;
          } else if (state == DOWN){
            state = HOLD;
          }
        } else if (pull == LOW)
        {
          if (state == DOWN){
            state = PRESS;
          } else if (state == HOLD){
            state = RELEASE;
          } else {
            state = UP;
          }
        }
        break;
      default:
        state = UP;
        break;
    }
  }

private: 
  int pin;
  enum BUTTON_TYPE type;
  enum STATE state;
};

// Which pin on the Arduino is connected to the NeoPixels?
#define OUT           PCINT4 // PB4

/* Button mapper and poller */
std::map<Button::BUTTON_TYPE, Button *> buttons;

void pollInput() {
  for (std::map<Button::BUTTON_TYPE, Button *>::iterator i = buttons.begin(); i != buttons.end(); i++){
    (*i->second).poll(HIGH);
  }
}

// Define order of LEDs in string, starting with 0
#define l             1
#define u             0
#define d             2
#define r             3
#define se            4
#define st            5
#define b             6
#define a             7

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
  buttons[Button::NAVIGATION] = new Button(Button::NAVIGATION, PCINT3);
  buttons[Button::LEFT] = new Button(Button::LEFT, PCINT2);
  buttons[Button::RIGHT] = new Button(Button::RIGHT, PCINT1);
  buttons[Button::SELECT] = new Button(Button::SELECT, PCINT0);

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

  // Inputs *MUST NOT* use ATTINY's pull-ups. These will rely on the GBC CPU's internal pull-ups to 3.3V. The ATTINY is powered by 5V!!
  /*
  pinMode(pushbtn, INPUT);
  pinMode(leftbtn, INPUT);
  pinMode(rightbtn, INPUT);
  pinMode(selbtn, INPUT);
  */

  pollInput();

  // Skip intro if nav switch is pushed in
  //if (digitalRead(pushbtn)==HIGH){
  if (buttons[Button::NAVIGATION]->getState() == Button::UP)
  {

      pixels.clear();
      pixels.show();
      delay(1100);

      int y=6;
      int k=0;

      uint8_t introarray[24] = {8,8,8,8,8,8,8,2,2,0,0,6,6,3,3,5,5,5,5,5,5,5,5,5};

      // Loop through values in introarray. Delays were timed experimentally.
      for(int j=0; j<16; j++){
        for(int i=0; i<NUMPIXELS; i++){
          if(i==l){
            k=introarray[j+6];
          }
          if(i==u){
            k=introarray[j+5];
          }
          if(i==d){
            k=introarray[j+5];
          }
          if(i==r){
            k=introarray[j+4];
          }
          if(i==se){
            k=introarray[j+3];
          }
          if(i==st){
            k=introarray[j+2];
          }
          if(i==b){
            k=introarray[j+1];
          }
          if(i==a){
            k=introarray[j];
          }
        pixels.setPixelColor(i, pixels.Color(red[k],green[k],blue[k]));
        pixels.show();
        delay(y);
        }
      }
      delay(2000);
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
  pollInput();

  // Down or hold
  if (buttons[Button::NAVIGATION]->getState() == Button::DOWN || buttons[Button::NAVIGATION]->getState() == Button::HOLD){
    // std::cout << "Got down " << std::endl;
    if (buttons[Button::LEFT]->getState() == Button::DOWN || buttons[Button::LEFT]->getState() == Button::HOLD) {
      // std::cout << "Got LEFT " << std::endl;
      brightness_change_flag = 1;
      if(brightness-BRIGHTNESS_INC<MIN_BRIGHTNESS){
        brightness=MIN_BRIGHTNESS;
      }else{
        brightness=brightness-BRIGHTNESS_INC;
      }
      pixels.setBrightness(brightness);
      outputLED(color_type);
    }
    if (buttons[Button::RIGHT]->getState() == Button::DOWN || buttons[Button::RIGHT]->getState() == Button::HOLD) {
      // std::cout << "Got RIGHT " << std::endl;
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
  if (buttons[Button::NAVIGATION]->getState() == Button::PRESS || buttons[Button::NAVIGATION]->getState() == Button::RELEASE){
    if (brightness_change_flag != 1)
    {
      // std::cout << "Press and release... " << std::endl;
      color_type += 1;
      if (color_type >= COLOR_QTY-1){
        color_type = 0;
      }
      outputLED(color_type);
    }
    
    brightness_change_flag = 0;
  }

  delay(DEBOUNCE_DELAY);

}

void outputLED(uint8_t x){
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(red[x],green[x],blue[x]));
    pixels.show();   // Send the updated pixel colors to the hardware.
  }  
}
