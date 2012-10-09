/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_PROGRAMS_EYES_H
#define INCLUDE_G35_PROGRAMS_EYES_H

#include <LightProgram.h>

class Eye {
 public:
 Eye()
   : color_(COLOR_BLACK), position_(0), next_blink_(0), state_(INIT) {
    switch (rand() % 4) {
    case 0: color_ = COLOR_ORANGE; break;
    case 1: color_ = COLOR_RED; break;
    case 2: color_ = COLOR_INDIGO; break;
    case 3: color_ = COLOR_GREEN; break;
    }
  }

  uint16_t get_position() { return position_; }
  void set_position(uint16_t value) { position_ = value; }

  void Do(G35& g35) {
    if (state_ == INIT) {
      state_ = WATCHING;
    }
    uint32_t now = millis();
    if (next_blink_ < now) {
      if (state_ == BLINKING) {
        g35.fill_color(position_, 2, G35::MAX_INTENSITY, color_);
        next_blink_ = now + 3000 + (rand() % 3000);
        state_ = WATCHING;
      } else {
        g35.fill_color(position_, 2, G35::MAX_INTENSITY, COLOR_BLACK); 
        next_blink_ = now + 100;
        state_ = BLINKING;
      }
    }
  }

 private:
  enum {
    INIT, WATCHING, BLINKING
  };

  color_t color_;
  uint16_t position_;
  uint32_t next_blink_;
  uint8_t state_;
};

class Eyes : public LightProgram {
 public:
  Eyes(G35& g35);
  uint32_t Do();

 private:
  enum { EYE_COUNT = 10 };

  uint8_t count_;
  uint32_t next_eye_;
  Eye eyes_[EYE_COUNT];
};

#endif  // INCLUDE_G35_PROGRAMS_EYES_H
