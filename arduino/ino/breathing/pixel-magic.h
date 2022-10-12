#ifndef _PIXEL_MAGIC_H
#define _PIXEL_MAGIC_H

#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

class PixelMagic
{
public:
    // POSITIONS
    enum POSITIONS
    {
        UP = 0,
        LEFT = 1,
        DONW = 2,
        RIGHT = 3,
        SELECT = 4,
        START = 5,
        B = 6,
        A = 7,
        TOTAL = 8
    };

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

    void nextMode();

    Adafruit_NeoPixel & getPixels() 
    {
        return this->pixels;
    }

protected:
    void initBreathing();
    void initLeftToRight();
    void initRightToLeft();
    void initCenterOut();
    void initOutToCenter();
    void updateBreathing();
    void updateLeftToRight();
    void updateRightToLeft();
    void updateCenterOut();
    void updateOutToCenter();

private:    
    struct PixelColor
    {
        uint8_t r; 
        uint8_t g;
        uint8_t b;
        // Multiuse flag (alternating, forward, backward, etc)
        bool flag;

        bool operator<(const PixelColor & p){
            return (r < p.r ||
                    g < p.g ||
                    b < p.b);
        }
        bool operator==(const PixelColor & p){
            return (r == p.r && 
                    g == p.g && 
                    b == p.b);
        }
        bool operator!=(const PixelColor & p){
            return (r != p.r ||
                    g != p.g ||
                    b != p.b);
        }
        bool operator>(const PixelColor &p){
            return (r > p.r ||
                    g > p.g ||
                    b > p.b);
        }
        void stepTo(const PixelColor & p, uint8_t increment){
            if (*this > p){
                r = r >= p.r ? p.r : r - increment;
                g = g >= p.g ? p.g : g - increment;
                b = b >= p.b ? p.b : b - increment;
            } else {
                r = r <= p.r ? p.r : r + increment;
                g = g <= p.g ? p.g : g + increment;
                b = b <= p.b ? p.b : b + increment;
            }
        }
    };
    static const uint8_t NUMPIXELS = 8;

    Adafruit_NeoPixel pixels;
    PixelColor colors[PixelMagic::NUMPIXELS];
    // PixelColor previousColors[PixelMagic::NUMPIXELS];
    PixelColor targetColor;
    volatile uint8_t brightness;
    volatile uint8_t previousBrightness;
    enum MODE currentMode;
};

#endif