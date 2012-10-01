/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <SpookySlow.h>

SpookySlow::SpookySlow(G35& g35) : LightProgram(g35), remaining_(0) {
}

uint32_t SpookySlow::Do() {
  if (remaining_ == 0) {
    remaining_ = rand() % (light_count_ >> 3);
    g35_.fill_color(0, light_count_, 255, COLOR_BLACK);
  }
  if (remaining_-- > 2) {
    g35_.set_color(rand() % light_count_, G35::MAX_INTENSITY,
                   (rand() & 1) ? COLOR_ORANGE : COLOR_PALE_ORANGE);
  }
  return 1000;
}
