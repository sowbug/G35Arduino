/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <Inchworm.h>

Inchworm::Inchworm(G35& g35) : LightProgram(g35), count_(0), next_worm_(0) {
  g35_.fill_color(0, light_count_, 255, COLOR_BLACK);
}

uint32_t Inchworm::Do() {
  for (int i = 0; i < count_; ++i) {
    worms_[i].Do(g35_);
  }
  if (count_ < 6 && millis() > next_worm_) {
    ++count_;
    next_worm_ = millis() + 2000 + 1000 * count_;
  }
  return bulb_frame_;
}
