/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  Original version by Paul Martis (http://www.digitalmisery.com). See
  README for complete attributions.
*/

#include <G35StringGroup.h>

G35StringGroup::G35StringGroup()
: string_count_(0) {
  light_count_ = 0;
}

void G35StringGroup::AddString(G35* g35) {
  if (string_count_ == MAX_STRINGS) {
    return;
  }
  uint16_t light_count = g35->get_light_count();
  string_offsets_[string_count_] =
    string_count_ == 0 ?
    light_count :
    string_offsets_[string_count_ - 1] + light_count;
  strings_[string_count_] = g35;
  ++string_count_;
  light_count_ += light_count;
}

uint16_t G35StringGroup::get_light_count() {
  return light_count_;
}

void G35StringGroup::set_color(uint8_t bulb, uint8_t intensity, color_t color) {
  uint8_t string = 0;
  while (bulb >= string_offsets_[string] && string < string_count_) {
    bulb -= string_offsets_[string++];
  }
  if (string < string_count_) {
    strings_[string]->set_color(bulb, intensity, color);
  } else {
    // A program is misbehaving.
#if 0
    Serial.println("out of bounds");
#endif
  }
}

void G35StringGroup::broadcast_intensity(uint8_t intensity) {
  for (uint8_t i = 0; i < string_count_; ++i) {
    strings_[i]->broadcast_intensity(intensity);
  }
}

uint8_t G35StringGroup::get_broadcast_bulb() {
  return 0;  // In this implementation, shouldn't ever be called.
}
