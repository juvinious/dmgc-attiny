#include "EEPROM.h"
#include "configuration.h"

#include <type_traits>

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

/*
template<typename T> 
T &EEPROMClass::get(int const address, T &t) volatile
{
	YAML::Node data = Configuration::Get()->loadData();
	t = data[address].as<T>();
	return t;
}
*/

template<typename T> 
void EEPROMClass::get(int const address, T &t) volatile
{
	const std::string pos = std::to_string(address);
	YAML::Node data = Configuration::Get()->loadData();
	if (data[pos])
	{
		t = data[pos].as<T>();
	}
}

void EEPROMClass::get(int const address, volatile unsigned char & value) volatile
{
	std::string pos = std::to_string(address);
	YAML::Node data = Configuration::Get()->loadData();
	if (data[pos])
	{
		unsigned char ch = data[pos].as<unsigned char>();
		value = ch; // dynamic_cast<volatile unsigned char>(ch);
	}
}

/*
template<typename T> 
const T & EEPROMClass::put(int const address, const T &t) volatile
{

}
*/

template<typename T> 
void EEPROMClass::put(int const address, T &t) volatile
{
	const std::string pos = std::to_string(address);
	YAML::Node data = Configuration::Get()->loadData();
	data[pos] = t;
	Configuration::Get()->writeData(data);
}

void EEPROMClass::put(int const address, volatile unsigned char & value) volatile
{
	const std::string pos = std::to_string(address);
	YAML::Node data = Configuration::Get()->loadData();
	unsigned char ch = value;
	data[pos] = ch;
	Configuration::Get()->writeData(data);
}

// Explicit initilazations
template<> void EEPROMClass::get<int>(int const address, int &t) volatile;
template<> void EEPROMClass::put<int>(int const address, int &t) volatile;

EEPROMClass EEPROM;