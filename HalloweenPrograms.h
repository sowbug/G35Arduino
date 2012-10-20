/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2012 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_HALLOWEEN_PROGRAMS_H
#define INCLUDE_G35_HALLOWEEN_PROGRAMS_H

#include <Creepers.h>
#include <SpookyFlicker.h>
#include <Eyes.h>
#include <LightProgram.h>
#include <PumpkinChase.h>
#include <SpookySlow.h>

class HalloweenProgramGroup : public LightProgramGroup {
 public:
  enum { ProgramCount = 5 };

  virtual LightProgram* CreateProgram(G35& lights, uint8_t program_index);
};

#endif  // INCLUDE_G35_HALLOWEEN_PROGRAMS_H
