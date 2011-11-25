/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  Original version by Paul Martis (http://www.digitalmisery.com). See
  README for complete attributions.
*/

#ifndef INCLUDE_G35_ARDUINO_H
#define INCLUDE_G35_ARDUINO_H

#include <Arduino.h>

#define color_t uint16_t
#define CHANNEL_MAX           (0xF)   // Each color channel is 4-bit
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
#define COLOR_INDIGO          COLOR(0x6,0,0xf)
#define COLOR_VIOLET          COLOR(0x8,0,0xf)

class G35 {
 public:
  enum {
    MAX_INTENSITY = 0xcc
  };

  // Arduino pin # and total lights on string
  G35(int pin, int light_count);

  // Initialize lights by giving them each an address. enumerate_forward()
  // numbers the bulb closest to the controller 0, and enumerate_reverse()
  // numbers the farthest bulb 0.
  void enumerate_forward();
  void enumerate_reverse();

  // Displays known-good patterns. Useful to prevent insanity during hardware
  // debugging.
  void test_patterns();

  // Turn on a specific LED with a color and brightness
  void set_color(uint8_t led, uint8_t intensity, color_t color);

  // Like set_color, but doesn't explode with positions out of range
  bool set_color_if_in_range(uint8_t led, uint8_t intensity, color_t color);

  // Color data type
  static color_t color(uint8_t r, uint8_t g, uint8_t b);

  // Returns primary hue colors
  static color_t color_hue(uint8_t h);

  typedef enum {
    RB_RED = 0,
    RB_ORANGE,
    RB_YELLOW,
    RB_GREEN,
    RB_BLUE,
    RB_INDIGO,
    RB_VIOLET
  };
  enum {
    RB_FIRST = RB_RED,
    RB_LAST = RB_VIOLET,
    RB_COUNT = RB_LAST + 1
  };
  static color_t rainbow_color(uint16_t color);

  // Given an int value, returns a "max" color (one with R/G/B each set to
  // 0 or 255, except for black and white). The mapping is arbitary but
  // deterministic.
  static color_t max_color(uint16_t color);

  // Make all LEDs the same color starting at specified beginning LED
  void fill_color(uint8_t begin, uint8_t count, uint8_t intensity,
                  color_t color);
  void fill_random_max(uint8_t begin, uint8_t count, uint8_t intensity);

  void fill_sequence(uint16_t sequence, uint8_t span_size, uint8_t intensity,
		     color_t (*sequence_func)(uint16_t sequence));
  void fill_sequence(uint8_t begin, uint8_t count,
		     uint16_t sequence, uint8_t span_size, uint8_t intensity,
		     color_t (*sequence_func)(uint16_t sequence));

  void fade_in(uint8_t delay_msec);
  void fade_out(uint8_t delay_msec);

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
