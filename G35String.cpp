/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.

  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  Original version by Paul Martis (http://www.digitalmisery.com). See
  README for complete attributions.

  Special thanks to Richard <rkfoote@gmail.com> and "slinky" for the unrolled-
  loop protocol code that seems to work very well!
*/

#include <G35String.h>

#define MHZ_16 (1)  // standard Arduino/Teensy

#if MHZ_16
#define DELAYLONG 17    // should be ~ 20uS long
#define DELAYSHORT 7   // should be ~ 10uS long
#else  // 20MHz
#define DELAYLONG 25    // should be ~ 20uS long
#define DELAYSHORT 11   // should be ~ 10uS long
#endif
#define DELAYEND 40     // should be ~ 30uS long

#define ZERO(x) digitalWrite(x, LOW);           \
  delayMicroseconds(DELAYSHORT);                \
  digitalWrite(x, HIGH);                        \
  delayMicroseconds(DELAYLONG);

#define ONE(x) digitalWrite(x, LOW);            \
  delayMicroseconds(DELAYLONG);                 \
  digitalWrite(x, HIGH);                        \
  delayMicroseconds(DELAYSHORT);

G35String::G35String(uint8_t pin, uint8_t light_count,
                     uint8_t physical_light_count,
                     uint8_t bulb_zero, bool is_forward)
: G35(), pin_(pin), physical_light_count_(physical_light_count),
  bulb_zero_(bulb_zero), is_forward_(is_forward) {
  pinMode(pin, OUTPUT);
  light_count_ = light_count;
}

G35String::G35String(uint8_t pin, uint8_t light_count)
: G35(), pin_(pin), physical_light_count_(light_count),
  bulb_zero_(0), is_forward_(true) {
  pinMode(pin, OUTPUT);
  light_count_ = light_count;
}

void G35String::set_color(uint8_t bulb, uint8_t intensity, color_t color) {
  bulb += bulb_zero_;
  uint8_t r, g, b;
  r = color & 0x0F;
  g = (color >> 4) & 0x0F;
  b = (color >> 8) & 0x0F;

  if (intensity > MAX_INTENSITY) {
    intensity = MAX_INTENSITY;
  }

  noInterrupts();

  digitalWrite(pin_, HIGH);
  delayMicroseconds(DELAYSHORT);

  // LED Address
  if (bulb & 0x20) { ONE(pin_); } else { ZERO(pin_); }
  if (bulb & 0x10) { ONE(pin_); } else { ZERO(pin_); }
  if (bulb & 0x08) { ONE(pin_); } else { ZERO(pin_); }
  if (bulb & 0x04) { ONE(pin_); } else { ZERO(pin_); }
  if (bulb & 0x02) { ONE(pin_); } else { ZERO(pin_); }
  if (bulb & 0x01) { ONE(pin_); } else { ZERO(pin_); }

  // Brightness
  if (intensity & 0x80) { ONE(pin_); } else { ZERO(pin_); }
  if (intensity & 0x40) { ONE(pin_); } else { ZERO(pin_); }
  if (intensity & 0x20) { ONE(pin_); } else { ZERO(pin_); }
  if (intensity & 0x10) { ONE(pin_); } else { ZERO(pin_); }
  if (intensity & 0x08) { ONE(pin_); } else { ZERO(pin_); }
  if (intensity & 0x04) { ONE(pin_); } else { ZERO(pin_); }
  if (intensity & 0x02) { ONE(pin_); } else { ZERO(pin_); }
  if (intensity & 0x01) { ONE(pin_); } else { ZERO(pin_); }

  // Blue
  if (b & 0x8) { ONE(pin_); } else { ZERO(pin_); }
  if (b & 0x4) { ONE(pin_); } else { ZERO(pin_); }
  if (b & 0x2) { ONE(pin_); } else { ZERO(pin_); }
  if (b & 0x1) { ONE(pin_); } else { ZERO(pin_); }

  // Green
  if (g & 0x8) { ONE(pin_); } else { ZERO(pin_); }
  if (g & 0x4) { ONE(pin_); } else { ZERO(pin_); }
  if (g & 0x2) { ONE(pin_); } else { ZERO(pin_); }
  if (g & 0x1) { ONE(pin_); } else { ZERO(pin_); }

  // Red
  if (r & 0x8) { ONE(pin_); } else { ZERO(pin_); }
  if (r & 0x4) { ONE(pin_); } else { ZERO(pin_); }
  if (r & 0x2) { ONE(pin_); } else { ZERO(pin_); }
  if (r & 0x1) { ONE(pin_); } else { ZERO(pin_); }

  digitalWrite(pin_, LOW);
  delayMicroseconds(DELAYEND);

  interrupts();
}

void G35String::enumerate() {
  enumerate(is_forward_);
}

void G35String::enumerate(bool forward) {
  uint8_t count = physical_light_count_;
  uint8_t bulb = forward ? 0 : light_count_ - 1;
  int8_t delta = forward ? 1 : -1;
  while (count--) {
    set_color(bulb, MAX_INTENSITY, COLOR_RED);
    bulb += delta;
  }
}

void G35String::enumerate_forward() {
  enumerate(true);
}

void G35String::enumerate_reverse() {
  enumerate(false);
}

void G35String::do_test_patterns() {
  const uint8_t d = 1000 / light_count_;
  const uint8_t last_light = light_count_ - 1;
  fill_sequence(0, light_count_, 0, 1, 0, rainbow_color);

  for (int i = 0; i <= MAX_INTENSITY; ++i) {
    set_color(BROADCAST_BULB, i, COLOR_BLACK);
    delay(1);
  }

  delay(500);

  for (int i = MAX_INTENSITY; i >= 0; --i) {
    set_color(BROADCAST_BULB, i, COLOR_BLACK);
    delay(1);
  }

  for (uint8_t i = 0; i < light_count_; ++i) {
    set_color(i, MAX_INTENSITY, COLOR_GREEN);
    set_color(last_light - i, MAX_INTENSITY, COLOR_BLUE);
    if (i > 0) {
      set_color(i - 1, MAX_INTENSITY, COLOR_BLACK);
      set_color(last_light - i + 1, MAX_INTENSITY, COLOR_BLACK);
    }
    delay(d);
  }

  fill_color(0, light_count_, MAX_INTENSITY, COLOR_BLACK);
}
