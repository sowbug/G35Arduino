// StockPlus: A Sunday afternoon's worth of work duplicating the stock G-35
// controller programs, plus a couple extra thrown in for fun. Some are
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

class RedGreenChase : public LightProgram {
 public:
 RedGreenChase(G35& g35)
   : LightProgram(g35), 
    count_(1),
    sequence_(0) {}

  uint32_t Do() {
    g35_.fill_sequence(0, count_, sequence_, 5, G35::MAX_INTENSITY,
                       red_green);
    if (count_ < light_count_) {
      ++count_;
    } else {
      ++sequence_;
    }
    return bulb_frame_;
  }

  static color_t red_green(uint16_t sequence) {
    return sequence % 2 ? COLOR_RED : COLOR_GREEN;
  }

 private:
  uint8_t count_;
  uint16_t sequence_;
};

class Meteorite : public LightProgram {
 public:
 Meteorite(G35& g35) 
   : LightProgram(g35),
    position_(g35_.get_last_light() + TAIL) {}

  uint32_t Do() {
    if (position_ == g35_.get_last_light() + TAIL) {
      position_ = 0;
      uint8_t r, g, b;
      r = rand() > (RAND_MAX / 2) ? 15 : 0;
      g = rand() > (RAND_MAX / 2) ? 15 : 0;
      b = rand() > (RAND_MAX / 2) ? 15 : 0;
      if (r == 0 && g == 0 && b == 0) {
        r = 15;
        g = 15;
        b = 15;
      }
      d_ = rand() % bulb_frame_ + 5;
      colors_[0] = COLOR(r, g, b);
      colors_[1] = COLOR(r * 3 / 4, g * 3 / 4, b * 3 / 4);
      colors_[2] = COLOR(r * 2 / 4, g * 2 / 4, b * 2 / 4);
      colors_[3] = COLOR(r * 1 / 4, g * 1 / 4, b * 1 / 4);
      colors_[4] = COLOR(r * 0 / 4, g * 0 / 4, b * 0 / 4);
    }

    for (int i = 0; i < TAIL; ++i) {
      int pos = position_ - i;
      g35_.set_color_if_in_range(pos, G35::MAX_INTENSITY, colors_[i]);
    }
    ++position_;
    return d_;
  }
 private:
  static const uint8_t TAIL = 5;

  uint8_t d_;
  int16_t position_;
  color_t colors_[TAIL];
};

class Twinkle : public LightProgram {
 public:
 Twinkle(G35& g35)
   : LightProgram(g35) {
    g35_.fill_random_max(0, light_count_, G35::MAX_INTENSITY);
  }

  uint32_t Do() {
    g35_.set_color(rand() % light_count_, G35::MAX_INTENSITY,
                   G35::max_color(rand()));
    return bulb_frame_;
  }
};

// How long each program should run.
#define PROGRAM_DURATION_SECONDS (30)

// Arduino pin number. Pin 13 will blink the on-board LED.
#define OUT_PIN (13)

#define LIGHT_COUNT (50)

G35 lights(OUT_PIN, LIGHT_COUNT);

#define PROGRAM_COUNT (15)
LightProgram* CreateProgram(uint8_t program_index) {
  switch (program_index) {
  case 0: return new SteadyWhite(lights);
  case 1: return new CrossOverWave(lights);
  case 2: return new ForwardWave(lights);
  case 3: return new ChasingRainbow(lights);
  case 4: return new AlternateDirectionalWave(lights);
  case 5: return new FadeInFadeOutSolidColors(lights);
  case 6: return new BidirectionalWave(lights);
  case 7: return new ChasingSolidColors(lights);
  case 8: return new FadeInFadeOutMultiColors(lights);
  case 9: return new RandomSparkling(lights);
  case 10: return new ChasingMultiColors(lights);
  case 11: return new ChasingWhiteRedBlue(lights);
  case 12: return new Meteorite(lights);
  case 13: return new Twinkle(lights);
  case 14: return new RedGreenChase(lights);
  case PROGRAM_COUNT:
  default:
    // PROBLEM! PROGRAM_COUNT is wrong.
    return NULL;
  }
}

ProgramRunner runner(CreateProgram, 15, PROGRAM_DURATION_SECONDS);

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
