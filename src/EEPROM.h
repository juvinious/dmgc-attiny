#ifndef EEPROM_H
#define EEPROM_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

class EEPROMClass {
public:
  EEPROMClass(uint32_t sector);
  EEPROMClass(void);

  void begin(size_t size);
  uint8_t read(int const address);
  void write(int const address, uint8_t const val);
  bool commit();
  bool end();

  uint8_t * getDataPtr();
  uint8_t const * getConstDataPtr() const;

  template<typename T> 
  //T &get(int const address, T &t) volatile {
  void get(int const address, T &t) volatile {
    if (address < 0 || address + sizeof(T) > _size)
      //return t;

    memcpy((uint8_t*) &t, _data + address, sizeof(T));
    //return t;
  }

  template<typename T> 
  //const T &put(int const address, const T &t) volatile {
  void put(int const address, const T &t) volatile {
    if (address < 0 || address + sizeof(T) > _size)
      //return t;
    if (memcmp(_data + address, (const uint8_t*)&t, sizeof(T)) != 0) {
      _dirty = true;
      memcpy(_data + address, (const uint8_t*)&t, sizeof(T));
    }

    //return t;
  }

  size_t length() {return _size;}

  uint8_t& operator[](int const address) {return getDataPtr()[address];}
  uint8_t const & operator[](int const address) const {return getConstDataPtr()[address];}

protected:
  uint32_t _sector;
  uint8_t* _data = nullptr;
  size_t _size = 0;
  bool _dirty = false;
};

//#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_EEPROM)
extern EEPROMClass EEPROM;
//#endif

#endif
