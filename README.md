# DMGC-ATTINY VIEWER
A software implementation of the ATTINY85 code that [BucketM0use](https://github.com/MouseBiteLabs/) made for the [DMGC](https://github.com/MouseBiteLabs/Game-Boy-DMG-Color).

# HOW TO USE
## Run with SDL2
- Install c++ development tools, libyaml-cpp and SDL2
  - Example in Ubuntu install: apt-get install build-essential libyaml-cpp-dev libsdl2-dev 
- make
- executables are dropped in bin directory

## Examples
There are currently three example sketches
### DMGC-IPS-ATTINY85
This is the original sketch. Refer to [DMGC-IPS-01](https://github.com/MouseBiteLabs/Game-Boy-DMG-Color/tree/main/DMGC-IPS-01#button-leds-and-attiny85) for details.
### DMGC-IPS-ATTINY85-PLUS
This is the same as the original, but improved using the key/button handler which is using states to determine the positions of buttons. Better practice instead of blocking with while loops. :P.
### BREATHING
A more extended example, allowing for different modes ([incomplete, see todo](#todo)).
  - Breathing - Increases and decreases brightness at a specific rate. Changes to random color when brightness is at 5 or below, starting a new cycle
  - Left to Right (LTR) - Run a gradient from left to right while brightning the a random color to the max, fade out and then restart
  - Right to Left - Same as LTR, but going in the opposite direction
  - Center Out - Same as LTR, but starting from select and start and going outward
  - Out to Center - Same as LTR, but starting at Left and A

## Deploy to ATTINY85 in Arduino IDE
- Symlink library directory as src in the directory of the sketch
  - In windows: mklink /J src ..\..\library
  - In linux: ln -s ../../library src
- Open ino sketch, compile & upload

## Finally
Have fun!

# TODO
- Cleanup/Optimize configuration content
- Add additional utils as needed
- EEPROM
  - Fix get/put templating
  - Implement other functionality
- Breathing example
  - Finish the rest of the modes

