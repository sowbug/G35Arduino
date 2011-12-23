// StockPlus: A Sunday afternoon's worth of work duplicating the stock G-35
// controller programs, plus a few extra thrown in for fun. Some are
// intentionally done a little differently from the originals because I thought
// they looked better.
//
// By Mike Tsao <http://github.com/sowbug>
//
// See http://www.youtube.com/watch?v=7al1vIinhuo for original patterns.

#include <G35String.h>
#include <ProgramRunner.h>
#include <StockPrograms.h>
#include <PlusPrograms.h>

// How long each program should run.
#define PROGRAM_DURATION_SECONDS (30)

#define LIGHT_COUNT (50)

// Arduino pin number. Pin 13 will blink the on-board LED.
#define G35_PIN (13)

G35String lights(G35_PIN, LIGHT_COUNT);

const int PROGRAM_COUNT = StockProgramGroup::ProgramCount +
  PlusProgramGroup::ProgramCount;

StockProgramGroup stock_programs;
PlusProgramGroup plus_programs;

LightProgram* CreateProgram(uint8_t program_index) {
  randomSeed(rand() + analogRead(0));

  if (program_index < StockProgramGroup::ProgramCount) {
    return stock_programs.CreateProgram(lights, program_index);
  }
  program_index -= StockProgramGroup::ProgramCount;

  if (program_index < PlusProgramGroup::ProgramCount) {
    return plus_programs.CreateProgram(lights, program_index);
  }
  program_index -= PlusProgramGroup::ProgramCount;

  return NULL;
}

ProgramRunner runner(CreateProgram, PROGRAM_COUNT, PROGRAM_DURATION_SECONDS);

void setup() {
  randomSeed(analogRead(0));

  delay(50);
  lights.enumerate();
  delay(50);

  lights.do_test_patterns();
}

void loop() {
  runner.loop();
}
