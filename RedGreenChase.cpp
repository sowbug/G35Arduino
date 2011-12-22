/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <RedGreenChase.h>

RedGreenChase::RedGreenChase(G35& g35)
  : LightProgram(g35),
    count_(1),
    sequence_(0) {}

uint32_t RedGreenChase::Do() {
  g35_.fill_sequence(0, count_, sequence_, 5, G35::MAX_INTENSITY, red_green);
  if (count_ < light_count_) {
    ++count_;
  } else {
    ++sequence_;
  }
  return bulb_frame_;
}

// static
color_t RedGreenChase::red_green(uint16_t sequence) {
  return sequence % 2 ? COLOR_RED : COLOR_GREEN;
}
