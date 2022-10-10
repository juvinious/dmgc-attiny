#ifndef _DMGC_UTILS_H
#define _DMGC_UTILS_H

/*
enum BUTTON_TYPE
{
NAVIGATION,
SELECT,
LEFT,
RIGHT
};
*/

class Button 
{
public:
    enum STATE
    {
        UP,
        DOWN,
        HOLD,
        PRESS,
        RELEASE
    };
    
    Button(int pin);
    ~Button();

    enum STATE getState() const {
        return state;
    }

    bool isDown() const {
        return state == DOWN || state == HOLD;
    }

    bool isClicked() const {
        return state == PRESS || state == RELEASE;
    }

    void poll(int pull);

private: 
    int pin;
    enum STATE state;

    // linked list
    Button * next;
    friend class ButtonHandler;
};

class ButtonHandler
{
public:
    ButtonHandler(int total);
    ~ButtonHandler();

    Button * add(int pin);

    void poll(int pull);

private:
    int totalPins;
    Button * buttonList;
};

#endif