/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2012 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.
  Programs referenced in MEOPrograms by Mark Ortiz <github.com/MarkEMarkEMark>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_MEO_PROGRAMS_H
#define INCLUDE_G35_MEO_PROGRAMS_H

#include <LightProgram.h>
/* #include <Chasing.h> */
/* #include <ColorPhasing.h> */
/* #include <Dither.h> */
/* #include <Oscillate.h> */
 #include <Rainbow.h> 
/* #include <RandomStrobe.h> */
/* #include <SimplexNoise.h> */
/* #include <SineWave.h> */
/* #include <Whites.h> */

class MEOProgramGroup : public LightProgramGroup {
 public:
  enum { ProgramCount = 9 };

  virtual LightProgram* CreateProgram(G35& lights, uint8_t program_index);
};

#endif  // INCLUDE_G35_MEO_PROGRAMS_H
