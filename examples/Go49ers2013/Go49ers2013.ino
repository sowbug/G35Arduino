/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2; -*-

   G35: An Arduino library for GE Color Effects G-35 holiday lights.
   Copyright © 2013 The G35 Authors. Use, modification, and
   distribution are subject to the BSD license as described in the
   accompanying LICENSE file.

   This sample took the San Francisco 49ers all the way to the Super
   Bowl in the 2012-2013 season. Unfortunately its awesomeness was
   insufficient, and they failed to defeat the Baltimore Ravens in a
   heartbreaking 34-31 loss.

   By Mike Tsao <github.com/sowbug>. See README for complete
   attributions.
*/

#include <G35String.h>
#include <G35StringGroup.h>
#include <ProgramRunner.h>
#include <StockPrograms.h>
#include <PlusPrograms.h>

// Arduino with Like A G35 shield (https://github.com/sowbug/like-a-g35).
// First string on pin 8. Second on 9. My roofline isn't quite long enough
// for 100 lights, so I have the final 10 hidden.
G35String lights_1(8, 50, 50, 0, false);
G35String lights_2(9, 40);

class RedYellowChase : public LightProgram {
 public:
  RedYellowChase(G35& g35);
  uint32_t Do();
  static color_t color_sequence(uint16_t sequence);

 private:
  uint8_t count_;
  uint16_t sequence_;
};

RedYellowChase::RedYellowChase(G35& g35)
  : LightProgram(g35),
    count_(1),
    sequence_(0) {}

uint32_t RedYellowChase::Do() {
  g35_.fill_sequence(0, count_, sequence_, 5, 255, color_sequence);
  if (count_ < light_count_) {
    ++count_;
  } else {
    ++sequence_;
  }
  return bulb_frame_;
}

// static
color_t RedYellowChase::color_sequence(uint16_t sequence) {
  return sequence % 2 ? COLOR_RED : COLOR_YELLOW;
}

const int PROGRAM_COUNT = 1;

G35StringGroup string_group;

LightProgram* CreateProgram(uint8_t program_index) {
  return new RedYellowChase(string_group);
}

// How long each program should run.
#define PROGRAM_DURATION_SECONDS (60)

ProgramRunner runner(CreateProgram, PROGRAM_COUNT, PROGRAM_DURATION_SECONDS);

// http://www.utopiamechanicus.com/77/better-arduino-random-numbers/
// We assume A0 and A1 are disconnected.
uint32_t seedOut(unsigned int noOfBits) {
  uint32_t seed = 0, limit = 99;
  int bit0 = 0, bit1 = 0;
  while (noOfBits--) {
    for (int i = 0; i < limit; ++i) {
      bit0 = analogRead(0) & 1;
      bit1 = analogRead(1) & 1;
      if (bit1 != bit0)
        break;
    }
    seed = (seed << 1) | bit1;
  }
  return seed;
}

void setup() {
  uint32_t seed = seedOut(32);
  randomSeed(seed);
  seed &= 0xff;
  // random() isn't very random. But this seed generator works quite well.
  while (seed--) {
    random();
  }

  delay(50);
  lights_1.enumerate();
  lights_2.enumerate();

  lights_1.do_test_patterns();
  lights_2.do_test_patterns();

  string_group.AddString(&lights_1);
  string_group.AddString(&lights_2);
}

void loop() {
  runner.loop();
}
