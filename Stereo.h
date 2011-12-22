/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_PROGRAMS_STEREO_H
#define INCLUDE_G35_PROGRAMS_STEREO_H

#include <LightProgram.h>

// Stereo was inspired by SparkFun's "FAKE MUSIC!" demo for their
// Bargraph Breakout board. Thanks, guys!
class Stereo : public LightProgram {
 public:
  Stereo(G35& g35);
  uint32_t Do();

 private:
  const uint8_t light_count_;
  const float half_light_count_;
  const float level0_, level1_, level2_, level3_;
  float step_, peak_;
};

#endif  // INCLUDE_G35_PROGRAMS_STEREO_H
