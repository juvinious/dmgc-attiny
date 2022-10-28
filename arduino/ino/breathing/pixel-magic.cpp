#include "pixel-magic.h"

// Which pin on the Arduino is connected to the NeoPixels?
#define OUT           PCINT4 // PB4

// Defaults
static const uint8_t BRIGHTNESS_INC = 5;
static const uint8_t MIN_BRIGHTNESS = 5;
static const uint8_t MAX_BRIGHTNESS = 50;
static const uint8_t MIN_COLOR = 25;
static const uint8_t MAX_COLOR = 255;
static const uint8_t COLOR_INC = 25;
static const uint8_t MAX_COLOR_INC = 125;
static const uint8_t MIN_COLOR_INC = 10;

// STATIC COLORS
const PixelColor PixelColor::WHITE = PixelColor(0,255,255,255,PixelColor::NOT_IN_USE);
const PixelColor PixelColor::BLACK = PixelColor(0,0,0,0,PixelColor::NOT_IN_USE);
const PixelColor PixelColor::RED = PixelColor(0,255,0,0,PixelColor::NOT_IN_USE);
const PixelColor PixelColor::GREEN = PixelColor(0,0,255,0,PixelColor::NOT_IN_USE);
const PixelColor PixelColor::BLUE = PixelColor(0,0,0,255,PixelColor::NOT_IN_USE);

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

PixelColor::PixelColor(uint8_t index, int r, int g, int b, const PixelColor::STATE & state):
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

void PixelColor::random()
{
    r = _random_color(0);
    g = _random_color(0);
    b = _random_color(0);
}

void PixelColor::setColorAs(const PixelColor & color){
    r = color.r;
    g = color.g;
    b = color.b;
}

void PixelColor::setNextColor(const PixelColor & color){
    nextColor = &color;
}

void PixelColor::update(Adafruit_NeoPixel & pixels, uint8_t increment)
{
    // Assuming we are going forward and backwards through colors
    switch (state)
    {
        /* FADE TO COLOR IN AND OUT */
        case BEGIN:
            // Initiate state and fall through
            state = FORWARD;
        case FORWARD:
            if (nextColor)
            {
                stepTo(*nextColor, increment);
                // Reached target
                if (*this == *nextColor){
                    state = REACHED_TARGET;
                }
            }
            break;
        case REACHED_TARGET:
            // Return to BLACK
            state = BACKWARD;
            nextColor = &BLACK;
        case BACKWARD:
            if (nextColor)
            {
                stepTo(*nextColor, increment);
                // Reached target
                if (*this == *nextColor){
                    state = COMPLETE;
                }
            }
            break;
        /* END IN AND OUT STATES */
        case HALF:
        case MAX:
            // Completed state does idles
        case COMPLETE:
        case WAITING:
        case NOT_IN_USE:
        default:
            break;
    }

    // Finally update the pixel
    pixels.setPixelColor(index, r, g, b);
}

bool PixelColor::isNearTargetColorBy(int amount){
    if (!nextColor)
        return false;
    return (r+g+b) > (nextColor->r+nextColor->g+nextColor->b)/amount;
}

PixelMagic::PixelMagic():
pixels(NUMPIXELS, OUT, NEO_GRB + NEO_KHZ800),
incrementSpeed(COLOR_INC),
brightness(MAX_BRIGHTNESS / 2), // Start brightness in the middle,
previousBrightness(brightness),
randomColors(0),
previousMode(MODE::NOT_DEFINED),
currentMode(MODE::BREATHING)
{
    for (int i = 0; i < NUMPIXELS; i++){
        // Set each index
        colors[i].index = i;
    }
    initBreathing();
    targetColor.r = targetColor.g = 0;
    targetColor.b = 255;
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
        case MODE::RANDOM:
            updateRandom();
            break;
        case MODE::SOLID:
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
    if (currentMode != PixelMagic::SOLID)
    {
        // Cycle forward through the modes
        uint8_t mode = static_cast<MODE>((static_cast<int>(currentMode) + 1) % NUM_MODES);
        setMode(mode);
    }
}

void PixelMagic::setMode(uint8_t mode)
{
    if ((mode >= 0 && mode < PixelMagic::NUM_MODES) || mode == PixelMagic::SOLID)
    {    
        previousMode = currentMode;
        currentMode = static_cast<MODE>(mode);
    } else 
    {
        previousMode = PixelMagic::NOT_DEFINED;
        currentMode = PixelMagic::BREATHING;
    }
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
        case MODE::RANDOM:
            initRandom();
            break;
        case MODE::SOLID:        
            for (int i = 0; i < NUMPIXELS; i++){
                // Set pixels to current target color
                colors[i].setColorAs(targetColor);
                colors[i].state = PixelColor::NO_CHANGE;
                // Set next color to black
                const PixelColor * t = const_cast<const PixelColor *>(&PixelColor::BLACK);
                colors[i].setNextColor(*t);
                // Finally set led
                pixels.setPixelColor(i, colors[i].r, colors[i].g, colors[i].b);
            }
            break;
        case MODE::OFF:
        default:
            resetColors(PixelColor::NOT_IN_USE, MIN_BRIGHTNESS);
            break;
    }
}

void PixelMagic::increaseIncrement()
{
    incrementSpeed += 5;
    if (incrementSpeed > MAX_COLOR_INC)
    {
        incrementSpeed = MAX_COLOR_INC;
    }
}

void PixelMagic::decreaseIncrement()
{
    incrementSpeed -= 5;
    if (incrementSpeed <= MIN_COLOR_INC)
    {
        incrementSpeed = MIN_COLOR_INC;
    }
}

void PixelMagic::setIncrementSpeed(uint8_t speed)
{
    if (speed > MAX_COLOR_INC)
    {
        incrementSpeed = MAX_COLOR_INC;
    } else {
        incrementSpeed = COLOR_INC;
    }
}

void PixelMagic::toggleRandomColors()
{
    this->randomColors = !this->randomColors;
    setMode(this->currentMode);
}

void PixelMagic::toggleSolidColors()
{
    if (this->currentMode == SOLID)
    {
        setMode(previousMode != NOT_DEFINED ? previousMode : BREATHING);
    } else {
        setMode(SOLID);
    }
}

void PixelMagic::toggleConfiguration()
{
    if (this->currentMode == CONFIGURATION)
    {
        setMode(previousMode != NOT_DEFINED ? previousMode : BREATHING);
    } else {
        setMode(CONFIGURATION);
    }
}

void PixelMagic::resetColors(const PixelColor::STATE & state, uint8_t brightness)
{
    // Brightness at the bottom Increments in 5 to 50
    this->previousBrightness = this->brightness = brightness;
    if (randomColors == 1)
    {
        // Random color to move to
        targetColor.random();
    }

    for (int i = 0; i < NUMPIXELS; i++){
        // All pixels off and reset flag
        colors[i].reset();
        colors[i].state = state;
        // Set next color
        const PixelColor * t = const_cast<const PixelColor *>(&targetColor);
        colors[i].setNextColor(*t);
        // Finally set led
        pixels.setPixelColor(i, colors[i].r, colors[i].g, colors[i].b);
    }
}

void PixelMagic::initBreathing()
{
   resetColors(PixelColor::BEGIN, MAX_BRIGHTNESS);
}

void PixelMagic::initLeftToRight()
{
    resetColors(PixelColor::BEGIN, MAX_BRIGHTNESS);
}

void PixelMagic::initRightToLeft()
{
    resetColors(PixelColor::BEGIN, MAX_BRIGHTNESS);
}

void PixelMagic::initCenterOut()
{
    resetColors(PixelColor::BEGIN, MAX_BRIGHTNESS);
}

void PixelMagic::initOutToCenter()
{
    resetColors(PixelColor::BEGIN, MAX_BRIGHTNESS);
}

void PixelMagic::initRandom()
{
    resetColors(PixelColor::BEGIN, MAX_BRIGHTNESS);
    // Set up random order
    bool chosen[NUMPIXELS] = {false,false,false,false,false,false,false,false};
    for (int i = 0; i < NUMPIXELS; i++){
        int r = random(0, NUMPIXELS);
        if (!chosen[r])
        {
            mixedOrder[i] = r;
            chosen[r] = true;
        } else {
            while (chosen[r]){
                r = random(0, NUMPIXELS);
            }
            mixedOrder[i] = r;
            chosen[r] = true;
        }
    }
    
}

void PixelMagic::updateBreathing(){
    // Update pixels
    for(int i=0; i<NUMPIXELS; i++) // For each pixel...
    {
        colors[i].update(pixels, incrementSpeed);
        if (i == NUMPIXELS-1 && colors[i].state == PixelColor::COMPLETE){
            initBreathing();
        }
    }
}

void PixelMagic::updateLeftToRight()
{
    for(int i=0; i<NUMPIXELS; i++) // For each pixel...
    {
        colors[i].update(pixels, incrementSpeed);
        if (!colors[i].isNearTargetColorBy(2) && colors[i].state == PixelColor::FORWARD)
        {
            break;
        }

        if (i == NUMPIXELS-1 && colors[i].state == PixelColor::COMPLETE){
            initLeftToRight();
        }
    }
}

void PixelMagic::updateRightToLeft()
{
    for(int i= (NUMPIXELS-1); i >= 0; i--) // For each pixel...
    {
        colors[i].update(pixels, incrementSpeed);
        if (!colors[i].isNearTargetColorBy(2) && colors[i].state == PixelColor::FORWARD)
        {
            break;
        }

        if (i == 0 && colors[i].state == PixelColor::COMPLETE){
            initRightToLeft();
        }
    }
}

void PixelMagic::updateCenterOut()
{
    uint8_t completed = 0;
    // Left Side
    for (int i = 3; i >= 0; i--)
    {
        colors[i].update(pixels, incrementSpeed);
        if (!colors[i].isNearTargetColorBy(2) && colors[i].state == PixelColor::FORWARD)
        {
            break;
        }
        completed += colors[i].state == PixelColor::COMPLETE ? 1 : 0;
    }
    // Right Side
    for (int i = 4; i < NUMPIXELS; i++)
    {
        colors[i].update(pixels, incrementSpeed);
        if (!colors[i].isNearTargetColorBy(2) && colors[i].state == PixelColor::FORWARD)
        {
            break;
        }
        completed += colors[i].state == PixelColor::COMPLETE ? 1 : 0;
    }
    if (completed == NUMPIXELS)
    {
        initCenterOut();
    }
}
void PixelMagic::updateOutToCenter()
{
    uint8_t completed = 0;
    // Left Side
    for (int i = 0; i < 4; i++)
    {
        colors[i].update(pixels, incrementSpeed);
        if (!colors[i].isNearTargetColorBy(2) && colors[i].state == PixelColor::FORWARD)
        {
            break;
        }
        completed += colors[i].state == PixelColor::COMPLETE ? 1 : 0;
    }
    // Right Side
    for (int i = (NUMPIXELS-1); i >= 4; i--)
    {
        colors[i].update(pixels, incrementSpeed);
        if (!colors[i].isNearTargetColorBy(2) && colors[i].state == PixelColor::FORWARD)
        {
            break;
        }
        completed += colors[i].state == PixelColor::COMPLETE ? 1 : 0;
    }
    if (completed == NUMPIXELS)
    {
        initOutToCenter();
    }
}

void PixelMagic::updateRandom()
{
    int completed = 0;
    for (int i = 0; i < NUMPIXELS; i++){
        int & current = mixedOrder[i];
        colors[current].update(pixels, incrementSpeed);
        if (!colors[current].isNearTargetColorBy(2) && colors[current].state == PixelColor::FORWARD)
        {
            break;
        }
        completed += colors[i].state == PixelColor::COMPLETE ? 1 : 0;
    }
    
    if (completed == NUMPIXELS)
    {
        initRandom();
    }
}