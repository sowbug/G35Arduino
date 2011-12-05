#include <G35.h>

// Total # of lights on string (usually 50, 48, or 36). Maximum is 63,
// because the protocol uses 6-bit addressing and bulb #63 is reserved
// for broadcast messages.
#define LIGHT_COUNT 50

// Arduino pin number. Pin 13 will blink the on-board LED.
#define G35_PIN (13)

static uint16_t c;

G35 lights(G35_PIN, LIGHT_COUNT);

void setup() {
  // Enumerate lights on string to enable individual bulb addressing
  lights.enumerate_forward();
}

void loop() {
  // fill_color makes all bulbs from 0 to LIGHT_COUNT the same brightness
  // and color
  lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_RED);

  delay(1000);

  lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_GREEN);

  delay(1000);

  // set_color addresses one specific bulb at a time with brightness and
  // color setting
  for (int i = 0; i < LIGHT_COUNT / 2; ++i) {
    color_t color = lights.color_hue((i + c) % (HUE_MAX + 1));
    lights.set_color(i, G35::MAX_INTENSITY, color);
    lights.set_color(LIGHT_COUNT - 1 - i, G35::MAX_INTENSITY, color);
  }

  ++c;

  delay(1000);
}
