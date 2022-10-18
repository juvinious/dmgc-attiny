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
        OFF = 5,
        NUM_MODES = 6
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

    void setCurrentMode(uint8_t mode)
    {
        this->currentMode = static_cast<MODE>(mode);
    }

    uint8_t getCurrentMode() const volatile
    {
        return static_cast<uint8_t>(this->currentMode);
    }

protected:
    void resetColors(uint8_t brightness, bool randomTarget);
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
        uint8_t index;
        int r; 
        int g;
        int b;
        // Multiuse flag (alternating, forward, backward, etc)
        bool flag;
        // Multiuse flag (for more options)
        bool flag2;

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
        
        void inc(int & i1, const int i2, uint8_t increment){
            if (i1 > i2){
                i1 -= increment;
                if (i1 <= i2){
                    i1 = i2;
                }
            }
            else if (i1 < i2){
                i1 += increment;
                if (i1 >= i2){
                    i1 = i2;
                }
            } else {
                i1 = i2;
            }
        }
        void stepTo(const PixelColor & p, uint8_t increment){
            inc(r, p.r, increment);
            inc(g, p.g, increment);
            inc(b, p.b, increment);
        }

        void reset()
        {
            r = g = b = 0;
            flag = flag2 = false;
        }
    };
    
    static const uint8_t NUMPIXELS = 8;
    
    // Basic Colors
    static const PixelColor WHITE;
    static const PixelColor BLACK;
    static const PixelColor RED;
    static const PixelColor GREEN;
    static const PixelColor BLUE;

    Adafruit_NeoPixel pixels;
    PixelColor colors[PixelMagic::NUMPIXELS];
    // PixelColor previousColors[PixelMagic::NUMPIXELS];
    PixelColor targetColor;
    volatile uint8_t brightness;
    volatile uint8_t previousBrightness;
    enum MODE currentMode;

    // Helpers
    bool _update_led(const PixelMagic::PixelColor &, PixelMagic::PixelColor &, uint8_t increment);
    bool _update_two_leds(const PixelMagic::PixelColor &, PixelMagic::PixelColor &, PixelMagic::PixelColor &, uint8_t increment);
};

#endif