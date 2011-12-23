/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <Meteorite.h>

Meteorite::Meteorite(G35& g35)
  : LightProgram(g35),
    position_(g35_.get_last_light() + TAIL) {}

uint32_t Meteorite::Do() {
  if (position_ == static_cast<int16_t>(g35_.get_last_light()) + TAIL) {
    position_ = 0;
    uint8_t r, g, b;
    r = rand() > (RAND_MAX / 2) ? 15 : 0;
    g = rand() > (RAND_MAX / 2) ? 15 : 0;
    b = rand() > (RAND_MAX / 2) ? 15 : 0;
    if (r == 0 && g == 0 && b == 0) {
      r = 15;
      g = 15;
      b = 15;
    }
    d_ = rand() % bulb_frame_ + 5;
    colors_[0] = COLOR(r, g, b);
    colors_[1] = COLOR(r * 3 / 4, g * 3 / 4, b * 3 / 4);
    colors_[2] = COLOR(r * 2 / 4, g * 2 / 4, b * 2 / 4);
    colors_[3] = COLOR(r * 1 / 4, g * 1 / 4, b * 1 / 4);
    colors_[4] = COLOR(r * 0 / 4, g * 0 / 4, b * 0 / 4);
  }

  for (int i = 0; i < TAIL; ++i) {
    int pos = position_ - i;
    g35_.set_color_if_in_range(pos, 255, colors_[i]);
  }
  ++position_;
  return d_;
}
