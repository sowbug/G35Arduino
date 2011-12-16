// StockPlus: A Sunday afternoon's worth of work duplicating the stock G-35
// controller programs, plus a few extra thrown in for fun. Some are
// intentionally done a little differently from the originals because I thought
// they looked better.
//
// By Mike Tsao <http://github.com/sowbug>
//
// See http://www.youtube.com/watch?v=7al1vIinhuo for original patterns.

#include <G35.h>
#include <LightProgram.h>
#include <ProgramRunner.h>
#include <StockPrograms.h>
#include <PlusPrograms.h>

// How long each program should run.
#define PROGRAM_DURATION_SECONDS (30)

#define LIGHT_COUNT (50)

// Arduino pin number. Pin 13 will blink the on-board LED.
#define G35_PIN (13)

G35 lights(G35_PIN, LIGHT_COUNT);

// -1 because SteadyWhite is more of a mode than a program.
#define PROGRAM_COUNT (STOCK_PROGRAM_COUNT + PLUS_PROGRAM_COUNT - 1)
LightProgram* CreateProgram(uint8_t program_index) {
  randomSeed(rand() + analogRead(0));

  switch (program_index) {
  case 0: return new Inchworm(lights);
  case 1: return new Orbit(lights);
  case 2: return new CrossOverWave(lights);
  case 3: return new ForwardWave(lights);
  case 4: return new ChasingRainbow(lights);
  case 5: return new AlternateDirectionalWave(lights);
  case 6: return new FadeInFadeOutSolidColors(lights);
  case 7: return new BidirectionalWave(lights);
  case 8: return new ChasingSolidColors(lights);
  case 9: return new FadeInFadeOutMultiColors(lights);
  case 10: return new RandomSparkling(lights);
  case 11: return new ChasingMultiColors(lights);
  case 12: return new ChasingWhiteRedBlue(lights);
  case 13: return new Meteorite(lights);
  case 14: return new Twinkle(lights);
  case 15: return new RedGreenChase(lights);
  case 16: return new Pulse(lights);
  case 17: return new Cylon(lights);
  case 18: return new Stereo(lights);
  case 19: return new OrbitSmudge(lights);
  case PROGRAM_COUNT:
  default:
    // PROBLEM! PROGRAM_COUNT is wrong.
    return NULL;
  }
}

ProgramRunner runner(CreateProgram, PROGRAM_COUNT, PROGRAM_DURATION_SECONDS);

void setup() {
  randomSeed(analogRead(0));

  delay(50);
  lights.enumerate_forward();
  delay(50);

  lights.test_patterns();
}

void loop() {
  runner.loop();
}
