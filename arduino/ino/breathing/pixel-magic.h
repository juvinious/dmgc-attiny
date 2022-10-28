#ifndef _PIXEL_MAGIC_H
#define _PIXEL_MAGIC_H

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#ifdef DMGC_SDL_ARDUINO_BUILD
#include "dmgc-utils.h"
#else
#include "src/dmgc-utils/dmgc-utils.h"
#endif

class PixelColor
{
public:
    enum STATE
    {
        BEGIN,
        FORWARD,
        BACKWARD,
        REACHED_TARGET,
        COMPLETE,
        HALF,
        MAX,
        WAITING,
        NOT_IN_USE,
        NO_CHANGE,
    };

    // Basic Colors
    static const PixelColor WHITE;
    static const PixelColor BLACK;
    static const PixelColor RED;
    static const PixelColor GREEN;
    static const PixelColor BLUE;

    PixelColor();
    PixelColor(uint8_t index, int r, int g, int b, const PixelColor::STATE &);
    PixelColor(const PixelColor &);
    ~PixelColor();

    bool operator<(const PixelColor & p);
    bool operator==(const PixelColor & p);
    bool operator!=(const PixelColor & p);
    bool operator>(const PixelColor &p);

    void stepTo(const PixelColor & p, uint8_t increment);
    void reset();
    // Randomize Colors
    void random();

    void setColorAs(const PixelColor &);
    void setNextColor(const PixelColor &);

    void update(Adafruit_NeoPixel &,uint8_t increment);

    bool isNearTargetColorBy(int amount);

    uint8_t index;
    int r; 
    int g;
    int b;
    // Pixel state forgo boolean flags from before
    enum STATE state;

private:
    const PixelColor * nextColor;
};

class PixelMagic
{
public:
    // POSITIONS
    enum POSITIONS
    {
        UP = 0,
        LEFT = 1,
        DOWN = 2,
        RIGHT = 3,
        SELECT = 4,
        START = 5,
        B = 6,
        A = 7,
        TOTAL = 8
    };

    enum MODE
    {
        BREATHING = 0,
        LEFT_TO_RIGHT = 1,
        RIGHT_TO_LEFT = 2,
        CENTER_OUT = 3,
        OUT_TO_CENTER = 4,
        RANDOM = 5,
        OFF = 6,
        NUM_MODES = 7,
        SOLID = 50,
        CONFIGURATION = 100,
        NOT_DEFINED = 200,
    };

    PixelMagic();
    ~PixelMagic();

    void setup();

    void update();

    void show();

    void nextMode();

    void setMode(uint8_t mode);

    void increaseIncrement();
    void decreaseIncrement();
    void setIncrementSpeed(uint8_t speed);
    void toggleRandomColors();
    void toggleSolidColors();
    void toggleConfiguration();

    Adafruit_NeoPixel & getPixels() 
    {
        return this->pixels;
    }

    const MODE & getMode() const 
    {
        return this->currentMode;
    }

    const uint8_t getCurrentMode() const volatile
    {
        return static_cast<uint8_t>(this->currentMode);
    }

    const uint8_t getIncrementSpeed() const volatile
    {
        return incrementSpeed;
    }

    const bool isInConfiguration() const 
    {
        return this->currentMode == CONFIGURATION;
    }

protected:
    void resetColors(const PixelColor::STATE & state, uint8_t brightness);
    void initBreathing();
    void initLeftToRight();
    void initRightToLeft();
    void initCenterOut();
    void initOutToCenter();
    void initRandom();
    void updateBreathing();
    void updateLeftToRight();
    void updateRightToLeft();
    void updateCenterOut();
    void updateOutToCenter();
    void updateRandom();

private:

    static const uint8_t NUMPIXELS = 8;

    Adafruit_NeoPixel pixels;
    PixelColor colors[PixelMagic::NUMPIXELS];
    int mixedOrder[PixelMagic::NUMPIXELS];
    PixelColor targetColor;
    uint8_t incrementSpeed;
    volatile uint8_t brightness;
    volatile uint8_t previousBrightness;
    volatile uint8_t randomColors;
    enum MODE previousMode;
    enum MODE currentMode;

    // Helpers
    void _randomize_colors(PixelColor &);
};

#endif