/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <Eyes.h>

Eyes::Eyes(G35& g35) : LightProgram(g35), count_(0), next_eye_(0) {
  g35_.fill_color(0, light_count_, 255, COLOR_BLACK);

  while (3 * EYE_COUNT >= light_count_) {
    // There isn't enough room on the light string for this many eyes.
  }

  for (int i = 0; i < EYE_COUNT; ++i) {
    uint16_t new_position;
    do {
      new_position = rand() % (light_count_ - 1);
      for (int j = 0; j < i; ++j) {
        uint16_t occupied_position = eyes_[j].get_position();
        if (new_position >= occupied_position - 1 &&
            new_position <= occupied_position + 1) {
          new_position = light_count_;
          break;
        }
      }
    } while (new_position == light_count_);
    eyes_[i].set_position(new_position);
  }
}

uint32_t Eyes::Do() {
  for (int i = 0; i < count_; ++i) {
    eyes_[i].Do(g35_);
  }
  if (count_ < EYE_COUNT && millis() > next_eye_) {
    ++count_;
    next_eye_ = millis() + 2000 + 1000 * count_;
  }
  return bulb_frame_;
}
