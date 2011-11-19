#include <G35.h>

// Arduino pin number. Pin 13 will blink the on-board LED.
#define OUT_PIN 13

// Total # of lights on string (usually 50, 48, or 36). Maximum is 63,
// because the protocol uses 6-bit addressing and bulb #63 is reserved
// for broadcast messages.
#define LIGHT_COUNT 50

static uint16_t c;

G35 lights(OUT_PIN, LIGHT_COUNT);  // Constructor

void setup() {
  // Enumerate lights on string to enable individual bulb addressing
  lights.enumerate();
}

void loop() {
  // fill_color makes all bulbs from 0 to LIGHT_COUNT the same brightness
  // and color
  lights.fill_color(0, LIGHT_COUNT, DEFAULT_INTENSITY, COLOR_RED);

  delay(1000);

  lights.fill_color(0, LIGHT_COUNT, DEFAULT_INTENSITY, COLOR_GREEN);

  delay(1000);

  // set_color addresses one specific bulb at a time with brightness and
  // color setting
  for (int i = 0; i < LIGHT_COUNT / 2; ++i) {
    color_t color = lights.color_hue((i + c) % (HUE_MAX + 1));
    lights.set_color(i, DEFAULT_INTENSITY, color);
    lights.set_color(LIGHT_COUNT - 1 - i, DEFAULT_INTENSITY, color);
  }

  ++c;

  delay(1000);
}
