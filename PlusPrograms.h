/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_PLUS_PROGRAMS_H
#define INCLUDE_G35_PLUS_PROGRAMS_H

#include <LightProgram.h>

class Meteorite : public LightProgram {
 public:
 Meteorite(G35& g35)
   : LightProgram(g35),
    position_(g35_.get_last_light() + TAIL) {}

  uint32_t Do() {
    if (position_ == g35_.get_last_light() + TAIL) {
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
      g35_.set_color_if_in_range(pos, G35::MAX_INTENSITY, colors_[i]);
    }
    ++position_;
    return d_;
  }
 private:
  static const uint8_t TAIL = 5;

  uint8_t d_;
  int16_t position_;
  color_t colors_[TAIL];
};

class Twinkle : public LightProgram {
 public:
 Twinkle(G35& g35)
   : LightProgram(g35) {
    g35_.fill_random_max(0, light_count_, G35::MAX_INTENSITY);
  }

  uint32_t Do() {
    g35_.set_color(rand() % light_count_, G35::MAX_INTENSITY,
                   G35::max_color(rand()));
    return bulb_frame_;
  }
};

class RedGreenChase : public LightProgram {
 public:
 RedGreenChase(G35& g35)
   : LightProgram(g35),
    count_(1),
    sequence_(0) {}

  uint32_t Do() {
    g35_.fill_sequence(0, count_, sequence_, 5, G35::MAX_INTENSITY,
                       red_green);
    if (count_ < light_count_) {
      ++count_;
    } else {
      ++sequence_;
    }
    return bulb_frame_;
  }

  static color_t red_green(uint16_t sequence) {
    return sequence % 2 ? COLOR_RED : COLOR_GREEN;
  }

 private:
  uint8_t count_;
  uint16_t sequence_;
};

class Pulse : public LightProgram {
 public:
 Pulse(G35& g35)
   : LightProgram(g35),
    count_(1),
    sequence_(0) {}

  uint32_t Do() {
    g35_.fill_sequence(0, count_, sequence_, 1, pulser);
    if (count_ < light_count_) {
      ++count_;
    } else {
      ++sequence_;
    }
    return 1;
  }

  static bool pulser(uint16_t sequence, color_t& color, uint8_t& intensity) {
    const int PHASE = 32;
    color = G35::max_color((sequence + PHASE) / (PHASE * 2));
    intensity = abs(PHASE - (int)(sequence) % (PHASE + PHASE));
    return true;
  }

 private:
  uint8_t count_;
  uint16_t sequence_;
};

class Cylon : public LightProgram {
 public:
 Cylon(G35& g35)
   : LightProgram(g35),
    last_light_shifted_((light_count_ << 8) - 1), x_(0), d2x_(-2),
    color_(G35::max_color(rand())) {}

  uint32_t Do() {
    uint8_t old_x_pix = x_ >> 8;
    dx_ += d2x_;
    x_ += dx_;

    bool moving_right = d2x_ > 0;
    bool past_right = old_x_pix >= g35_.get_halfway_point();
    if (moving_right == past_right) {
      d2x_ = -d2x_;
    }

    if (x_ <= 0) {
      x_ = 0;
      dx_ = 0;
    }
    if (x_ > last_light_shifted_) {
      x_ = last_light_shifted_;
      dx_ = 0;
    }

    uint8_t x_pix = x_ >> 8;
    if (old_x_pix != x_pix) {
      g35_.set_color(old_x_pix, G35::MAX_INTENSITY, COLOR_BLACK);
    }
    g35_.set_color(x_pix, G35::MAX_INTENSITY, color_);

    return bulb_frame_ >> 1;
  }

 private:
  int16_t last_light_shifted_;
  int16_t x_;
  int16_t dx_;
  int16_t d2x_;
  color_t color_;
};

class Orbiter {
 public:
 Orbiter()
   : center_(rand()),
    x_(rand()),
    old_x_(0),
    dx_(0),
    d2x_(rand() % 10 - 5),
    color_(G35::max_color(rand())) {
    old_x_ = x_;
    if (d2x_ == 0) {
      d2x_ = 1;
    }
  }

  void Do() {
    old_x_ = x_;
    dx_ += d2x_;
    x_ += dx_;

    bool moving_right = d2x_ > 0;
    bool past_center = x_ >= center_;
    if (moving_right == past_center) {
      d2x_ = -d2x_;
    }

    if (x_ <= 0) {
      x_ = 0;
      dx_ = 0;
    }
  }

  int16_t x() { return x_; }
  int16_t old_x() { return old_x_; }
  color_t color() { return color_; }

 private:
  int16_t center_;
  int16_t x_;
  int16_t old_x_;
  int16_t dx_;
  int16_t d2x_;
  color_t color_;
};

class Orbit : public LightProgram {
 public:
 Orbit(G35& g35, bool should_erase)
   : LightProgram(g35),
    should_erase_(should_erase),
    count_(MAX_OBJECTS),
    last_light_shifted_((light_count_ << 8) - 1),
    light_count_(g35_.get_light_count()) {}

  uint32_t Do() {
    for (int i = 0; i < count_; ++i) {
      Orbiter *o = &orbiter_[i];
      int16_t old_x = (o->old_x() >> 8) % light_count_;
      o->Do();
      int16_t x = (o->x() >> 8) % light_count_;

      if (should_erase_ && old_x != x) {
        g35_.set_color(old_x, G35::MAX_INTENSITY, COLOR_BLACK);
      }
      g35_.set_color(x, G35::MAX_INTENSITY, o->color());
    }
    return bulb_frame_;
  }

 private:
  enum { MAX_OBJECTS = 10 };
  bool should_erase_;
  uint8_t count_;
  int16_t last_light_shifted_;
  int8_t light_count_;
  Orbiter orbiter_[MAX_OBJECTS];
};

#endif  // INCLUDE_G35_PLUS_PROGRAMS_H
