/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <StockPrograms.h>

SteadyWhite::SteadyWhite(G35& g35)
  : LightProgram(g35), intensity_(0) {
  g35_.fill_color(0, light_count_, 0, COLOR_WHITE);
}

uint32_t SteadyWhite::Do() {
  if (intensity_ <= G35::MAX_INTENSITY) {
    g35_.broadcast_intensity(intensity_++);
    return bulb_frame_;
  }
  return 1000;
}

CrossOverWave::CrossOverWave(G35& g35)
  : LightProgram(g35), x_(light_count_) {}

uint32_t CrossOverWave::Do() {
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

ForwardWave::ForwardWave(G35& g35)
  : LightProgram(g35), x_(light_count_) {}

uint32_t ForwardWave::Do() {
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

ChasingRainbow::ChasingRainbow(G35& g35)
  : LightProgram(g35), count_(1), sequence_(0) {}

uint32_t ChasingRainbow::Do() {
  g35_.fill_sequence(0, count_, sequence_, 1, G35::MAX_INTENSITY,
                     G35::rainbow_color);
  if (count_ < light_count_) {
    ++count_;
  } else {
    ++sequence_;
  }
  return bulb_frame_;
}

AlternateDirectionalWave::AlternateDirectionalWave(G35& g35)
  : LightProgram(g35), x_(0), x_target_(light_count_),
    x_other_target_(-1), direction_(1), color_(G35::max_color(rand())) {}

uint32_t AlternateDirectionalWave::Do() {
  g35_.set_color(x_, G35::MAX_INTENSITY, color_);
  x_ += direction_;

  if (x_ == x_target_) {
    direction_ = -direction_;
    x_ += direction_;
    int16_t t = x_target_;
    x_target_ = x_other_target_;
    x_other_target_ = t;
    color_t old_color = color_;
    do {
      color_ = G35::max_color(rand());
    } while (old_color == color_);
    return 1000;
  }
  return bulb_frame_;
}

FadeInFadeOutSolidColors::FadeInFadeOutSolidColors(G35& g35)
  : LightProgram(g35), color_(0), intensity_(0) {}

uint32_t FadeInFadeOutSolidColors::Do() {
  if (intensity_ == 0) {
    color_t new_color = color_;
    do {
      color_ = G35::max_color(rand());
    } while (new_color == color_);

    g35_.broadcast_intensity(0);
    g35_.fill_color(0, light_count_, 0, color_);
    d_intensity_ = 1;
  }
  if (intensity_ == G35::MAX_INTENSITY) {
    d_intensity_ = -1;
  }
  intensity_ += d_intensity_;
  g35_.broadcast_intensity(intensity_);

  return 10;
}

BidirectionalWave::BidirectionalWave(G35& g35)
  : LightProgram(g35), x_(g35_.get_halfway_point()) {}

uint32_t BidirectionalWave::Do() {
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

ChasingSolidColors::ChasingSolidColors(G35& g35)
  : LightProgram(g35), count_(1), sequence_(0) {}

uint32_t ChasingSolidColors::Do() {
  g35_.fill_sequence(0, count_, sequence_, 5, G35::MAX_INTENSITY,
                     G35::max_color);
  if (count_ < light_count_) {
    ++count_;
  } else {
    ++sequence_;
  }
  return bulb_frame_;
}

FadeInFadeOutMultiColors::FadeInFadeOutMultiColors(G35& g35)
  : LightProgram(g35), state_(0), intensity_(0) {}

uint32_t FadeInFadeOutMultiColors::Do() {
  switch (state_) {
  case 0:
    if (intensity_++ == 0) {
      // We mask off the last two bits so that the color segments are aligned
      // from scene to scene.
      g35_.fill_sequence(rand() & 0x7ffc, 4, 0, G35::max_color);
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
  g35_.broadcast_intensity(intensity_);

  return 10;
}

ChasingTwoColors::ChasingTwoColors(G35& g35)
  : LightProgram(g35), sequence_(0) {}

uint32_t ChasingTwoColors::Do() {
  g35_.fill_sequence(sequence_, light_count_ / 2,
                     G35::MAX_INTENSITY, G35::rainbow_color);
  sequence_ += light_count_ / 2;
  return 500;
}

RandomSparkling::RandomSparkling(G35& g35)
  : LightProgram(g35), state_(1) {}

uint32_t RandomSparkling::Do() {
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

ChasingMultiColors::ChasingMultiColors(G35& g35)
  : LightProgram(g35), count_(1), sequence_(0) {}

uint32_t ChasingMultiColors::Do() {
  g35_.fill_sequence(0, count_, sequence_, 1, G35::MAX_INTENSITY,
                     G35::max_color);
  if (count_ < light_count_) {
    ++count_;
  } else {
    ++sequence_;
  }
  return bulb_frame_ * 6;
}

ChasingWhiteRedBlue::ChasingWhiteRedBlue(G35& g35)
  : LightProgram(g35), count_(1), sequence_(0) {}

uint32_t ChasingWhiteRedBlue::Do() {
  g35_.fill_sequence(0, count_, sequence_, 3, G35::MAX_INTENSITY, red_white_blue);
  if (count_ < light_count_) {
    ++count_;
  } else {
    ++sequence_;
  }
  return bulb_frame_;
}

// static
color_t ChasingWhiteRedBlue::red_white_blue(uint16_t sequence) {
  sequence = sequence % 3;
  if (sequence == 0) {
    return COLOR_RED;
  }
  if (sequence == 1) {
    return COLOR_WHITE;
  }
  return COLOR_BLUE;
}

LightProgram* StockProgramGroup::CreateProgram(G35& lights,
                                               uint8_t program_index) {
  switch (program_index % ProgramCount) {
  case 0: return new CrossOverWave(lights);
  case 1: return new ForwardWave(lights);
  case 2: return new ChasingRainbow(lights);
  case 3: return new AlternateDirectionalWave(lights);
  case 4: return new FadeInFadeOutSolidColors(lights);
  case 5: return new BidirectionalWave(lights);
  case 6: return new ChasingSolidColors(lights);
  case 7: return new FadeInFadeOutMultiColors(lights);
  case 8: return new ChasingTwoColors(lights);
  case 9: return new RandomSparkling(lights);
  case 10: return new ChasingMultiColors(lights);
  case 11: return new ChasingWhiteRedBlue(lights);
  }

  // not reached
  return NULL;
}
