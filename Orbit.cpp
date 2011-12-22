/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <Orbit.h>

Orbit::Orbit(G35& g35)
  : LightProgram(g35),
    should_erase_(true),
    count_(MAX_OBJECTS),
    light_count_(g35_.get_light_count()) {}

uint32_t  Orbit::Do() {
  for (int i = 0; i < count_; ++i) {
    Orbiter *o = &orbiter_[i];
    o->Do();
    uint8_t x = o->x_local(light_count_, orbiter_center_[i]);

    if (should_erase_ && last_x_[i] != x) {
      g35_.set_color(last_x_[i], G35::MAX_INTENSITY, COLOR_BLACK);
      last_x_[i] = x;
    }
    g35_.set_color(x, G35::MAX_INTENSITY, o->color());
  }
  return bulb_frame_ >> 1;
}

Orbit::Orbit(G35& g35, bool should_erase)
  : LightProgram(g35),
    should_erase_(should_erase),
    count_(MAX_OBJECTS),
    light_count_(g35_.get_light_count()) {}

void Orbit::set_centers() {
  for (int i = 0; i < count_; ++i) {
    orbiter_center_[i] = rand() % light_count_;
  }
}

OrbitSmudge::OrbitSmudge(G35& g35) : Orbit(g35, false) {}
