/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <Stereo.h>

Stereo::Stereo(G35& g35) : LightProgram(g35),
                           light_count_(g35_.get_light_count()),
                           half_light_count_((float)light_count_ / 2.0),
                           level0_(half_light_count_ * 0.5),
                           level1_(half_light_count_ * 0.1666),
                           level2_(half_light_count_ * 0.1666),
                           level3_(half_light_count_ * 0.1666),
                           step_(0), peak_(0) {
  g35_.fill_color(0, light_count_, 255, COLOR_BLACK);
}

uint32_t Stereo::Do() {
  float wave = level0_ +
    sin(step_) * level1_ +
    sin(step_ * .7) * level2_ +
    sin(step_ * .3) * level3_;
  if (wave > peak_) {
    peak_ = wave;
  } else {
    peak_ *= 0.99;
  }
  uint8_t i = wave;
  while (i--) {
    g35_.set_color(i, 255, COLOR_GREEN);
    g35_.set_color(light_count_ - i, 255, COLOR_GREEN);
  }
  uint8_t halfway = g35_.get_halfway_point();
  uint8_t peak_i = peak_;
  for (i = wave; i < halfway; ++i) {
    uint8_t color = i == peak_i ? COLOR_RED : COLOR_BLACK;
    g35_.set_color(i, 255, color);
    g35_.set_color(light_count_ - i, 255, color);
  }
  step_ += 0.4;
  return bulb_frame_;
}
