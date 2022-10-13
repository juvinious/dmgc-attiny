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

uint8_t randomColor(int min=MIN_COLOR)
{
    return random(min, MAX_COLOR);
}

PixelMagic::PixelMagic():
pixels(NUMPIXELS, OUT, NEO_GRB + NEO_KHZ800),
brightness(MAX_BRIGHTNESS / 2), // Start brightness in the middle,
previousBrightness(brightness),
currentMode(MODE::BREATHING)
{
    for (int i = 0; i < NUMPIXELS; i++){
        // Set each index
        colors[i].index = i;
    }
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
            updateBreathing();
            break;
        case MODE::OFF:
        default:
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
            initBreathing();
            break;
        case MODE::OFF:
        default:
            resetColors(MIN_BRIGHTNESS, false);
            break;
    }
}

void PixelMagic::resetColors(uint8_t brightness, bool randomTarget)
{
    // Brightness at the bottom Increments in 5 to 50
    this->previousBrightness = this->brightness = brightness;
    if (randomTarget)
    {
        // Color to move to
        targetColor.r = randomColor(0);
        targetColor.g = randomColor(0);
        targetColor.b = randomColor(0);
        // All colors reached target?
        targetColor.flag = false;
    } else {
        targetColor.reset();
    }

    for (int i = 0; i < NUMPIXELS; i++){
        // All pixels off and reset flag
        colors[i].reset();
        pixels.setPixelColor(i, colors[i].r, colors[i].g, colors[i].b);
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
    resetColors(10, true);
}

void PixelMagic::initRightToLeft()
{
    // Brightness at the bottom Increments in 5 to 50
    resetColors(10, true);
}

void PixelMagic::initCenterOut()
{
    // Brightness at the bottom Increments in 5 to 50
    previousBrightness = brightness = 10;
    resetColors(10, true);
}

void PixelMagic::initOutToCenter()
{
    // Brightness at the bottom Increments in 5 to 50
    resetColors(10, true);
}

void PixelMagic::updateBreathing(){
    // Save the prior brightness in case we need to update
    previousBrightness = brightness;
    // Dim
    if (!targetColor.flag)
    {
        brightness = brightness > MIN_BRIGHTNESS ? brightness - BRIGHTNESS_INC : MIN_BRIGHTNESS;
        if (brightness == MIN_BRIGHTNESS)
        {
            targetColor.flag = !targetColor.flag;
            
            // Shift colors
            targetColor.r = randomColor();
            targetColor.g = randomColor();
            targetColor.b = randomColor();
        }
    } else if (targetColor.flag) // Increase
    {
        brightness = brightness < MAX_BRIGHTNESS ? brightness + BRIGHTNESS_INC : MAX_BRIGHTNESS;
        if (brightness == MAX_BRIGHTNESS)
        {
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
            if (!colors[i].flag)
            {
                if (_update_led(targetColor, colors[i], COLOR_INC))
                {
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
    if (targetColor.flag)
    {
        // Do fade then reset
        brightness = brightness > MIN_BRIGHTNESS ? brightness - BRIGHTNESS_INC : MIN_BRIGHTNESS;
        if (brightness == MIN_BRIGHTNESS){
            initRightToLeft();
        }
    } else {
        for (int i = (NUMPIXELS-1); i >= 0; i--)
        {
            // Step along all colors until they arrive at the target
            if (!colors[i].flag)
            {
                if (_update_led(targetColor, colors[i], COLOR_INC))
                {
                    // Step brightness up
                    previousBrightness = brightness;
                    brightness += BRIGHTNESS_INC;
                }
                break;
            }
            // End goal
            if (i == 0 && colors[i].flag){
                targetColor.flag = true;
            }
        }
    }
}

void PixelMagic::updateCenterOut()
{
    if (targetColor.flag)
    {
        // Do fade then reset
        brightness = brightness > MIN_BRIGHTNESS ? brightness - BRIGHTNESS_INC : MIN_BRIGHTNESS;
        if (brightness == MIN_BRIGHTNESS){
            initCenterOut();
        }
    } else {
        if (!colors[RIGHT].flag && !colors[SELECT].flag)
        {
            if (_update_two_leds(targetColor, colors[RIGHT], colors[SELECT], COLOR_INC/3))
            {
                // Step brightness up
                previousBrightness = brightness;
                brightness += BRIGHTNESS_INC*2;
            }
        } else if (!colors[DOWN].flag && !colors[START].flag)
        {
            if (_update_two_leds(targetColor, colors[DOWN], colors[START], COLOR_INC/3))
            {
                // Step brightness up
                previousBrightness = brightness;
                brightness += BRIGHTNESS_INC*2;
            }
        } else if (!colors[LEFT].flag && !colors[B].flag)
        {
            if (_update_two_leds(targetColor, colors[LEFT], colors[B], COLOR_INC/3))
            {
                // Step brightness up
                previousBrightness = brightness;
                brightness += BRIGHTNESS_INC*2;
            }
        } else if (!colors[UP].flag && !colors[A].flag)
        {
            if (_update_two_leds(targetColor, colors[UP], colors[A], COLOR_INC/3))
            {
                // Step brightness up
                previousBrightness = brightness;
                brightness += BRIGHTNESS_INC*2;
            }
        } else {
            // End
            targetColor.flag = true;
        }
    }
}
void PixelMagic::updateOutToCenter()
{
    if (targetColor.flag)
    {
        // Do fade then reset
        brightness = brightness > MIN_BRIGHTNESS ? brightness - BRIGHTNESS_INC : MIN_BRIGHTNESS;
        if (brightness == MIN_BRIGHTNESS){
            initOutToCenter();
        }
    } else {
        if (!colors[UP].flag && !colors[A].flag)
        {
            if (_update_two_leds(targetColor, colors[UP], colors[A], COLOR_INC/3))
            {
                // Step brightness up
                previousBrightness = brightness;
                brightness += BRIGHTNESS_INC*2;
            }
        } else if (!colors[LEFT].flag && !colors[B].flag)
        {
            if (_update_two_leds(targetColor, colors[LEFT], colors[B], COLOR_INC/3))
            {
                // Step brightness up
                previousBrightness = brightness;
                brightness += BRIGHTNESS_INC*2;
            }
        } else if (!colors[DOWN].flag && !colors[START].flag)
        {
            if (_update_two_leds(targetColor, colors[DOWN], colors[START], COLOR_INC/3))
            {
                // Step brightness up
                previousBrightness = brightness;
                brightness += BRIGHTNESS_INC*2;
            }
        } else if (!colors[RIGHT].flag && !colors[SELECT].flag)
        {
            if (_update_two_leds(targetColor, colors[RIGHT], colors[SELECT], COLOR_INC/3))
            {
                // Step brightness up
                previousBrightness = brightness;
                brightness += BRIGHTNESS_INC*2;
            }
        } else {
            // End
            targetColor.flag = true;
        }
    }
}

// Update one led
bool PixelMagic::_update_led(PixelMagic::PixelColor & targetColor, PixelMagic::PixelColor & c1, uint8_t increment)
{
    if (!c1.flag){
        c1.stepTo(targetColor, increment);

        pixels.setPixelColor(c1.index, c1.r, c1.g, c1.b);
        
        if (c1 == targetColor)
        {
            c1.flag = true;
            return true;
        }
    }

    // Not finished
    return false;
}

// Update two leds at once
bool PixelMagic::_update_two_leds(PixelMagic::PixelColor & targetColor, PixelMagic::PixelColor & c1, PixelMagic::PixelColor & c2, uint8_t increment)
{
    return _update_led(targetColor, c1, increment) && _update_led(targetColor, c2, increment);
}