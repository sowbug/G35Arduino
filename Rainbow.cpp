/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2012 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <http://github.com/sowbug>.
  Programs referenced in MEOPrograms by Mark Ortiz <github.com/MarkEMarkEMark>.
  Portions adapted from Adafruit.

  See README for complete attributions.
*/

#include <Rainbow.h>

#define PATTERN_COUNT (8)
Rainbow::Rainbow(G35& g35)
  : LightProgram(g35), wait_(0), pattern_(rand() % PATTERN_COUNT), step_(0) {
}

uint32_t Rainbow::Do() {
  bool fortyEight;
  for (int i=0; i < light_count_; i++) {
    switch (pattern_) {
    case 0:
      fortyEight = false;
      g35_.fill_color(i, 1, G35::MAX_INTENSITY,
                      Rainbow::LineRG((i + step_) % 32));
      break;
    case 1:
      fortyEight = false;
      g35_.fill_color(i, 1, G35::MAX_INTENSITY,
                      Rainbow::LineGB((i + step_) % 32));
      break;
    case 2:
      fortyEight = false;
      g35_.fill_color(i, 1, G35::MAX_INTENSITY,
                      Rainbow::LineBR((i + step_) % 32));
      break;
    case 3:
      fortyEight = true;
      g35_.fill_color(i, 1, G35::MAX_INTENSITY,
                      Rainbow::Wheel((i + step_) % 48));
      break;
    case 4:
      fortyEight = false;
      g35_.fill_color(i, 1, G35::MAX_INTENSITY,
                      Rainbow::LineRG(((i * 32 / light_count_) + step_) % 32));
      break;
    case 5:
      fortyEight = false;
      g35_.fill_color(i, 1, G35::MAX_INTENSITY,
                      Rainbow::LineGB(((i * 32 / light_count_) + step_) % 32));
      break;
    case 6:
      fortyEight = false;
      g35_.fill_color(i, 1, G35::MAX_INTENSITY,
                      Rainbow::LineBR(((i * 32 / light_count_) + step_) % 32));
      break;
    case 7:
      fortyEight = true;
      g35_.fill_color(i, 1, G35::MAX_INTENSITY,
                      Rainbow::Wheel(((i * 48 / light_count_) + step_) % 48));
      break;
    }
  }

  // reset at end of wheel or line
  ++step_;
  if (((step_ == 48) && fortyEight) || ((step_ == 32) && !fortyEight)) {
    step_ = 0;
  }

  delay(wait_);

  return bulb_frame_;
}

uint32_t Rainbow::Wheel(uint16_t WheelPos) {
  byte r, g, b;
  switch (WheelPos / 16) {
  case 0:
    r = 15 - WheelPos % 16; // red down
    g = WheelPos % 16;       // green up
    b = 0;                    // blue off
    break;
  case 1:
    g = 15 - WheelPos % 16; // green down
    b = WheelPos % 16;       // blue up
    r = 0;                    // red off
    break;
  case 2:
    b = 15 - WheelPos % 16; // blue down
    r = WheelPos % 16;       // red up
    g = 0;                    // green off
    break;
  }
  return (COLOR(r,g,b));
}

uint32_t Rainbow::LineRG(uint16_t WheelPos) {
  byte r, g, b;
  switch (WheelPos / 16) {
  case 0:
    r = 15 - WheelPos % 16; // red down
    g = WheelPos % 16;       // green up
    b = 0;					// blue off
    break;
  case 1:
    r = WheelPos % 16;       // red up
    g = 15 - WheelPos % 16; // green down
    b = 0;					// blue off
    break;
  }
  return (COLOR(r,g,b));
}

uint32_t Rainbow::LineGB(uint16_t WheelPos) {
  byte r, g, b;
  switch (WheelPos / 16) {
  case 0:
    r = 0;                    // red off
    g = 15 - WheelPos % 16; // green down
    b = WheelPos % 16;       // blue up
    break;
  case 1:
    r = 0;                    // red off
    g = WheelPos % 16;       // green up
    b = 15 - WheelPos % 16; // blue down
    break;
  }
  return (COLOR(r,g,b));
}

uint32_t Rainbow::LineBR(uint16_t WheelPos) {
  byte r, g, b;
  switch (WheelPos / 16) {
  case 0:
    r = WheelPos % 16;       // red up
    g = 0;                    // green off
    b = 15 - WheelPos % 16; // blue down
    break;
  case 1:
    r = 15 - WheelPos % 16; // red down
    g = 0;                    // green off
    b = WheelPos % 16;       // blue up
    break;
  }
  return (COLOR(r,g,b));
}
