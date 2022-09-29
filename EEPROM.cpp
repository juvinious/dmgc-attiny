#include "EEPROM.h"

EEPROMClass::EEPROMClass(uint32_t sector){
}
EEPROMClass::EEPROMClass(void){
}

void EEPROMClass::begin(size_t size){
}

uint8_t EEPROMClass::read(int const address){
	return 0;
}

void EEPROMClass::write(int const address, uint8_t const val){
	
}

bool EEPROMClass::commit(){
	return true;
}

bool EEPROMClass::end(){
	return true;
}

uint8_t * EEPROMClass::getDataPtr(){
	return NULL;
}

uint8_t const * EEPROMClass::getConstDataPtr() const{
	return 0;
}


