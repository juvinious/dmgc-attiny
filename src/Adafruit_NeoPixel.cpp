#include "Adafruit_NeoPixel.h"
#include "configuration.h"


Adafruit_NeoPixel::Adafruit_NeoPixel(uint16_t n, int16_t pin, uint16_t type){
    Configuration::Get()->setTotalLeds(n);
}

Adafruit_NeoPixel::Adafruit_NeoPixel(void){
}
Adafruit_NeoPixel::~Adafruit_NeoPixel(){
}

void Adafruit_NeoPixel::begin(void){
}
void Adafruit_NeoPixel::show(void){
    Configuration::Get()->renderBackground();
    Configuration::Get()->renderLeds();
    if (!Configuration::Get()->isSetupComplete())
    {
        Configuration::Get()->screenRender();
    }
}
void Adafruit_NeoPixel::setPin(int16_t p){
}
void Adafruit_NeoPixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b){
}
void Adafruit_NeoPixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w){
}
void Adafruit_NeoPixel::setPixelColor(uint16_t n, _Color c){
    Configuration::Get()->setPixelColor(n, c.r, c.g, c.b);
}
void Adafruit_NeoPixel::fill(uint32_t c, uint16_t first, uint16_t count){
}
void Adafruit_NeoPixel::setBrightness(uint8_t b){
    Configuration::Get()->setBrightness(b);
}
void Adafruit_NeoPixel::clear(void){
    //Configuration::Get()->screenClear();
}
void Adafruit_NeoPixel::updateLength(uint16_t n){
}
void Adafruit_NeoPixel::updateType(neoPixelType t){
}
uint16_t Adafruit_NeoPixel::numPixels(void) const 
{
    return Configuration::Get()->getTotalLeds();
}
uint32_t Adafruit_NeoPixel::getPixelColor(uint16_t n) const {
    return 0;
}