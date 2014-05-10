#include "Simple_WS2801.h"

/**
 * @file
 * Library to control WS2801-based RGB LED strips.
 * Written by Adafruit - MIT license.
 * Adapted to be tiny and not use SPI, by @doublejosh.
 */

// Constructor for use with arbitrary clock/data pins:
Simple_WS2801::Simple_WS2801(uint16_t n, uint8_t dpin, uint8_t cpin, uint8_t order) {
  rgb_order = order;
  alloc(n);
  updatePins(dpin, cpin);
}

// Allocate 3 bytes per pixel, init to RGB 'off' state:
void Simple_WS2801::alloc(uint16_t n) {
  begun   = false;
  numLEDs = ((pixels = (uint8_t *)calloc(n, 3)) != NULL) ? n : 0;
}

// Release memory (as needed):
Simple_WS2801::~Simple_WS2801(void) {
  if (pixels != NULL) {
    free(pixels);
  }
}

// Activate hard/soft:
void Simple_WS2801::begin(void) {
  pinMode(datapin, OUTPUT);
  pinMode(clkpin , OUTPUT);
  begun = true;
}

// Change pin assignments post-constructor, using arbitrary pins:
void Simple_WS2801::updatePins(uint8_t dpin, uint8_t cpin) {

  if(begun == true) { // If begin() was previously invoked...
    // Regardless, now enable output on 'soft' SPI pins:
    pinMode(dpin, OUTPUT);
    pinMode(cpin, OUTPUT);
  } // Otherwise, pins are not set to outputs until begin() is called.

  // Note: any prior clock/data pin directions are left as-is and are
  // NOT restored as inputs!
  datapin     = dpin;
  clkpin      = cpin;
  clkport     = portOutputRegister(digitalPinToPort(cpin));
  clkpinmask  = digitalPinToBitMask(cpin);
  dataport    = portOutputRegister(digitalPinToPort(dpin));
  datapinmask = digitalPinToBitMask(dpin);
}

uint16_t Simple_WS2801::numPixels(void) {
  return numLEDs;
}

void Simple_WS2801::show(void) {
  uint16_t i, nl3 = numLEDs * 3; // 3 bytes per LED
  uint8_t  bit;

  // Write 24 bits per pixel:
  for(i=0; i<nl3; i++ ) {
    for(bit=0x80; bit; bit >>= 1) {
      if(pixels[i] & bit) *dataport |=  datapinmask;
      else                *dataport &= ~datapinmask;
      *clkport |=  clkpinmask;
      *clkport &= ~clkpinmask;
    }
  }

  delay(1); // Data is latched by holding clock pin low for 1 millisecond
}

// Set pixel color from separate 8-bit R, G, B components:
void Simple_WS2801::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
  if(n < numLEDs) { // Arrays are 0-indexed, thus NOT '<='
    uint8_t *p = &pixels[n * 3];
    // See notes later regarding color order
    if(rgb_order == WS2801_RGB) {
      *p++ = r;
      *p++ = g;
    } else {
      *p++ = g;
      *p++ = r;
    }
    *p++ = b;
  }
}

// Set pixel color from 'packed' 32-bit RGB value:
void Simple_WS2801::setPixelColor(uint16_t n, uint32_t c) {
  if(n < numLEDs) { // Arrays are 0-indexed, thus NOT '<='
    uint8_t *p = &pixels[n * 3];
    // To keep the show() loop as simple & fast as possible, the
    // internal color representation is native to different pixel
    // types.  For compatibility with existing code, 'packed' RGB
    // values passed in or out are always 0xRRGGBB order.
    if(rgb_order == WS2801_RGB) {
      *p++ = c >> 16; // Red
      *p++ = c >>  8; // Green
    } else {
      *p++ = c >>  8; // Green
      *p++ = c >> 16; // Red
    }
    *p++ = c;         // Blue
  }
}
