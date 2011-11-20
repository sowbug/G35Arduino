/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  Original version by Paul Martis (http://www.digitalmisery.com). See
  README for complete attributions.
*/

#include <WProgram.h>
#include <G35.h>

G35::G35(int pin, int light_count) {
  pinMode(pin, OUTPUT);
  _pin = pin;
  _light_count = light_count;
}

// The timings below have been hand-tuned. Adjust if necessary.
void G35::begin() {
  digitalWrite(_pin, HIGH);  
  delayMicroseconds(6);  // Output should be ~ 10uS long 
  digitalWrite(_pin, LOW); 
}

void G35::one() {
  digitalWrite(_pin, LOW);  
  delayMicroseconds(12);  // Output should be ~ 20uS long 
  digitalWrite(_pin, HIGH);  
  delayMicroseconds(6);   // Output should be ~ 10uS long 
  digitalWrite(_pin, LOW);
}

void G35::zero() {
  digitalWrite(_pin, LOW);  
  delayMicroseconds(2);  // Output should be ~ 10uS long
  digitalWrite(_pin, HIGH);  
  delayMicroseconds(17);  // Output should be ~ 20uS long  
  digitalWrite(_pin, LOW);
}

void G35::end() {
  digitalWrite(_pin, LOW);  
  delayMicroseconds(30);  // Quiet time should be ~ 30us long
}

// Turn on a specific LED with a color and brightness
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
  for (i = 8; i; i--, (intensity <<=1 )) {  
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

// Returns 12-bit color from red, green, and blue components
color_t G35::color(uint8_t r, uint8_t g, uint8_t b) {  
  return COLOR(r, g, b);  
}

// Returns primary hue colors
color_t G35::color_hue(uint8_t h) {  
  switch (h >> 4) {  
  case 0:     h -= 0; return color(h, CHANNEL_MAX, 0);  
  case 1:     h -= 16; return color(CHANNEL_MAX, (CHANNEL_MAX - h), 0);  
  case 2:     h -= 32; return color(CHANNEL_MAX, 0, h);  
  case 3:     h -= 48; return color((CHANNEL_MAX - h), 0, CHANNEL_MAX);  
  case 4:     h -= 64; return color(0, h, CHANNEL_MAX);  
  case 5:     h -= 80; return color(0, CHANNEL_MAX, (CHANNEL_MAX - h));  
  }   
}

// Make all LEDs the same color starting at specified beginning LED
void G35::fill_color(uint8_t begin, uint8_t count,
                     uint8_t intensity, color_t color) {
  while (count--) {  
    set_color(begin++, intensity, color);  
  }
}

// Initialize string for individual bulb addressing
void G35::enumerate(bool reverse) {
  uint8_t count = _light_count;
  uint8_t bulb = reverse ? _light_count - 1 : 0;
  int8_t delta = reverse ? -1 : 1;
  while (count--) {
    set_color(bulb, DEFAULT_INTENSITY, COLOR_BLACK);
    bulb += delta;
  }
}

void G35::enumerate_forward() {
  enumerate(false);
}

void G35::enumerate_reverse() {
  enumerate(true);
}
