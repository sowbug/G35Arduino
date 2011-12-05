/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.

  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  Original version by Paul Martis (http://www.digitalmisery.com). See
  README for complete attributions.
*/

// In order to support multiple strings on different pins, we have to
// disable DigitalWriteFast. I (miket) am thinking about how to generally
// enable this. It might involve callbacks -- which might be just as slow
// as Arduino digitalWrite().
#define USE_DIGITAL_WRITE_FAST (0)

#include <G35.h>
#include <DigitalWriteFast.h>

#if USE_DIGITAL_WRITE_FAST
#define SHORT_PHASE (10)
#define LONG_PHASE (20)
#define SHORT_ZERO_PHASE (SHORT_PHASE)
#define LONG_ZERO_PHASE (LONG_PHASE)
#define QUIET_PHASE (30)

// Hardcode this to the number of the pin you want.
// See digitalWriteFast.h for why a plain number matters (TL;DR: it's possible
// to generate a much faster write if the value is known at compile time).
#define G35_PIN (13)

#else

// Your digitalWrite() is slow, and we haven't yet worked out perfect timings
// for all parts of the protocol. Sorry.
#define SHORT_PHASE (6)
#define LONG_PHASE (12)
#define SHORT_ZERO_PHASE (2)
#define LONG_ZERO_PHASE (17)
#define QUIET_PHASE (30)
#define G35_PIN (pin_)

#endif

G35::G35(int pin, int light_count) {
  pinModeFast(pin, OUTPUT);
  pin_ = pin;
  light_count_ = light_count;
}

// The timings below have been hand-tuned. Adjust if necessary.
void G35::begin() {
  digitalWriteFast(G35_PIN, HIGH);
  delayMicroseconds(SHORT_PHASE);
  digitalWriteFast(G35_PIN, LOW);
}

void G35::one() {
  digitalWriteFast(G35_PIN, LOW);
  delayMicroseconds(LONG_PHASE);
  digitalWriteFast(G35_PIN, HIGH);
  delayMicroseconds(SHORT_PHASE);
  digitalWriteFast(G35_PIN, LOW);
}

void G35::zero() {
  digitalWriteFast(G35_PIN, LOW);
  delayMicroseconds(SHORT_ZERO_PHASE);
  digitalWriteFast(G35_PIN, HIGH);
  delayMicroseconds(LONG_ZERO_PHASE);
  digitalWriteFast(G35_PIN, LOW);
}

void G35::end() {
  digitalWriteFast(G35_PIN, LOW);
  delayMicroseconds(QUIET_PHASE);
}

void G35::set_color(uint8_t led, uint8_t intensity, color_t color) {
  uint8_t i;
  begin();
  for (i = 6; i; i--, (led <<= 1)) {
    if (led & (1 << 5)) {
      one();
    } else {
      zero();
    }
  }
  for (i = 8; i; i--, (intensity <<= 1)) {
    if (intensity & (1 << 7)) {
      one();
    } else {
      zero();
    }
  }

  for (i=12; i; i--, (color <<= 1)) {
    if (color & (1 << 11)) {
      one();
    } else {
      zero();
    }
  }
  end();
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
