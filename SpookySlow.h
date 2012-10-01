/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2012 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_PROGRAMS_SPOOKY_SLOW_H
#define INCLUDE_G35_PROGRAMS_SPOOKY_SLOW_H

#include <LightProgram.h>

class SpookySlow : public LightProgram {
 public:
  SpookySlow(G35& g35);
  uint32_t Do();

 private:
  uint16_t remaining_;
};

#endif  // INCLUDE_G35_PROGRAMS_SPOOKY_SLOW_H
