/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <Orbiter.h>

Orbiter::Orbiter()
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

Orbiter::Orbiter(float radius, float d_angle)
  : radius_(radius),
    angle_(0),
    d_angle_(d_angle),
    x_(0),
    color_(G35::max_color(rand())) {}

void Orbiter::Do() {
  x_ = sin(angle_) * radius_;
  angle_ += d_angle_;
}

float Orbiter::x() {
  return x_;
}

uint8_t Orbiter::x_local(uint8_t range, uint8_t center) {
  return ((uint8_t)(x_ * range + center)) % range;
}

color_t Orbiter::color() {
  return color_;
}
