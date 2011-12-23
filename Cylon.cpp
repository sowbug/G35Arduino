/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <Cylon.h>

Cylon::Cylon(G35& g35) : LightProgram(g35), orbiter_(0.5, 0.01), last_x_(0) {}

uint32_t Cylon::Do() {
  orbiter_.Do();
  uint8_t x = orbiter_.x_local(light_count_, light_count_ >> 1);

  if (last_x_ != x) {
    g35_.set_color(last_x_, 255, COLOR_BLACK);
    last_x_ = x;
  }
  g35_.set_color(x, 255, orbiter_.color());

  return bulb_frame_ >> 1;
}
