#include "pixel-magic.h"

// Which pin on the Arduino is connected to the NeoPixels?
#define OUT           PCINT4 // PB4

// Defaults
static const uint8_t BRIGHTNESS_INC = 5;
static const uint8_t MIN_BRIGHTNESS = 5;
static const uint8_t MAX_BRIGHTNESS = 50;
static const uint8_t MIN_COLOR = 25;
static const uint8_t MAX_COLOR = 255;
static const uint8_t COLOR_INC = 15;
static const uint8_t MAX_COLOR_INC = 125;
static const uint8_t MIN_COLOR_INC = 25;

// STATIC COLORS
const PixelColor PixelMagic::WHITE = PixelColor(0,255,255,255,PixelColor::NOT_IN_USE);
const PixelColor PixelMagic::BLACK = PixelColor(0,0,0,0,PixelColor::NOT_IN_USE);
const PixelColor PixelMagic::RED = PixelColor(0,255,0,0,PixelColor::NOT_IN_USE);
const PixelColor PixelMagic::GREEN = PixelColor(0,0,255,0,PixelColor::NOT_IN_USE);
const PixelColor PixelMagic::BLUE = PixelColor(0,0,0,255,PixelColor::NOT_IN_USE);

// HELPERS
uint8_t _random_color(int min=MIN_COLOR)
{
    return random(min, MAX_COLOR);
}

void _increment(int & i1, const int i2, uint8_t increment)
{
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

PixelColor::PixelColor():
index(0),
r(0),
g(0),
b(0),
state(PixelColor::BEGIN),
nextColor(NULL)
{

}

PixelColor::PixelColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b, const PixelColor::STATE & state):
index(index),
r(r),
g(g),
b(b),
state(state),
nextColor(NULL)
{

}

PixelColor::PixelColor(const PixelColor & copy):
index(copy.index),
r(copy.r),
g(copy.g),
b(copy.b),
state(copy.state),
nextColor(copy.nextColor)
{

}

PixelColor::~PixelColor()
{

}

bool PixelColor::operator<(const PixelColor & p)
{
    return (r < p.r ||
            g < p.g ||
            b < p.b);
}

bool PixelColor::operator==(const PixelColor & p)
{
    return (r == p.r && 
            g == p.g && 
            b == p.b);
}

bool PixelColor::operator!=(const PixelColor & p)
{
    return (r != p.r ||
            g != p.g ||
            b != p.b);
}

bool PixelColor::operator>(const PixelColor &p)
{
    return (r > p.r ||
            g > p.g ||
            b > p.b);
}

void PixelColor::stepTo(const PixelColor & p, uint8_t increment)
{
    _increment(r, p.r, increment);
    _increment(g, p.g, increment);
    _increment(b, p.b, increment);
}

void PixelColor::reset()
{
    r = g = b = 0;
    state = BEGIN;
}

void PixelColor::setNextColor(const PixelColor & color, const STATE state = NO_CHANGE){
    nextColor = &color;
    // Update state to new state if requested
    if (state != NO_CHANGE){
        this->state = state;
    }
}

void PixelColor::update(uint8_t increment)
{
    // Assuming we are going forward and backwards through colors
    switch (state)
    {
        case BEGIN:
            // Initiate state and fall through
            state = FORWARD;
        case FORWARD:
            if (nextColor)
            {
                stepTo(*nextColor, increment);
                // Reached target
                if (this == *nextColor){
                    state = REACHED_TARGET;
                }
            }
            break;
        case BACKWARD:
        case HALF:
        case MAX:
        case COMPLETE:
        case WAITING:
        case NOT_IN_USE:
        default:
            break;
    }
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
        _randomize_colors(targetColor);
        // All colors reached target?
        targetColor.state = PixelColor::BEGIN;
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
    /*
    previousBrightness = brightness = MAX_BRIGHTNESS / 2;
    targetColor.r = targetColor.g = targetColor.b = 0;
    // targetColor.b = MAX_COLOR;
    targetColor.flag = true;
    */

   resetColors(MAX_BRIGHTNESS, true);
}

void PixelMagic::initLeftToRight()
{
    // Brightness at the bottom Increments in 5 to 50
    resetColors(MAX_BRIGHTNESS, true);
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
    //previousBrightness = brightness;

    if (targetColor.state == PixelColor::BEGIN)
    {
        // Move to that color
        for(int i=0; i<NUMPIXELS; i++) // For each pixel...
        {
            // Step along all colors until they arrive at the target
            _update_led(targetColor, colors[i], COLOR_INC);
            
            // End goal
            if (i == NUMPIXELS-1 && colors[i].state == PixelColor::MAX){
                targetColor.state = PixelColor::COMPLETE;
            }
        }
    } else {
        // Dim
        for(int i=0; i<NUMPIXELS; i++) // For each pixel...
        {
            // Step along all colors until they arrive at the target
            _update_led(BLACK, colors[i], COLOR_INC);

            // End goal
            if (i == NUMPIXELS-1 && colors[i].state == PixelColor::COMPLETE){
                initBreathing();
            }
        }
    }
}

void PixelMagic::updateLeftToRight()
{
    if (targetColor.state == PixelColor::COMPLETE)
    {
        initLeftToRight();
    } else 
    {
        
        for (int i = 0; i < NUMPIXELS; i++)
        {
            // Dim
            if (colors[i].state == PixelColor::MAX && (targetColor.state == PixelColor::HALF || targetColor.state == PixelColor::MAX || targetColor.state == PixelColor::BACKWARD))
            {
                _update_led(BLACK, colors[i], COLOR_INC);
                    
                // Only start the next one if half way there
                if (colors[i].r > targetColor.r/2 || colors[i].g > targetColor.g/2 || colors[i].b > targetColor.b/2){
                    break;
                }

                if (i >= NUMPIXELS/2 && colors[i].state == PixelColor::COMPLETE){
                    _randomize_colors(targetColor);
                    targetColor.state = PixelColor::BACKWARD;
                }
                
                // End goal
                if (i == NUMPIXELS-1 && colors[i].state == PixelColor::COMPLETE){
                    targetColor.state = PixelColor::COMPLETE;
                }
            }
        }
        for (int i = 0; i < NUMPIXELS; i++)
        {
            // Go to color
            if (colors[i].state == PixelColor::BEGIN || targetColor.state == PixelColor::BACKWARD)
            {
                _update_led(targetColor, colors[i], COLOR_INC);

                if (colors[i].r < targetColor.r/3 || colors[i].g < targetColor.g/3 || colors[i].b < targetColor.b/3){
                    break;
                }
                
                // Half way
                if (i >= NUMPIXELS/3 && colors[i].state == PixelColor::MAX){
                    targetColor.state = PixelColor::HALF;
                }
                // End goal
                if (i == NUMPIXELS-1 && colors[i].state == PixelColor::MAX){
                    targetColor.state = PixelColor::MAX;
                }
            }
        }
    }
}
void PixelMagic::updateRightToLeft()
{
#if 0
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
#endif
}

void PixelMagic::updateCenterOut()
{
#if 0
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
#endif
}
void PixelMagic::updateOutToCenter()
{
#if 0
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
#endif
}


// Update one led
bool PixelMagic::_update_led(const PixelColor & targetColor, PixelColor & c1, uint8_t increment)
{
    if (c1.state == PixelColor::BEGIN){
        c1.stepTo(targetColor, increment);
        
        pixels.setPixelColor(c1.index, c1.r, c1.g, c1.b);
        
        if (c1 == targetColor)
        {
            c1.state = PixelColor::MAX;
            return true;
        }
    } else if (c1.state == PixelColor::MAX) {
        c1.stepTo(targetColor, increment);

        pixels.setPixelColor(c1.index, c1.r, c1.g, c1.b);
        
        if (c1 == targetColor)
        {
            c1.state = PixelColor::COMPLETE;
            return true;
        }
    }

    // Not finished
    return false;
}

// Update two leds at once
bool PixelMagic::_update_two_leds(const PixelColor & targetColor, PixelColor & c1, PixelColor & c2, uint8_t increment)
{
    return _update_led(targetColor, c1, increment) && _update_led(targetColor, c2, increment);
}

// Randomize colors
void PixelMagic::_randomize_colors(PixelColor & color)
{
    color.r = _random_color(0);
    color.g = _random_color(0);
    color.b = _random_color(0);
}
