/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.

  See README for complete attributions.
*/

#include <Pulse.h>

Pulse::Pulse(G35& g35)
  : LightProgram(g35),
    count_(1),
    sequence_(0) {}

uint32_t Pulse::Do() {
  g35_.fill_sequence(0, count_, sequence_, 1, pulser);
  if (count_ < light_count_) {
    ++count_;
  } else {
    ++sequence_;
  }
  return 1;
}

// static
bool Pulse::pulser(uint16_t sequence, color_t& color, uint8_t& intensity) {
  const int PHASE = 32;
  const int PULSE_RATE = 2;  // Above 1 is glitchy but IMO more attractive.
  color = G35::max_color((sequence + PHASE) / (PHASE * 2));
  intensity = abs(PHASE - (int)(sequence * PULSE_RATE) % (PHASE + PHASE));
  return true;
}
