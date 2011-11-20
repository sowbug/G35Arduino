/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  Original version by Paul Martis (http://www.digitalmisery.com). See
  README for complete attributions.
*/

#ifndef INCLUDE_G35_ARDUINO_H
#define INCLUDE_G35_ARDUINO_H

#include <WProgram.h>

#define color_t uint16_t
#define CHANNEL_MAX           (0xF)   // Each color channel is 4-bit
#define DEFAULT_INTENSITY     (0xCC)  // Brightness is 8-bit
#define HUE_MAX               ((CHANNEL_MAX+1)*6-1)

// Color is 12-bit (4-bit each R, G, B)
#define COLOR(r,g,b)          ((r)+((g)<<4)+((b)<<8))
#define COLOR_WHITE           COLOR(CHANNEL_MAX,CHANNEL_MAX,CHANNEL_MAX)
#define COLOR_BLACK           COLOR(0,0,0)
#define COLOR_RED             COLOR(CHANNEL_MAX,0,0)
#define COLOR_GREEN           COLOR(0,CHANNEL_MAX,0)
#define COLOR_BLUE            COLOR(0,0,CHANNEL_MAX)
#define COLOR_CYAN            COLOR(0,CHANNEL_MAX,CHANNEL_MAX)
#define COLOR_MAGENTA         COLOR(CHANNEL_MAX,0,CHANNEL_MAX)
#define COLOR_YELLOW          COLOR(CHANNEL_MAX,CHANNEL_MAX,0)
#define COLOR_PURPLE          COLOR(0xa,0x3,0xd)
#define COLOR_ORANGE          COLOR(0xf,0x1,0x0)
#define COLOR_WARMWHITE       COLOR(0xf,0x7,0x2)

class G35 {
 public:
  // Arduino pin # and total lights on string
  G35(int pin, int light_count);

  // Turn on a specific LED with a color and brightness
  void set_color(uint8_t led, uint8_t intensity, color_t color);

  // Color data type
  color_t color(uint8_t r, uint8_t g, uint8_t b);

  color_t color_hue(uint8_t h);

  // Make all LEDs the same color starting at specified beginning LED
  void fill_color(uint8_t begin, uint8_t count, uint8_t intensity,
                  color_t color);

  // Initialize lights by giving them each an address. enumerate_forward()
  // gives the bulb closest to the controller #0, and enumerate_reverse()
  // gives #0 to the farthest bulb.
  void enumerate_forward();
  void enumerate_reverse();
 private:
  void enumerate(bool reverse=false);

  // Low-level one-wire protocol commands
  void begin();
  void one();
  void zero();
  void end();
  int _pin;
  int _light_count;
};

#endif  // INCLUDE_G35_ARDUINO_H
