/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <Worm.h>

Worm::Worm() : head_(0), tail_(0), speed_(0), head_dir_(0), tail_dir_(0),
               is_stretching_(true), color_(G35::max_color(rand())) {
  const float MIN = 0.1;
  const float MAX = 0.6;
  while (speed_ < MIN) {
    speed_ = MAX * ((float)rand() / (float)RAND_MAX);
  }
  head_dir_ = tail_dir_ = speed_;
}

void Worm::Do(G35& g35) {
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
