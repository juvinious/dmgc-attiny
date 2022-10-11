# DMGC-ATTINY VIEWER
A software implementation of the attiny85 code that [BucketM0use](https://github.com/MouseBiteLabs/) made for the [DMGC](https://github.com/MouseBiteLabs/Game-Boy-DMG-Color).

# HOW TO USE
## Run with SDL2
- Install c++ development tools, libyaml-cpp and SDL2
  - Example in Ubuntu install: apt-get install build-essential libyaml-cpp-dev libsdl2-dev 
- make
- executables are dropped in bin directory

## In Arduino IDE
- Symlink library directory as src in the directory of the sketch
  - In windows: mklink /J src ..\..\library
  - In linux: ln -s ../../library src
- Open ino sketch, compile & upload

## Finally
Have fun!

# TODO
- Cleanup/Optimize content
- Handle EEPROM functionality

