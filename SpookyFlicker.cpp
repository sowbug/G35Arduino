/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <SpookyFlicker.h>

SpookyFlicker::SpookyFlicker(G35& g35) : LightProgram(g35) {
  intensities_ = static_cast<uint8_t*>(malloc(light_count_ * sizeof(uint8_t)));
  deltas_ = static_cast<int8_t*>(malloc(light_count_ * sizeof(int8_t)));
  for (uint8_t i = 0; i < light_count_; ++i) {
    intensities_[i] = rand();
    deltas_[i] = rand() % 5 - 2;
  }
  g35_.fill_color(0, light_count_, 255, COLOR_BLACK);
}

SpookyFlicker::~SpookyFlicker() {
  free(intensities_);
  free(deltas_);
}

uint32_t SpookyFlicker::Do() {
  for (int i = 0; i < light_count_; ++i) {
    g35_.set_color(i, intensities_[i], COLOR_ORANGE);
    intensities_[i] += deltas_[i];
  }
  return 10;
}
