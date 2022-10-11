#include "dmgc-utils.h"

#ifdef DMGC_SDL_ARDUINO_BUILD
#include "arduino_defs.h"
#else
#include "Arduino.h"
#endif


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

ButtonHandler::ButtonHandler(int total):
totalPins(total),
buttonList(NULL)
{
    
}

ButtonHandler::~ButtonHandler()
{
    if (buttonList){
        Button * current = buttonList;
        while (current)
        {
            Button * next = current->next;
            delete current;
            current = next;
        }
    }
}

Button * ButtonHandler::add(int pin)
{
    Button * newButton = new Button(pin);
    if (!buttonList)
    {
        buttonList = newButton;
    } else {
        Button * current = buttonList;
        while (current)
        {
            if (current->next == NULL){
                current->next = newButton;
                break;
            }
            current = current->next;
        }
    }

    return newButton;
}

void ButtonHandler::poll(int pull)
{
    Button * current = buttonList;
    while (current)
    {
        Button * next = current->next;
        current->poll(pull);
        current = next;
    }
}