/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_PROGRAMS_METEORITE_H
#define INCLUDE_G35_PROGRAMS_METEORITE_H

#include <LightProgram.h>

class Meteorite : public LightProgram {
 public:
  Meteorite(G35& g35);
  uint32_t Do();

 private:
  static const uint8_t TAIL = 5;

  uint8_t d_;
  int16_t position_;
  color_t colors_[TAIL];
};

#endif  // INCLUDE_G35_PROGRAMS_METEORITE_H
