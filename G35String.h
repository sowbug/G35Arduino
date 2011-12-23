/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  Original version by Paul Martis (http://www.digitalmisery.com). See
  README for complete attributions.
*/

#ifndef INCLUDE_G35_STRING_H
#define INCLUDE_G35_STRING_H

#include <G35.h>

class G35String : public G35 {
 public:
  // |pin|: the Arduino pin driving this string's data line.
  // |light_count|: the number of visible, physical bulbs on the string.
  // |physical_light_count|: the number of physical bulbs on the string.
  // |bulb_zero|: the index of the first bulb (almost always zero).
  // |is_forward|: true if the closest bulb to the plug has the lowest index.
  G35String(uint8_t pin, uint8_t light_count, uint8_t physical_light_count,
            uint8_t bulb_zero, bool is_forward);
  G35String(uint8_t pin, uint8_t light_count);

  // Implementation of G35 interface.
  virtual uint16_t get_light_count() { return light_count_; }
  void set_color(uint8_t led, uint8_t intensity, color_t color);

  // Initialize lights by giving them each an address.
  void enumerate();

  // Displays known-good patterns. Useful to prevent insanity during hardware
  // debugging.
  void do_test_patterns();

 protected:
  virtual uint8_t get_max_intensity() { return MAX_INTENSITY; }
  virtual uint8_t get_broadcast_bulb() { return BROADCAST_BULB; }

 private:
  uint8_t pin_;
  uint8_t physical_light_count_;
  uint8_t bulb_zero_;
  bool is_forward_;

  enum {
    MAX_INTENSITY = 0xcc,
    BROADCAST_BULB = 63,
  };

  // Initialize lights by giving them each an address. enumerate_forward()
  // numbers the bulb closest to the controller 0, and enumerate_reverse()
  // numbers the farthest bulb 0.
  void enumerate(bool reverse);
  void enumerate_forward();
  void enumerate_reverse();

  // Low-level one-wire protocol commands
  void begin();
  void one();
  void zero();
  void end();
};

#endif  // INCLUDE_G35_STRING_H
