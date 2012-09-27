/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2012 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <PumpkinChase.h>

PumpkinChase::PumpkinChase(G35& g35)
  : LightProgram(g35),
    count_(1),
    sequence_(0) {}

uint32_t PumpkinChase::Do() {
  g35_.fill_sequence(0, count_, sequence_, 5, 255, orange_green);
  if (count_ < light_count_) {
    ++count_;
  } else {
    ++sequence_;
  }
  return bulb_frame_;
}

// static
color_t PumpkinChase::orange_green(uint16_t sequence) {
  uint16_t s = sequence % 6;
  if (s == 2) {
    return COLOR_ORANGE;
  }
  if (s == 5) {
    return COLOR_GREEN;
  }
  return COLOR_BLACK;
}
