#ifndef ADAFRUIT_NEOPIXEL_H
#define ADAFRUIT_NEOPIXEL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "arduino_defs.h"

// RGB NeoPixel permutations; white and red offsets are always same
// Offset:         W        R        G        B
#define NEO_RGB ((0 << 6) | (0 << 4) | (1 << 2) | (2)) ///< Transmit as R,G,B
#define NEO_RBG ((0 << 6) | (0 << 4) | (2 << 2) | (1)) ///< Transmit as R,B,G
#define NEO_GRB ((1 << 6) | (1 << 4) | (0 << 2) | (2)) ///< Transmit as G,R,B
#define NEO_GBR ((2 << 6) | (2 << 4) | (0 << 2) | (1)) ///< Transmit as G,B,R
#define NEO_BRG ((1 << 6) | (1 << 4) | (2 << 2) | (0)) ///< Transmit as B,R,G
#define NEO_BGR ((2 << 6) | (2 << 4) | (1 << 2) | (0)) ///< Transmit as B,G,R

// RGBW NeoPixel permutations; all 4 offsets are distinct
// Offset:         W          R          G          B
#define NEO_WRGB ((0 << 6) | (1 << 4) | (2 << 2) | (3)) ///< Transmit as W,R,G,B
#define NEO_WRBG ((0 << 6) | (1 << 4) | (3 << 2) | (2)) ///< Transmit as W,R,B,G
#define NEO_WGRB ((0 << 6) | (2 << 4) | (1 << 2) | (3)) ///< Transmit as W,G,R,B
#define NEO_WGBR ((0 << 6) | (3 << 4) | (1 << 2) | (2)) ///< Transmit as W,G,B,R
#define NEO_WBRG ((0 << 6) | (2 << 4) | (3 << 2) | (1)) ///< Transmit as W,B,R,G
#define NEO_WBGR ((0 << 6) | (3 << 4) | (2 << 2) | (1)) ///< Transmit as W,B,G,R

#define NEO_RWGB ((1 << 6) | (0 << 4) | (2 << 2) | (3)) ///< Transmit as R,W,G,B
#define NEO_RWBG ((1 << 6) | (0 << 4) | (3 << 2) | (2)) ///< Transmit as R,W,B,G
#define NEO_RGWB ((2 << 6) | (0 << 4) | (1 << 2) | (3)) ///< Transmit as R,G,W,B
#define NEO_RGBW ((3 << 6) | (0 << 4) | (1 << 2) | (2)) ///< Transmit as R,G,B,W
#define NEO_RBWG ((2 << 6) | (0 << 4) | (3 << 2) | (1)) ///< Transmit as R,B,W,G
#define NEO_RBGW ((3 << 6) | (0 << 4) | (2 << 2) | (1)) ///< Transmit as R,B,G,W

#define NEO_GWRB ((1 << 6) | (2 << 4) | (0 << 2) | (3)) ///< Transmit as G,W,R,B
#define NEO_GWBR ((1 << 6) | (3 << 4) | (0 << 2) | (2)) ///< Transmit as G,W,B,R
#define NEO_GRWB ((2 << 6) | (1 << 4) | (0 << 2) | (3)) ///< Transmit as G,R,W,B
#define NEO_GRBW ((3 << 6) | (1 << 4) | (0 << 2) | (2)) ///< Transmit as G,R,B,W
#define NEO_GBWR ((2 << 6) | (3 << 4) | (0 << 2) | (1)) ///< Transmit as G,B,W,R
#define NEO_GBRW ((3 << 6) | (2 << 4) | (0 << 2) | (1)) ///< Transmit as G,B,R,W

#define NEO_BWRG ((1 << 6) | (2 << 4) | (3 << 2) | (0)) ///< Transmit as B,W,R,G
#define NEO_BWGR ((1 << 6) | (3 << 4) | (2 << 2) | (0)) ///< Transmit as B,W,G,R
#define NEO_BRWG ((2 << 6) | (1 << 4) | (3 << 2) | (0)) ///< Transmit as B,R,W,G
#define NEO_BRGW ((3 << 6) | (1 << 4) | (2 << 2) | (0)) ///< Transmit as B,R,G,W
#define NEO_BGWR ((2 << 6) | (3 << 4) | (1 << 2) | (0)) ///< Transmit as B,G,W,R
#define NEO_BGRW ((3 << 6) | (2 << 4) | (1 << 2) | (0)) ///< Transmit as B,G,R,W

#define NEO_KHZ800 0x0000
#define NEO_KHZ400 0x0100

typedef uint16_t neoPixelType;


struct _Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

class Adafruit_NeoPixel {
  public:
	Adafruit_NeoPixel(uint16_t n, int16_t pin, uint16_t type);
	Adafruit_NeoPixel(void);
	~Adafruit_NeoPixel();

	void begin(void);
	void show(void);
	void setPin(int16_t p);
	void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
	void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
	//void setPixelColor(uint16_t n, uint32_t c);
  void setPixelColor(uint16_t n, _Color c);
	void fill(uint32_t c, uint16_t first, uint16_t count);
	void setBrightness(uint8_t);
	void clear(void);
	void updateLength(uint16_t n);
	void updateType(neoPixelType t);

  uint16_t numPixels(void) const;
  uint32_t getPixelColor(uint16_t n) const;

  static _Color Color(uint8_t r, uint8_t g, uint8_t b) {
    return {r,g,b};
  }
#if 0
	static uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
  }
  /*!
    @brief   Convert separate red, green, blue and white values into a
             single "packed" 32-bit WRGB color.
    @param   r  Red brightness, 0 to 255.
    @param   g  Green brightness, 0 to 255.
    @param   b  Blue brightness, 0 to 255.
    @param   w  White brightness, 0 to 255.
    @return  32-bit packed WRGB value, which can then be assigned to a
             variable for later use or passed to the setPixelColor()
             function. Packed WRGB format is predictable, regardless of
             LED strand color order.
  */
  static uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
  }
#endif
  static uint32_t ColorHSV(uint16_t hue, uint8_t sat = 255, uint8_t val = 255);
};

#endif /* ENDIF ADAFRUIT_NEOPIXEL_H */
