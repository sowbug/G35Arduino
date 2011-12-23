// A demonstration of multiple strings acting as a single large one.
//
// By Mike Tsao <http://github.com/sowbug>

#include <G35String.h>
#include <G35StringGroup.h>
#include <ProgramRunner.h>
#include <StockPrograms.h>
#include <PlusPrograms.h>

// How long each program should run.
#define PROGRAM_DURATION_SECONDS (30)

// Standard Arduino, string 1 on Pin 13, string 2 on Pin 12.
G35String lights_1(13, 50);
G35String lights_2(12, 50);
G35StringGroup string_group;

const int PROGRAM_COUNT = StockProgramGroup::ProgramCount +
  PlusProgramGroup::ProgramCount;

StockProgramGroup stock_programs;
PlusProgramGroup plus_programs;

LightProgram* CreateProgram(uint8_t program_index) {
  randomSeed(rand() + analogRead(0));

  if (program_index < StockProgramGroup::ProgramCount) {
    return stock_programs.CreateProgram(string_group, program_index);
  }
  program_index -= StockProgramGroup::ProgramCount;

  if (program_index < PlusProgramGroup::ProgramCount) {
    return plus_programs.CreateProgram(string_group, program_index);
  }
  program_index -= PlusProgramGroup::ProgramCount;

  return NULL;
}

ProgramRunner runner(CreateProgram, PROGRAM_COUNT, PROGRAM_DURATION_SECONDS);

void setup() {
  randomSeed(analogRead(0));

  delay(50);
  lights_1.enumerate();
  lights_2.enumerate();
  delay(50);

  lights_1.do_test_patterns();
  lights_2.do_test_patterns();

  string_group.AddString(&lights_1);
  string_group.AddString(&lights_2);
}

void loop() {
  runner.loop();
}
