#include "dmgc-utils.h"
#include <Adafruit_NeoPixel.h>

#ifdef DMGC_SDL_ARDUINO_BUILD
#include "arduino_defs.h"
#else
#include "Arduino.h"
#endif

using namespace DMGC_UTILS;

Button::Button(int pin):
pin(pin),
state(UP)
{
    pinMode(pin, INPUT);
}
Button::~Button()
{

}

void Button::poll(int pull)
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

ButtonHandler::ButtonHandler():
totalPins(0)
{
    
}

ButtonHandler::~ButtonHandler()
{
    for (List<Button *>::iterator i = buttonList.begin(); i != buttonList.end(); i++){
        Button * button = *i;
        if (button != NULL)
        {
            delete button;
        }
    }
}

Button * ButtonHandler::add(int pin)
{
    Button * newButton = new Button(pin);
    
    buttonList.push(newButton);

    totalPins++;

    return newButton;
}

void ButtonHandler::poll(int pull)
{
   for (List<Button *>::iterator i = buttonList.begin(); i != buttonList.end(); i++){
        Button * button = *i;
        button->poll(pull);
   }
}

void DMGC_UTILS::dmgc_intro(Adafruit_NeoPixel & pixels, bool skipDelays) {
    
    // default colors:                     red  org  ylw  grn  cyan blue purp wht  off
    const uint8_t COLOR_QTY = 9;
    volatile uint8_t red[COLOR_QTY]   =  { 255, 255, 255, 0,   0,   0,   255, 255, 0};
    volatile uint8_t green[COLOR_QTY] =  { 0,   100, 200, 255, 255, 0,   0,   255, 0};
    volatile uint8_t blue[COLOR_QTY]  =  { 0,   0,   0,   0,   255, 255, 255, 255, 0};

    // Define order of LEDs in string, starting with 0
    const uint8_t l = 1;
    const uint8_t u = 0;
    const uint8_t d = 2;
    const uint8_t r = 3;
    const uint8_t se = 4;
    const uint8_t st = 5;
    const uint8_t b = 6;
    const uint8_t a = 7;

    pixels.clear();
    pixels.show();
    if (!skipDelays)
        delay(1100);

    int y=6;
    int k=0;

    uint8_t introarray[24] = {8,8,8,8,8,8,8,2,2,0,0,6,6,3,3,5,5,5,5,5,5,5,5,5};

    // Loop through values in introarray. Delays were timed experimentally.
    for(int j=0; j<16; j++){
        for(int i=0; i<pixels.numPixels(); i++)
        {
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

    if (!skipDelays)
        delay(2000);
}