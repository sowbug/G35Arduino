/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.

  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  Original version by Paul Martis (http://www.digitalmisery.com). See
  README for complete attributions.

  Special thanks to Richard <rkfoote@gmail.com> and "slinky" for the unrolled-
  loop protocol code that seems to work very well!
*/

#include <G35.h>

G35::G35(int pin, int light_count) {
  pinMode(pin, OUTPUT);
  pin_ = pin;
  light_count_ = light_count;
}

#define MHZ_20 (0)  // else 16MHz, standard Arduino/Teensy
#if MHZ_20
#define DELAYLONG 25    // should be ~ 20uS long
#define DELAYSHORT 11   // should be ~ 10uS long
#else
#define DELAYLONG 17    // should be ~ 20uS long
#define DELAYSHORT 7   // should be ~ 10uS long
#endif
#define DELAYEND 40     // should be ~ 30uS long

#define ZERO(x) digitalWrite(x, LOW); \
  delayMicroseconds(DELAYSHORT); \
  digitalWrite(x, HIGH); \
  delayMicroseconds(DELAYLONG);

#define ONE(x) digitalWrite(x, LOW); \
  delayMicroseconds(DELAYLONG); \
  digitalWrite(x, HIGH); \
  delayMicroseconds(DELAYSHORT);

void G35::set_color(uint8_t led, uint8_t intensity, color_t color) {
  uint8_t r, g, b;
  r = color & 0x0F;
  g = (color >> 4) & 0x0F;
  b = (color >> 8) & 0x0F;

  digitalWrite(pin_, HIGH);
  delayMicroseconds(DELAYSHORT);

  // LED Address
  if (led & 0x20) { ONE(pin_); } else { ZERO(pin_); }
  if (led & 0x10) { ONE(pin_); } else { ZERO(pin_); }
  if (led & 0x08) { ONE(pin_); } else { ZERO(pin_); }
  if (led & 0x04) { ONE(pin_); } else { ZERO(pin_); }
  if (led & 0x02) { ONE(pin_); } else { ZERO(pin_); }
  if (led & 0x01) { ONE(pin_); } else { ZERO(pin_); }

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
}

bool G35::set_color_if_in_range(uint8_t position, uint8_t intensity,
				color_t color) {
  if (position >= light_count_) {
    return false;
  }
  set_color(position, intensity, color);
  return true;
}

// Returns 12-bit color from red, green, and blue components
color_t G35::color(uint8_t r, uint8_t g, uint8_t b) {
  return COLOR(r, g, b);
}

color_t G35::color_hue(uint8_t h) {
  switch (h >> 4) {
  case 0:     h -= 0; return color(h, CHANNEL_MAX, 0);
  case 1:     h -= 16; return color(CHANNEL_MAX, (CHANNEL_MAX - h), 0);
  case 2:     h -= 32; return color(CHANNEL_MAX, 0, h);
  case 3:     h -= 48; return color((CHANNEL_MAX - h), 0, CHANNEL_MAX);
  case 4:     h -= 64; return color(0, h, CHANNEL_MAX);
  case 5:     h -= 80; return color(0, CHANNEL_MAX, (CHANNEL_MAX - h));
  default:
    return COLOR_WHITE;
  }
}

void G35::fill_color(uint8_t begin, uint8_t count,
		     uint8_t intensity, color_t color) {
  while (count--) {
    set_color(begin++, intensity, color);
  }
}

void G35::fill_random_max(uint8_t begin, uint8_t count, uint8_t intensity) {
  while (count--) {
    set_color(begin++, intensity, max_color(rand()));
  }
}

void G35::fill_sequence(uint8_t begin, uint8_t count,
			uint16_t sequence, uint8_t span_size,
			uint8_t intensity,
			color_t (*sequence_func)(uint16_t sequence)) {
  while (count--) {
    set_color(begin + count, intensity,
	      sequence_func(sequence++ / span_size));
  }
}

void G35::fill_sequence(uint16_t sequence, uint8_t span_size,
			uint8_t intensity,
			color_t (*sequence_func)(uint16_t sequence)) {
  fill_sequence(0, light_count_, sequence, span_size, intensity, sequence_func);
}

void G35::fill_sequence(uint8_t begin, uint8_t count,
			uint16_t sequence, uint8_t span_size,
			bool (*sequence_func)(uint16_t sequence, color_t& color,
					      uint8_t& intensity)) {
  while (count--) {
    color_t color;
    uint8_t intensity;
    sequence_func(sequence++ / span_size, color, intensity);
    set_color(begin + count, intensity, color);
  }
}

void G35::enumerate(bool reverse) {
  uint8_t count = light_count_;
  uint8_t bulb = reverse ? light_count_ - 1 : 0;
  int8_t delta = reverse ? -1 : 1;
  while (count--) {
    set_color(bulb, MAX_INTENSITY, COLOR_BLACK);
    bulb += delta;
  }
}

void G35::enumerate_forward() {
  enumerate(false);
}

void G35::enumerate_reverse() {
  enumerate(true);
}

void G35::test_patterns() {
  fill_color(0, light_count_, 0, rainbow_color(RB_FIRST));
  fade_in(1);
  for (uint8_t i = RB_FIRST; i <= RB_LAST; ++i) {
    delay(50);
    fill_color(0, light_count_, MAX_INTENSITY, rainbow_color(i));
  }
  fade_out(1);
  fill_color(0, light_count_, MAX_INTENSITY, COLOR_BLACK);
  delay(500);
}

color_t G35::rainbow_color(uint16_t color) {
  if (color >= RB_COUNT) {
    color = color % RB_COUNT;
  }
  switch (color) {
  case RB_RED:
    return COLOR_RED;
  case RB_ORANGE:
    return COLOR_ORANGE;
  case RB_YELLOW:
    return COLOR_YELLOW;
  case RB_GREEN:
    return COLOR_GREEN;
  case RB_BLUE:
    return COLOR_BLUE;
  case RB_INDIGO:
    return COLOR_INDIGO;
  case RB_VIOLET:
    return COLOR_VIOLET;
  default:
    return COLOR_WHITE;
  }
}

color_t G35::max_color(uint16_t color) {
  if (color >= 7) {
    color = color % 7;
  }
  switch (color) {
  case 0:
    return COLOR_RED;
  case 1:
    return COLOR_GREEN;
  case 2:
    return COLOR_BLUE;
  case 3:
    return COLOR_CYAN;
  case 4:
    return COLOR_MAGENTA;
  case 5:
    return COLOR_YELLOW;
  default:
    return COLOR_WHITE;
  }
}

void G35::fade_in(uint8_t delay_msec) {
  for (int i = 0; i <= MAX_INTENSITY; ++i) {
    set_color(BROADCAST_BULB, i, COLOR_BLACK);
    delay(delay_msec);
  }
}

void G35::fade_out(uint8_t delay_msec) {
  for (int i = MAX_INTENSITY; i >= 0; --i) {
    set_color(BROADCAST_BULB, i, COLOR_BLACK);
    delay(delay_msec);
  }
}
