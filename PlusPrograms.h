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

#define PLUS_PROGRAM_COUNT (8)

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
    const int PULSE_RATE = 2;  // Above 1 is glitchy but IMO more attractive.
    color = G35::max_color((sequence + PHASE) / (PHASE * 2));
    intensity = abs(PHASE - (int)(sequence * PULSE_RATE) % (PHASE + PHASE));
    return true;
  }

 private:
  uint8_t count_;
  uint16_t sequence_;
};

// An Orbiter doesn't know about string length. Its coordinate system is
// [0.0, 1.0], and it's the caller's job to scale that to real-world
// values.
class Orbiter {
 public:
 Orbiter()
   : radius_((float)rand() / (float)RAND_MAX),
    angle_(2 * PI * (float)rand() / (float)RAND_MAX),
    d_angle_(0),
    x_(0),
    color_(G35::max_color(rand())) {
    const float MIN_ACTION = 0.005;
    while (d_angle_ < MIN_ACTION && d_angle_ > -MIN_ACTION) {
      d_angle_ = -0.02 + 0.04 * (float)rand() / (float)RAND_MAX;
    }
  }

 Orbiter(float radius, float d_angle)
   : radius_(radius),
    angle_(0),
    d_angle_(d_angle),
    x_(0),
    color_(G35::max_color(rand())) {
  }

  void Do() {
    x_ = sin(angle_) * radius_;
    angle_ += d_angle_;
  }

  float x() { return x_; }
  uint8_t x_local(uint8_t range, uint8_t center) {
    return ((uint8_t)(x_ * range + center)) % range;
  }
  color_t color() { return color_; }

 private:
  float radius_, angle_, d_angle_;
  float x_;
  color_t color_;
};

class Orbit : public LightProgram {
 public:
 Orbit(G35& g35)
   : LightProgram(g35),
    should_erase_(true),
    count_(MAX_OBJECTS),
    light_count_(g35_.get_light_count()) {
  }

  uint32_t Do() {
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

 protected:
 Orbit(G35& g35, bool should_erase)
   : LightProgram(g35),
    should_erase_(should_erase),
    count_(MAX_OBJECTS),
    light_count_(g35_.get_light_count()) {
  }

 private:
  enum { MAX_OBJECTS = 10 };
  bool should_erase_;
  uint8_t count_;
  int16_t last_light_shifted_;
  int8_t light_count_;
  Orbiter orbiter_[MAX_OBJECTS];
  uint8_t orbiter_center_[MAX_OBJECTS];
  uint8_t last_x_[MAX_OBJECTS];

  void set_centers() {
    for (int i = 0; i < count_; ++i) {
      orbiter_center_[i] = rand() % light_count_;
    }
  }
};

class OrbitSmudge : public Orbit {
 public:
 OrbitSmudge(G35& g35) : Orbit(g35, false) {}
};

class Cylon : public LightProgram {
 public:
 Cylon(G35& g35) : LightProgram(g35), orbiter_(0.5, 0.01), last_x_(0) {}

  uint32_t Do() {
    orbiter_.Do();
    uint8_t x = orbiter_.x_local(light_count_, light_count_ >> 1);

    if (last_x_ != x) {
      g35_.set_color(last_x_, G35::MAX_INTENSITY, COLOR_BLACK);
      last_x_ = x;
    }
    g35_.set_color(x, G35::MAX_INTENSITY, orbiter_.color());

    return bulb_frame_ >> 1;
  }

 private:
  Orbiter orbiter_;
  uint8_t last_x_;
};

// Stereo was inspired by SparkFun's "FAKE MUSIC!" demo for their
// Bargraph Breakout board. Thanks, guys!
class Stereo : public LightProgram {
 public:
 Stereo(G35& g35) : LightProgram(g35), light_count_(g35_.get_light_count()),
    half_light_count_((float)light_count_ / 2.0),
    level0_(half_light_count_ * 0.5),
    level1_(half_light_count_ * 0.1666),
    level2_(half_light_count_ * 0.1666),
    level3_(half_light_count_ * 0.1666),
    step_(0), peak_(0) {
    g35_.fill_color(0, light_count_, G35::MAX_INTENSITY, COLOR_BLACK);
  }

  uint32_t Do() {
    float wave = level0_ + sin(step_) * level1_ + sin(step_ * .7) * level2_ +
      sin(step_ * .3) * level3_;
    if (wave > peak_) {
      peak_ = wave;
    } else {
      peak_ *= 0.99;
    }
    uint8_t i = wave;
    while (i--) {
      g35_.set_color(i, G35::MAX_INTENSITY, COLOR_GREEN);
      g35_.set_color(light_count_ - i, G35::MAX_INTENSITY, COLOR_GREEN);
    }
    uint8_t halfway = g35_.get_halfway_point();
    uint8_t peak_i = peak_;
    for (i = wave; i < halfway; ++i) {
      uint8_t color = i == peak_i ? COLOR_RED : COLOR_BLACK;
      g35_.set_color(i, G35::MAX_INTENSITY, color);
      g35_.set_color(light_count_ - i, G35::MAX_INTENSITY, color);
    }
    step_ += 0.4;
    return bulb_frame_;
  }

 private:
  const uint8_t light_count_;
  const float half_light_count_;
  const float level0_, level1_, level2_, level3_;
  float step_, peak_;
};

class Worm {
 public:
 Worm() : head_(0), tail_(0), speed_(0), head_dir_(0), tail_dir_(0),
    is_stretching_(true), color_(G35::max_color(rand())) {
    const float MIN = 0.1;
    const float MAX = 0.6;
    while (speed_ < MIN) {
      speed_ = MAX * ((float)rand() / (float)RAND_MAX);
    }
    head_dir_ = tail_dir_ = speed_;
  }

  void Do(G35& g35) {
    if (is_stretching_) {
      g35.set_color(head_, G35::MAX_INTENSITY, color_);
      head_ += head_dir_;
    } else {
      g35.set_color(tail_, G35::MAX_INTENSITY, COLOR_BLACK);
      tail_ += tail_dir_;
    }
    int8_t length = abs(head_ - tail_);
    if (length < UNIT) {
      is_stretching_ = true;
    }
    if (length >= UNIT * 4) {
      is_stretching_ = false;
    }
    if (head_ <= 0) {
      head_ = 0;
      head_dir_ = speed_;
    }
    uint8_t light_count = g35.get_light_count();
    if (head_ >= light_count - 1) {
      head_ = light_count - 1;
      head_dir_ = -speed_;
    }
    if (tail_ <= 0) {
      tail_ = 0;
      tail_dir_ = speed_;
    }
    if (tail_ >= light_count - 1) {
      tail_ = light_count - 1;
      tail_dir_ = -speed_;
    }
  }
 private:
  enum { UNIT = 2 };

  float head_, tail_;
  float speed_, head_dir_, tail_dir_;
  bool is_stretching_;
  color_t color_;
};

class Inchworm : public LightProgram {
 public:
 Inchworm(G35& g35) : LightProgram(g35), count_(0), next_worm_(0) {
    g35_.fill_color(0, light_count_, G35::MAX_INTENSITY, COLOR_BLACK);
  }

  uint32_t Do() {
    for (int i = 0; i < count_; ++i) {
      worms_[i].Do(g35_);
    }
    if (count_ < 6 && millis() > next_worm_) {
      ++count_;
      next_worm_ = millis() + 2000 + 1000 * count_;
    }
    return bulb_frame_;
  }
 private:
  uint8_t count_;
  uint32_t next_worm_;
  Worm worms_[6];
};

#endif  // INCLUDE_G35_PLUS_PROGRAMS_H
