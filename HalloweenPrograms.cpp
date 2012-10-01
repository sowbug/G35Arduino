/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <HalloweenPrograms.h>

LightProgram* HalloweenProgramGroup::CreateProgram(G35& lights,
                                                   uint8_t program_index) {
  switch (program_index % ProgramCount) {
  case 0: return new Creepers(lights);
  case 1: return new PumpkinChase(lights);
  case 2: return new SpookySlow(lights);
  }

  // not reached
  return NULL;
}
