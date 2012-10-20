/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2012 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_PROGRAMS_SPOOKY_FLICKER_H
#define INCLUDE_G35_PROGRAMS_SPOOKY_FLICKER_H

#include <LightProgram.h>

class SpookyFlicker : public LightProgram {
 public:
  SpookyFlicker(G35& g35);
  ~SpookyFlicker();

  uint32_t Do();

 private:
  uint8_t *intensities_;
  int8_t *deltas_;
};

#endif  // INCLUDE_G35_PROGRAMS_SPOOKY_FLICKER_H
