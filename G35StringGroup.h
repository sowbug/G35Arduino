/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  Original version by Paul Martis (http://www.digitalmisery.com). See
  README for complete attributions.
*/

#ifndef INCLUDE_G35_STRING_GROUP_H
#define INCLUDE_G35_STRING_GROUP_H

#include <G35.h>

// A G35StringGroup takes a set of G35 instances and presents them as a single
// virtual light string of arbitrary length.
//
// A LightProgram that is given a G35StringGroup will then be able to run
// itself over the whole virtual string, without needing to know the details
// of which physical string contains which logical bulb.
class G35StringGroup : public G35 {
 public:
  G35StringGroup();

  void AddString(G35* g35);

  virtual uint16_t get_light_count();

  virtual void set_color(uint8_t bulb, uint8_t intensity, color_t color);
  virtual void broadcast_intensity(uint8_t intensity);

 protected:
  virtual uint8_t get_broadcast_bulb();

 private:
  enum { MAX_STRINGS = 16 };

  uint8_t string_count_;
  G35* strings_[MAX_STRINGS];
  uint16_t string_offsets_[MAX_STRINGS];
};

#endif  // INCLUDE_G35_STRING_GROUP_H
