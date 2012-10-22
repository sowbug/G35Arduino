/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2; -*-

   G35: An Arduino library for GE Color Effects G-35 holiday lights.
   Copyright © 2012 The G35 Authors. Use, modification, and distribution are
   subject to the BSD license as described in the accompanying LICENSE file.

   This file is the code that I used for Halloween 2012 at my home.

   By Mike Tsao <github.com/sowbug>. See README for complete attributions.
*/

// If defined, then the hardware is an Arduino. If not, then it's a Teensy.
// More described below. I used this switch because I had an Arduino in the
// house when I was writing new programs, then ran outside in the cold with my
// laptop to reprogram the Teensy that was running the real lights.
#define ARDUINO_HW 1

#include <G35String.h>
#include <G35StringGroup.h>
#include <ProgramRunner.h>
#include <Programs.h>

#ifdef ARDUINO_HW
// A regular Arduino with Like A G35 shield.
// First string on pin 8. Second on 9.
G35String lights_1(8, 50, 50, 0, false);
G35String lights_2(9, 41);

#else
// A Teensy 2.0 that's been flashed with the Arduino Leonardo bootloader (thus
// permanently destroying the HalfKay bootloader that ships with the Teensy).
// The first string is connected to D6, and the second to B4.
G35String lights_1(12, 50, 50, 0, false);
G35String lights_2(8, 41);
#endif

class PG2 {
 public:
 PG2()
   : program_count_(0) {}

  typedef LightProgram* (CreateFunc)(G35 &);
  void Add(CreateFunc *create_func) {
    create_funcs_[program_count_++] = create_func;
  }

  LightProgram* GetNextProgram(G35 &g35, uint8_t index) {
    return create_funcs_[index](g35);
  }

  uint8_t program_count() {
    return program_count_;
  }

 private:
  uint8_t program_count_;
  CreateFunc *create_funcs_[10];
};

PG2 pg2;
G35StringGroup string_group;

LightProgram* CreateProgram(uint8_t program_index) {
  return pg2.GetNextProgram(string_group, program_index);
}

// How long each program should run.
#define PROGRAM_DURATION_SECONDS (60)

ProgramRunner runner(CreateProgram, pg2.program_count(),
                     PROGRAM_DURATION_SECONDS);

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

  pg2.Add(&CreateEyes);
}

void loop() {
  runner.loop();
}
