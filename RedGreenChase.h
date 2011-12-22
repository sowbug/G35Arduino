/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_PROGRAMS_RED_GREEN_CHASE_H
#define INCLUDE_G35_PROGRAMS_RED_GREEN_CHASE_H

#include <LightProgram.h>

class RedGreenChase : public LightProgram {
 public:
  RedGreenChase(G35& g35);
  uint32_t Do();
  static color_t red_green(uint16_t sequence);

 private:
  uint8_t count_;
  uint16_t sequence_;
};

#endif  // INCLUDE_G35_PROGRAMS_RED_GREEN_CHASE_H
