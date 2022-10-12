#include "pixel-magic.h"

// Which pin on the Arduino is connected to the NeoPixels?
#define OUT           PCINT4 // PB4

// Defaults
static const uint8_t BRIGHTNESS_INC = 5;
static const uint8_t MIN_BRIGHTNESS = 5;
static const uint8_t MAX_BRIGHTNESS = 50;
static const uint8_t MIN_COLOR = 25;
static const uint8_t MAX_COLOR = 255;
static const uint8_t COLOR_INC = 20;

uint8_t randomColor()
{
    return random(MIN_COLOR, MAX_COLOR);
}

PixelMagic::PixelMagic():
pixels(NUMPIXELS, OUT, NEO_GRB + NEO_KHZ800),
brightness(MAX_BRIGHTNESS / 2), // Start brightness in the middle,
previousBrightness(brightness),
currentMode(MODE::BREATHING)
{  
    initBreathing();
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

void PixelMagic::nextMode()
{
    // Cycle forward through the modes
    this->currentMode = static_cast<MODE>((static_cast<int>(currentMode) + 1) % NUM_MODES);

    switch (currentMode)
    {
        case MODE::LEFT_TO_RIGHT:
            initLeftToRight();
            break;
        case MODE::RIGHT_TO_LEFT:
            initRightToLeft();
            break;
        case MODE::CENTER_OUT:
            initCenterOut();
            break;
        case MODE::OUT_TO_CENTER:
            initOutToCenter();
            break;
        case MODE::BREATHING:
        default:
            initBreathing();
            break;
    }
}

void PixelMagic::initBreathing()
{
    previousBrightness = brightness = MAX_BRIGHTNESS / 2;
    targetColor.r = targetColor.g = 0;
    targetColor.b = MAX_COLOR;
    targetColor.flag = true;
}

void PixelMagic::initLeftToRight()
{
    // Brightness at the bottom Increments in 5 to 50
    previousBrightness = brightness = 10;
    // Color to move to
    targetColor.r = randomColor();
    targetColor.g = randomColor();
    targetColor.b = randomColor();
    // All colors reached target?
    targetColor.flag = false;

    for (int i = 0; i < NUMPIXELS; i++){
        // All pixels off
        colors[i].r = colors[i].g = colors[i].b = 0;
        // Target reached?
        colors[i].flag = false;
    }
}

void PixelMagic::initRightToLeft()
{

}

void PixelMagic::initCenterOut()
{

}

void PixelMagic::initOutToCenter()
{

}

void PixelMagic::updateBreathing(){
    // Save the prior brightness in case we need to update
    previousBrightness = brightness;
    // Dim
    if (!targetColor.flag){
        if (brightness > MIN_BRIGHTNESS){
            brightness -= BRIGHTNESS_INC;
        }
        if (brightness <= MIN_BRIGHTNESS){
            targetColor.flag = !targetColor.flag;
            
            // Shift colors
            targetColor.r = randomColor();
            targetColor.g = randomColor();
            targetColor.b = randomColor();
        }
    } else if (targetColor.flag) // Increase
    {
        if (brightness < MAX_BRIGHTNESS){
        brightness += BRIGHTNESS_INC;
        }
        if (brightness >= MAX_BRIGHTNESS){
        targetColor.flag = !targetColor.flag;
        }
    }

    // Finally update the pixels
    for(int i=0; i<NUMPIXELS; i++) // For each pixel...
        pixels.setPixelColor(i, targetColor.r, targetColor.g, targetColor.b);
}

void PixelMagic::updateLeftToRight()
{
    if (targetColor.flag)
    {
        // Do fade then reset
        brightness = brightness > MIN_BRIGHTNESS ? brightness - BRIGHTNESS_INC : MIN_BRIGHTNESS;
        if (brightness == MIN_BRIGHTNESS){
            initLeftToRight();
        }
    } else {
        for (int i = 0; i < NUMPIXELS; i++)
        {
            // Step along all colors until they arrive at the target
            if (!colors[i].flag)git
            {
                colors[i].stepTo(targetColor, COLOR_INC);
                pixels.setPixelColor(i, colors[i].r, colors[i].g, colors[i].b);

                if (colors[i] == targetColor)
                {
                    colors[i].flag = true;
                    // Step brightness up
                    previousBrightness = brightness;
                    brightness += BRIGHTNESS_INC;
                }
                break;
            }
            // End goal
            if (i == NUMPIXELS-1 && colors[i].flag){
                targetColor.flag = true;
            }
        }
    }
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