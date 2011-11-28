/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_STOCK_PROGRAMS_H
#define INCLUDE_G35_STOCK_PROGRAMS_H

#include <LightProgram.h>

class SteadyWhite : public LightProgram {
 public:
 SteadyWhite(G35& g35)
   : LightProgram(g35), intensity_(0) {
    g35_.fill_color(0, light_count_, 0, COLOR_WHITE);
  }

  uint32_t Do() {
    if (intensity_ <= G35::MAX_INTENSITY) {
      g35_.set_color(G35::BROADCAST_BULB, intensity_++, COLOR_BLACK);
      return bulb_frame_;
    }
    return 1000;
  }
 private:
  uint8_t intensity_;
};

class CrossOverWave : public LightProgram {
 public:
 CrossOverWave(G35& g35)
   : LightProgram(g35), x_(light_count_) {}

  uint32_t Do() {
    if (x_ == light_count_) {
      x_ = 0;
      color_a_ = 0;
      color_b_ = 0;
      while (color_a_ == color_b_) {
        color_a_ = G35::max_color(rand());
        color_b_ = G35::max_color(rand());
      }
    }
    g35_.set_color(x_, G35::MAX_INTENSITY, color_a_);
    g35_.set_color(g35_.get_last_light() - x_, G35::MAX_INTENSITY, color_b_);
    ++x_;
    return bulb_frame_;
  }
 private:
  uint8_t x_;
  color_t color_a_;
  color_t color_b_;
};

class ForwardWave : public LightProgram {
 public:
 ForwardWave(G35& g35)
   : LightProgram(g35), x_(light_count_) {}

  uint32_t Do() {
    if (x_ == light_count_) {
      x_ = 0;
      color_t old_color = color_;
      do {
        color_ = G35::max_color(rand());
      } while (old_color == color_);
    }
    g35_.set_color(x_, G35::MAX_INTENSITY, color_);
    ++x_;
    return bulb_frame_;
  }
 private:
  uint8_t x_;
  color_t color_;
};

class ChasingRainbow : public LightProgram {
 public:
 ChasingRainbow(G35& g35)
   : LightProgram(g35), count_(1), sequence_(0) {}

  uint32_t Do() {
    g35_.fill_sequence(0, count_, sequence_, 1, G35::MAX_INTENSITY,
                       G35::rainbow_color);
    if (count_ < light_count_) {
      ++count_;
    } else {
      ++sequence_;
    }
    return bulb_frame_;
  }
 private:
  uint8_t count_;
  uint16_t sequence_;
};

class AlternateDirectionalWave : public LightProgram {
 public:
 AlternateDirectionalWave(G35& g35)
   : LightProgram(g35), x_(light_count_),
    direction_(-1) {}

  uint32_t Do() {
    bool hit_end = false;
    if (x_ == light_count_) {
      x_ = 0;
      hit_end = true;
    } else if (x_ == -1) {
      x_ = light_count_ - 1;
      hit_end = true;
    }
    if (hit_end) {
      direction_ = -direction_;
      color_t old_color = color_;
      do {
        color_ = G35::max_color(rand());
      } while (old_color == color_);
      return 500;
    } else {
      g35_.set_color(x_, G35::MAX_INTENSITY, color_);
      x_ += direction_;
      return bulb_frame_;
    }
  }
 private:
  uint8_t x_;
  int8_t direction_;
  color_t color_;
};

class FadeInFadeOutSolidColors : public LightProgram {
 public:
 FadeInFadeOutSolidColors(G35& g35)
   : LightProgram(g35), color_(0), intensity_(0) {}

  uint32_t Do() {
    if (intensity_ == 0) {
      color_t new_color = color_;
      do {
	color_ = G35::max_color(rand());
      } while (new_color == color_);

      g35_.set_color(G35::BROADCAST_BULB, 0, COLOR_BLACK);
      g35_.fill_color(0, light_count_, 0, color_);
      d_intensity_ = 1;
    }
    if (intensity_ == G35::MAX_INTENSITY) {
      d_intensity_ = -1;
    }
    intensity_ += d_intensity_;
    g35_.set_color(G35::BROADCAST_BULB, intensity_, COLOR_BLACK);

    return 500 / G35::BROADCAST_BULB;
  }

 private:
  color_t color_;
  uint8_t intensity_;
  int8_t d_intensity_;
};

class BidirectionalWave : public LightProgram {
 public:
 BidirectionalWave(G35& g35)
   : LightProgram(g35), x_(g35_.get_halfway_point()) {}

  uint32_t Do() {
    // With 50 lights, we run into some edge cases because 50 isn't evenly
    // divisible by 4. It's a fairly crazy program to start with, so I'm
    // leaving it like this.
    if (x_ == g35_.get_halfway_point()) {
      x_ = 0;
      do {
        color_a_ = G35::max_color(rand());
        color_b_ = G35::max_color(rand());
      } while (color_a_ == color_b_);
      do {
        color_c_ = G35::max_color(rand());
        color_d_ = G35::max_color(rand());
      } while (color_c_ == color_d_);
    }
    g35_.set_color(x_, G35::MAX_INTENSITY, color_a_);
    g35_.set_color(g35_.get_halfway_point() - 1 - x_, G35::MAX_INTENSITY,
                   color_b_);
    g35_.set_color(g35_.get_halfway_point() + x_, G35::MAX_INTENSITY,
                   color_c_);
    g35_.set_color(g35_.get_last_light() - x_, G35::MAX_INTENSITY, color_d_);
    ++x_;
    return bulb_frame_;
  }
 private:
  uint8_t x_;
  color_t color_a_;
  color_t color_b_;
  color_t color_c_;
  color_t color_d_;
};

class ChasingSolidColors : public LightProgram {
 public:
 ChasingSolidColors(G35& g35)
   : LightProgram(g35), count_(1), sequence_(0) {}

  uint32_t Do() {
    g35_.fill_sequence(0, count_, sequence_, 5, G35::MAX_INTENSITY,
                       G35::max_color);
    if (count_ < light_count_) {
      ++count_;
    } else {
      ++sequence_;
    }
    return bulb_frame_;
  }
 private:
  uint8_t count_;
  uint16_t sequence_;
};

class FadeInFadeOutMultiColors : public LightProgram {
 public:
 FadeInFadeOutMultiColors(G35& g35)
   : LightProgram(g35), state_(0), intensity_(0) {}

  uint32_t Do() {
    switch (state_) {
    case 0:
      if (intensity_++ == 0) {
        g35_.fill_sequence(rand(), 5, 0, G35::max_color);
      }
      if (intensity_ == G35::MAX_INTENSITY) {
        state_ = 1;
      }
      break;
    case 1:
      if (--intensity_ == 0) {
        state_ = 0;
      }
      break;
    }
    g35_.set_color(G35::BROADCAST_BULB, intensity_, COLOR_BLACK);

    return 500 / G35::BROADCAST_BULB;
  }
 private:
  uint8_t state_;
  uint8_t intensity_;
};

class ChasingTwoColors : public LightProgram {
 public:
 ChasingTwoColors(G35& g35)
   : LightProgram(g35), sequence_(0) {}

  uint32_t Do() {
    g35_.fill_sequence(sequence_, light_count_ / 2,
                       G35::MAX_INTENSITY, G35::rainbow_color);
    sequence_ += light_count_ / 2;
    return 500;
  }
 private:
  uint16_t sequence_;
};

class RandomSparkling : public LightProgram {
 public:
 RandomSparkling(G35& g35)
   : LightProgram(g35), state_(1) {}

  uint32_t Do() {
    if (state_++ > 1) {
      state_ = 0;
    }
    if (state_ == 0) {
      g35_.fill_random_max(0, light_count_, G35::MAX_INTENSITY);
      return 1000;
    }
    g35_.fill_color(0, light_count_, G35::MAX_INTENSITY, COLOR_BLACK);
    return 500;
  }
 private:
  color_t color_;
  uint8_t state_;
};

class ChasingMultiColors : public LightProgram {
 public:
 ChasingMultiColors(G35& g35)
   : LightProgram(g35), count_(1), sequence_(0) {}

  uint32_t Do() {
    g35_.fill_sequence(0, count_, sequence_, 1, G35::MAX_INTENSITY,
                       G35::max_color);
    if (count_ < light_count_) {
      ++count_;
    } else {
      ++sequence_;
    }
    return bulb_frame_ * 6;
  }
 private:
  uint8_t count_;
  uint16_t sequence_;
};

class ChasingWhiteRedBlue : public LightProgram {
 public:
 ChasingWhiteRedBlue(G35& g35)
   : LightProgram(g35), count_(1), sequence_(0) {}

  uint32_t Do() {
    g35_.fill_sequence(0, count_, sequence_, 3, G35::MAX_INTENSITY,
                       red_white_blue);
    if (count_ < light_count_) {
      ++count_;
    } else {
      ++sequence_;
    }
    return bulb_frame_;
  }

  static color_t red_white_blue(uint16_t sequence) {
    sequence = sequence % 3;
    if (sequence == 0) {
      return COLOR_RED;
    }
    if (sequence == 1) {
      return COLOR_WHITE;
    }
    return COLOR_BLUE;
  }

 private:
  uint8_t count_;
  uint16_t sequence_;
};

#endif  // INCLUDE_G35_STOCK_PROGRAMS_H
