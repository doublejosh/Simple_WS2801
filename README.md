Stripped down library for controlling WS2801 pixels via tiny non-SPI micro-controllers.

###Usage
```c++
#include "Simple_WS2801.h"

const int PIN_DATA = 2;
const int PIN_CLOCK = 3;

void setup() {
  pinMode(PIN_DATA, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  strip.begin();
}

void loop() {
  // Strandtest from standard library...
  int i, j;
  // 3 cycles of all 256 colors in the wheel.
  for (j=0; j < 256; j++) {
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }
    // Write all the pixels out.
    strip.show();
    delay(20);
  }
}

// Turn an int value 0 to 255 into a color value.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

// Build a color from RGB values, required by Wheel().
// NOTE: You can also pass rgb values directly into setPixelColor().
uint32_t Color(byte r, byte g, byte b) {
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
```

-------------------------------------

###Install
To install within Arduino IDE, place the Simple_WS2801 library folder
within your <arduinosketchfolder>/libraries/ folder. You may need to
create the libraries subfolder if its your first library.
Restart the IDE.

-------------------------------------

###Authorship
Original library written by Limor Fried/Ladyada for Adafruit Industries.  
BSD license.

Stripped down and SPI removed for compact board use by Josh Lind.  
MIT License.

-------------------------------------

###REQUIRED:
Designed specifically to work with the Adafruit RGB Pixels!  
12mm Bullet shape ----> https://www.adafruit.com/products/322  
12mm Flat shape   ----> https://www.adafruit.com/products/738  
36mm Square shape ----> https://www.adafruit.com/products/683  

These pixels use SPI to transmit the color data, and have built in
high speed PWM drivers for 24 bit color per pixel. 2 pins are required
to interface.

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!
