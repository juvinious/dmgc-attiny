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
    Configuration::Get()->screenRender();
}
void Adafruit_NeoPixel::setPin(int16_t p){
}
void Adafruit_NeoPixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b){
}
void Adafruit_NeoPixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w){
}
void Adafruit_NeoPixel::setPixelColor(uint16_t n, _Color c){
    Configuration::Get()->setPixelColor(c.r, c.g, c.b);
}
#if 0
void Adafruit_NeoPixel::setPixelColor(uint16_t n, uint32_t c){
    if (n < Configuration::Get()->getTotalLeds()) 
    {
        const int brightness = Configuration::Get()->getBrightness();
        //uint8_t *p, r = (uint8_t)(c >> 16), g = (uint8_t)(c >> 8), b = (uint8_t)c;
        uint8_t r = (uint8_t)(c >> 16), g = (uint8_t)(c >> 8), b = (uint8_t)c;
        // printf("r: %d g: %d b: %d", r,g,b);
        if (brightness) { // See notes in setBrightness()
            r = (r * brightness) >> 8;
            g = (g * brightness) >> 8;
            b = (b * brightness) >> 8;
        }
        /*
        if (wOffset == rOffset) {
            p = &pixels[n * 3];
        } else {
            p = &pixels[n * 4];
            uint8_t w = (uint8_t)(c >> 24);
            p[wOffset] = brightness ? ((w * brightness) >> 8) : w;
        }
        */

        /*
        p[rOffset] = r;
        p[gOffset] = g;
        p[bOffset] = b;
        */
        Configuration::Get()->setPixelColor(r, g, b);
    }
}
#endif
void Adafruit_NeoPixel::fill(uint32_t c, uint16_t first, uint16_t count){
}
void Adafruit_NeoPixel::setBrightness(uint8_t b){
    Configuration::Get()->setBrightness(b);
}
void Adafruit_NeoPixel::clear(void){
    Configuration::Get()->screenClear();
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
