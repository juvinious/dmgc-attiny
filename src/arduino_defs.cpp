#include "arduino_defs.h"
#include "configuration.h"

void init(void){

}
void initVariant(void){

}

void pinMode(uint8_t pin, uint8_t mode){

}

void digitalWrite(uint8_t pin, uint8_t val){

}

int digitalRead(uint8_t pin){
    //if (!Configuration::Get()->isSetupComplete())
    //{
        // Have to update the reads regardless, because the default loops bucketm0use uses are killing me!!!!! :)
        bool running = true;
        Configuration::Get()->handleKeys(running);
    //}

    // If pins are pulled high by default, then we need to inverse the check
    bool inversePressed = Configuration::Get()->pullDirection() == HIGH;

    return inversePressed ? !Configuration::Get()->checkKeyByPin(pin) : Configuration::Get()->checkKeyByPin(pin);
}

int analogRead(uint8_t pin){
    return 0;
}

void analogReference(uint8_t mode){

}

void analogWrite(uint8_t pin, int val){

}

void analogWriteMode(uint8_t pin, int val, bool openDrain){

}

void analogWriteFreq(uint32_t freq){

}

void analogWriteResolution(int res){

}

void analogWriteRange(uint32_t range){

}

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout){
    return 0;
}

unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout){
    return 0;
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val){

}

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder){
    return 0;
}

void delay(uint32_t length){
    Configuration::Get()->delay(length);   
    // printf("Delaying %d            \n", length);
}