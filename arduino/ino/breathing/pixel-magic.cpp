#include "pixel-magic.h"

// Which pin on the Arduino is connected to the NeoPixels?
#define OUT           PCINT4 // PB4

// Defaults
static const uint8_t BRIGHTNESS_INC = 5;
static const uint8_t MIN_BRIGHTNESS = 5;
static const uint8_t MAX_BRIGHTNESS = 50;

PixelMagic::PixelMagic():
pixels(NUMPIXELS, OUT, NEO_GRB + NEO_KHZ800),
brightness(MAX_BRIGHTNESS / 2), // Start brightness in the middle,
previousBrightness(brightness),
currentMode(MODE::BREATHING)
{
    for (int i = 0; i < NUMPIXELS; i++)
    {
        colors[i].r = colors[i].g = 0;
        colors[i].b = 255;
        colors[i].forward = true;
    }
}

PixelMagic::~PixelMagic(){
}

void PixelMagic::setup()
{
    // Setup pixels
    pixels.begin();
    // Set brightness
    pixels.setBrightness(brightness);
}

void PixelMagic::update()
{
    switch (currentMode)
    {
        case MODE::LEFT_TO_RIGHT:
            updateLeftToRight();
            break;
        case MODE::RIGHT_TO_LEFT:
            updateRightToLeft();
            break;
        case MODE::CENTER_OUT:
            updateCenterOut();
            break;
        case MODE::OUT_TO_CENTER:
            updateOutToCenter();
            break;
        case MODE::BREATHING:
        default:
            updateBreathing();
            break;
    }
}

void PixelMagic::show()
{
    // Update the brightness
    if (brightness != previousBrightness)
    {
        pixels.setBrightness(brightness);

    }

    // Finally Show pixels
    pixels.show();
}

void PixelMagic::updateBreathing(){
    // Since we are only are using a single color we can just use the first index
    PixelColor & color = colors[0];
    // Save the prior brightness in case we need to update
    previousBrightness = brightness;
    if (!color.forward){
        if (brightness > MIN_BRIGHTNESS){
            brightness -= BRIGHTNESS_INC;
        }
        if (brightness <= MIN_BRIGHTNESS){
            color.forward = !color.forward;
            
            // Shift colors
            color.r = random(25, 255);
            color.g = random(25, 255);
            color.b = random(25, 255);
        }
    } else if (color.forward){
        if (brightness < MAX_BRIGHTNESS){
        brightness += BRIGHTNESS_INC;
        }
        if (brightness >= MAX_BRIGHTNESS){
        color.forward = !color.forward;
        }
    }

    // Finally update the pixels
    for(int i=0; i<NUMPIXELS; i++) // For each pixel...
        pixels.setPixelColor(i, color.r, color.g, color.b);
}

void PixelMagic::updateLeftToRight()
{

}
void PixelMagic::updateRightToLeft()
{

}
void PixelMagic::updateCenterOut()
{

}
void PixelMagic::updateOutToCenter()
{

}