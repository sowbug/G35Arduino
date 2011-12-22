/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_PROGRAMS_CYLON_H
#define INCLUDE_G35_PROGRAMS_CYLON_H

#include <LightProgram.h>
#include <Orbiter.h>

class Cylon : public LightProgram {
 public:
  Cylon(G35& g35);

  uint32_t Do();

 private:
  Orbiter orbiter_;
  uint8_t last_x_;
};

#endif  // INCLUDE_G35_PROGRAMS_CYLON_H
