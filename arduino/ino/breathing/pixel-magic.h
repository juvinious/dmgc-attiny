#ifndef _PIXEL_MAGIC_H
#define _PIXEL_MAGIC_H

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

struct PixelColor
{
    uint8_t r; 
    uint8_t g;
    uint8_t b;
    bool forward;
};

class PixelMagic
{
public:
    enum MODE
    {
        BREATHING,
        LEFT_TO_RIGHT,
        RIGHT_TO_LEFT,
        CENTER_OUT,
        OUT_TO_CENTER,
        NUM_MODES
    };

    PixelMagic();
    ~PixelMagic();

    void setup();

    void update();

    void show();

    void nextMode()
    {
        // Cycle forward through the modes
        this->currentMode = static_cast<MODE>((static_cast<int>(currentMode) + 1) % NUM_MODES);
    }

    Adafruit_NeoPixel & getPixels() 
    {
        return this->pixels;
    }

protected:
    void updateBreathing();
    void updateLeftToRight();
    void updateRightToLeft();
    void updateCenterOut();
    void updateOutToCenter();

private:
    static const uint8_t NUMPIXELS = 8;

    Adafruit_NeoPixel pixels;
    PixelColor colors[PixelMagic::NUMPIXELS];
    volatile uint8_t brightness;
    volatile uint8_t previousBrightness;
    enum MODE currentMode;
};

#endif