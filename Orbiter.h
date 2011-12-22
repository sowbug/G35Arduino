/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_ORBITER_H
#define INCLUDE_G35_ORBITER_H

#include <G35.h>

// An Orbiter doesn't know about string length. Its coordinate system is
// [0.0, 1.0], and it's the caller's job to scale that to real-world
// values.
class Orbiter {
 public:
  Orbiter();
  Orbiter(float radius, float d_angle);
  void Do();
  float x();
  uint8_t x_local(uint8_t range, uint8_t center);
  color_t color();

 private:
  float radius_, angle_, d_angle_;
  float x_;
  color_t color_;
};

#endif  // INCLUDE_G35_ORBITER_H
