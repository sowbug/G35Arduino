/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_PROGRAMS_WORM_H
#define INCLUDE_G35_PROGRAMS_WORM_H

#include <G35.h>

class Worm {
 public:
  Worm();
  void Do(G35& g35);

 private:
  enum { UNIT = 2 };

  float head_, tail_;
  float speed_, head_dir_, tail_dir_;
  bool is_stretching_;
  color_t color_;
};

#endif  // INCLUDE_G35_PROGRAMS_WORM_H
