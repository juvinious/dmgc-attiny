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
    switch (pin){
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            return 1;
    }
    return 1;
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
    // printf("Delaying %d            \r", length);
}