#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#ifdef DMGC_SDL_ARDUINO_BUILD
#include "dmgc-utils.h"
  // Forward declarations
  void shiftColors();
  void outputLED();
#else
#include "src/dmgc-utils/dmgc-utils.h"
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define OUT           PCINT4 // PB4

/* Button mapper and poller */
DMGC_UTILS::ButtonHandler buttons;
DMGC_UTILS::Button * NAVIGATION = NULL, 
                   * LEFT = NULL,
                   * RIGHT = NULL,
                   * SELECT = NULL;


const uint8_t NUMPIXELS = 8;

const uint8_t    DEBOUNCE_DELAY = 170;
const uint8_t    BRIGHTNESS_INC = 5;
const uint8_t    MIN_BRIGHTNESS = 5;
const uint8_t    MAX_BRIGHTNESS = 50;

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

  PixelMagic():
  pixels(NUMPIXELS, OUT, NEO_GRB + NEO_KHZ800),
  brightness(MAX_BRIGHTNESS / 2), // Start brightness in the middle
  currentMode(MODE::BREATHING)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      colors[i].r = colors[i].g = 0;
      colors[i].b = 255;
      colors[i].forward = true;
    }
  }
  ~PixelMagic(){
  }

  void setup()
  {
    // Setup pixels
    pixels.begin();
    // Set brightness
    pixels.setBrightness(brightness);
  }

  void update()
  {
    switch (currentMode)
    {
      case MODE::LEFT_TO_RIGHT:
        break;
      case MODE::RIGHT_TO_LEFT:
        break;
      case MODE::CENTER_OUT:
        break;
      case MODE::OUT_TO_CENTER:
        break;
      case MODE::BREATHING:
      default:
        breathing();
        break;
    }
  }

  void show()
  {
    // Update the brightness
    pixels.setBrightness(brightness);

    switch (currentMode)
    {
      case MODE::BREATHING:
      default:
        for(int i=0; i<NUMPIXELS; i++) // For each pixel...
          pixels.setPixelColor(i, colors[0].r, colors[0].g, colors[0].b);
    }

    // Finally Show pixels
    pixels.show();
  }

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
  void breathing(){
    // Since we are only are using a single color we can just use the first index
    PixelColor & color = colors[0];

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
  }

private:
  Adafruit_NeoPixel pixels;
  PixelColor colors[NUMPIXELS];
  volatile uint8_t brightness;
  enum MODE currentMode;
};

PixelMagic magic;

void setup() {

  // Setup initial items
  magic.setup();

  // Add buttons
  NAVIGATION = buttons.add(PCINT3);
  LEFT = buttons.add(PCINT2);
  RIGHT = buttons.add(PCINT1);
  SELECT = buttons.add(PCINT0);

  buttons.poll(HIGH);

  // Skip intro if nav switch is pushed in
  //if (digitalRead(pushbtn)==HIGH){
  if (NAVIGATION->getState() == DMGC_UTILS::Button::UP)
  {
#if DMGC_SDL_ARDUINO_BUILD
    // Skip intro/outro delays
    DMGC_UTILS::dmgc_intro(magic.getPixels(), true);
#else
    DMGC_UTILS::dmgc_intro(magic.getPixels());
#endif
  }else{
    while(1){      
#if DMGC_SDL_ARDUINO_BUILD
    // So we can get out of this mess if pressed
    buttons.poll(HIGH);
    if (NAVIGATION->isClicked()) exit(0);
    delay(DEBOUNCE_DELAY);
#endif
    };
  }
}
void loop() {
  // Update display
  magic.show();

  // Poll the buttons
  buttons.poll(HIGH);

  // Update current mode
  magic.update();

  delay(DEBOUNCE_DELAY);
}
