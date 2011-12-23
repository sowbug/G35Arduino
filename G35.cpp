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

G35::G35() : light_count_(0) {
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
    set_color(begin + count, intensity, sequence_func(sequence++ / span_size));
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

void G35::broadcast_intensity(uint8_t intensity) {
  set_color(get_broadcast_bulb(), intensity, COLOR_BLACK);
}
