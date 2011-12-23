/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <PlusPrograms.h>

LightProgram* PlusProgramGroup::CreateProgram(G35& lights,
                                              uint8_t program_index) {
  switch (program_index % ProgramCount) {
  case 0: return new Meteorite(lights);
  case 1: return new Twinkle(lights);
  case 2: return new RedGreenChase(lights);
  case 3: return new Pulse(lights);
  case 4: return new Orbit(lights);
  case 5: return new OrbitSmudge(lights);
  case 6: return new Cylon(lights);
  case 7: return new Stereo(lights);
  case 8: return new Inchworm(lights);
  }

  // not reached
  return NULL;
}
