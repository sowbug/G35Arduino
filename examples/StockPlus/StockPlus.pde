// By Mike Tsao <github.com/sowbug>
//
// StockPlus: A Sunday afternoon's worth of work duplicating the stock G-35
// controller programs, plus a couple extra thrown in for fun. Some are
// intentionally done a little differently from the originals because I thought
// they looked better.
//
// See http://www.youtube.com/watch?v=7al1vIinhuo for original patterns.

#include <G35.h>

// How long each program should run.
#define PROGRAM_DURATION_SECONDS (30)

// Arduino pin number. Pin 13 will blink the on-board LED.
#define OUT_PIN 13

// Total # of lights on string (usually 50, 48, or 36). Maximum is 63, because the
// protocol uses 6-bit addressing and bulb #63 is reserved for broadcast messages.
#define LIGHT_COUNT 50
#define LAST_LIGHT (LIGHT_COUNT - 1)
#define HALFWAY_POINT (LIGHT_COUNT / 2)

// One bulb's share of a second, in milliseconds
#define BULB_FRAME (1000 / LIGHT_COUNT)

G35 lights(OUT_PIN, LIGHT_COUNT);

void setup() {
  delay(50);
  lights.enumerate_forward();
  delay(50);
  lights.test_patterns();
  randomSeed(analogRead(0));
}

// Interface for light programs.
class LightProgram {
 public:
  virtual void Init() {}
  virtual void Do() = 0;
};

class SteadyWhite : public LightProgram {
 public:
  void Init() {
    lights.fill_color(0, LIGHT_COUNT, 0, COLOR_WHITE);
    lights.fade_in(10);
  }
  void Do() {
  }
};

class CrossOverWave : public LightProgram {
 public:
  void Init() {
    x_ = LIGHT_COUNT;
  }

  void Do() {
    if (x_ == LIGHT_COUNT) {
      x_ = 0;
      color_a_ = 0;
      color_b_ = 0;
      while (color_a_ == color_b_) {
	color_a_ = G35::max_color(rand());
	color_b_ = G35::max_color(rand());
      }
    }
    lights.set_color(x_, G35::MAX_INTENSITY, color_a_);
    lights.set_color(LAST_LIGHT - x_, G35::MAX_INTENSITY, color_b_);
    ++x_;
    delay(BULB_FRAME);
  }
 private:
  uint8_t x_;
  color_t color_a_;
  color_t color_b_;
};

class ForwardWave : public LightProgram {
 public:
  void Init() {
    x_ = LIGHT_COUNT;
  }

  void Do() {
    if (x_ == LIGHT_COUNT) {
      x_ = 0;
      int old_color = color_;
      do {
	color_ = G35::max_color(rand());
      } while (old_color == color_);
    }
    lights.set_color(x_, G35::MAX_INTENSITY, color_);
    ++x_;
    delay(BULB_FRAME);
  }
 private:
  uint8_t x_;
  color_t color_;
};

class ChasingRainbow : public LightProgram {
 public:
  void Init() {
    count_ = 1;
    sequence_ = 0;
  }
  void Do() {
    lights.fill_sequence(0, count_, sequence_, 1, G35::MAX_INTENSITY,
			 G35::rainbow_color);
    if (count_ < LIGHT_COUNT) {
      ++count_;
    } else {
      ++sequence_;
    }
    delay(BULB_FRAME);
  }
 private:
  uint8_t count_;
  uint16_t sequence_;
};

class AlternateDirectionalWave : public LightProgram {
 public:
  void Init() {
    x_ = LIGHT_COUNT;
    direction_ = -1;
  }

  void Do() {
    bool hit_end = false;
    if (x_ == LIGHT_COUNT) {
      x_ = 0;
      hit_end = true;
    } else if (x_ == 0) {
      x_ = LIGHT_COUNT;
      hit_end = true;
    }
    if (hit_end) {
      direction_ = -direction_;
      int old_color = color_;
      do {
	color_ = G35::max_color(rand());
      } while (old_color == color_);
      delay(500);
    }
    lights.set_color(x_, G35::MAX_INTENSITY, color_);
    x_ += direction_;
    delay(BULB_FRAME);
  }
 private:
  uint8_t x_;
  int8_t direction_;
  color_t color_;
};

class FadeInFadeOutSolidColors : public LightProgram {
 public:
  void Do() {
    int new_color = color_;
    do {
      color_ = G35::max_color(rand());
    } while (new_color == color_);

    lights.fill_color(0, LIGHT_COUNT, 0, color_);
    lights.fade_in(5);
    lights.fade_out(5);
  }
 private:
  color_t color_;
};

class BidirectionalWave : public LightProgram {
 public:
  void Init() {
    x_ = HALFWAY_POINT;
  }

  void Do() {
    // With 50 lights, we run into some edge cases because 50 isn't evenly
    // divisible by 4. It's a fairly crazy program to start with, so I'm
    // leaving it like this.
    if (x_ == HALFWAY_POINT) {
      x_ = 0;
      do {
	color_a_ = G35::max_color(rand());
	color_b_ = G35::max_color(rand());
      } while (color_a_ == color_b_);
      do {
	color_c_ = G35::max_color(rand());
	color_d_ = G35::max_color(rand());
      } while (color_c_ == color_d_);
    }
    lights.set_color(x_, G35::MAX_INTENSITY, color_a_);
    lights.set_color(HALFWAY_POINT - 1 - x_, G35::MAX_INTENSITY, color_b_);
    lights.set_color(HALFWAY_POINT + x_, G35::MAX_INTENSITY, color_c_);
    lights.set_color(LAST_LIGHT - x_, G35::MAX_INTENSITY, color_d_);
    ++x_;
    delay(BULB_FRAME);
  }
 private:
  uint8_t x_;
  color_t color_a_;
  color_t color_b_;
  color_t color_c_;
  color_t color_d_;
};

class ChasingSolidColors : public LightProgram {
 public:
  void Init() {
    count_ = 1;
    sequence_ = 0;
  }
  void Do() {
    lights.fill_sequence(0, count_, sequence_, 5, G35::MAX_INTENSITY,
			 G35::max_color);
    if (count_ < LIGHT_COUNT) {
      ++count_;
    } else {
      ++sequence_;
    }
    delay(BULB_FRAME);
  }
 private:
  uint8_t count_;
  uint16_t sequence_;
};

class FadeInFadeOutMultiColors : public LightProgram {
 public:
  void Do() {
    lights.fill_sequence(rand(), 5, 0, G35::max_color);
    lights.fade_in(5);
    lights.fade_out(5);
  }
};

class ChasingTwoColors : public LightProgram {
 public:
  void Init() {
    sequence_ = 0;
  }
  void Do() {
    lights.fill_sequence(sequence_, LIGHT_COUNT / 2, G35::MAX_INTENSITY,
			 G35::rainbow_color);
    sequence_ += LIGHT_COUNT / 2;
    delay(500);
  }
 private:
  uint16_t sequence_;
};

class RandomSparkling : public LightProgram {
 public:
  void Do() {
    lights.fill_random_max(0, LIGHT_COUNT, G35::MAX_INTENSITY);
    delay(1000);
    lights.fill_color(0, LIGHT_COUNT, G35::MAX_INTENSITY, COLOR_BLACK);
    delay(500);
  }
 private:
  color_t color_;
};

class ChasingMultiColors : public LightProgram {
 public:
  void Init() {
    count_ = 1;
    sequence_ = 0;
  }
  void Do() {
    lights.fill_sequence(0, count_, sequence_, 1, G35::MAX_INTENSITY,
			 G35::max_color);
    if (count_ < LIGHT_COUNT) {
      ++count_;
    } else {
      ++sequence_;
    }
    delay(BULB_FRAME);
  }
 private:
  uint8_t count_;
  uint16_t sequence_;
};

color_t red_white_blue(uint16_t sequence) {
  sequence = sequence % 3;
  if (sequence == 0) {
    return COLOR_RED;
  }
  if (sequence == 1) {
    return COLOR_WHITE;
  }
  return COLOR_BLUE;
}

class ChasingWhiteRedBlue : public LightProgram {
 public:
  void Init() {
    count_ = 1;
    sequence_ = 0;
  }
  void Do() {
    lights.fill_sequence(0, count_, sequence_, 3, G35::MAX_INTENSITY,
			 red_white_blue);
    if (count_ < LIGHT_COUNT) {
      ++count_;
    } else {
      ++sequence_;
    }
    delay(BULB_FRAME);
  }
 private:
  uint8_t count_;
  uint16_t sequence_;
};

color_t red_green(uint16_t sequence) {
  return sequence % 2 ? COLOR_RED : COLOR_GREEN;
}

class RedGreenChase : public LightProgram {
 public:
  void Init() {
    count_ = 1;
    sequence_ = 0;
  }
  void Do() {
    lights.fill_sequence(0, count_, sequence_, 5, G35::MAX_INTENSITY, red_green);
    if (count_ < LIGHT_COUNT) {
      ++count_;
    } else {
      ++sequence_;
    }
    delay(BULB_FRAME);
  }
 private:
  uint8_t count_;
  uint16_t sequence_;
};

class Meteorite : public LightProgram {
 public:
  void Init() {
    position_ = LAST_LIGHT + TAIL;
  }

  void Do() {
    if (position_ == LAST_LIGHT + TAIL) {
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
      d_ = rand() % BULB_FRAME + 5;
      colors_[0] = COLOR(r, g, b);
      colors_[1] = COLOR(r * 3 / 4, g * 3 / 4, b * 3 / 4);
      colors_[2] = COLOR(r * 2 / 4, g * 2 / 4, b * 2 / 4);
      colors_[3] = COLOR(r * 1 / 4, g * 1 / 4, b * 1 / 4);
      colors_[4] = COLOR(r * 0 / 4, g * 0 / 4, b * 0 / 4);
    }

    for (int i = 0; i < TAIL; ++i) {
      int pos = position_ - i;
      lights.set_color_if_in_range(pos, G35::MAX_INTENSITY, colors_[i]);
    }
    ++position_;
    delay(d_);
  }
 private:
  static const uint8_t TAIL = 5;

  uint8_t d_;
  int16_t position_;
  color_t colors_[TAIL];
};

class Twinkle : public LightProgram {
 public:
  void Init() {
    lights.fill_random_max(0, LIGHT_COUNT, G35::MAX_INTENSITY);
  }

  void Do() {
    delay(BULB_FRAME);
    lights.set_color(rand() % LIGHT_COUNT, G35::MAX_INTENSITY,
		     G35::max_color(rand()));
  }
};

SteadyWhite steady_white;
CrossOverWave cross_over_wave;
ForwardWave forward_wave;
ChasingRainbow chasing_rainbow;
AlternateDirectionalWave alternate_directional_wave;
FadeInFadeOutSolidColors fade_in_fade_out_solid_colors;
BidirectionalWave bidirectional_wave;
ChasingSolidColors chasing_solid_colors;
FadeInFadeOutMultiColors fade_in_fade_out_multi_colors;
ChasingTwoColors chasing_two_colors;
RandomSparkling random_sparkling;
ChasingMultiColors chasing_multi_colors;
ChasingWhiteRedBlue chasing_white_red_blue;

// Added; not in stock.
Meteorite meteorite;
Twinkle twinkle;
RedGreenChase red_green_chase;

// TODO: if these guys are constructed and destructed as needed, then
// we'll have much more memory to work with per program. The Arduino has
// plenty of memory relative to these programs' needs, so I didn't tackle
// that task.
LightProgram* programs[] = {
  &steady_white,
  &cross_over_wave,
  &forward_wave,
  &chasing_rainbow,
  &alternate_directional_wave,
  &fade_in_fade_out_solid_colors,
  &bidirectional_wave,
  &chasing_solid_colors,
  &fade_in_fade_out_multi_colors,
  &chasing_two_colors,
  &random_sparkling,
  &chasing_multi_colors,
  &chasing_white_red_blue,
  &meteorite,
  &twinkle,
  &red_green_chase
};
#define PROGRAM_COUNT (sizeof(programs) / sizeof (LightProgram*))

void loop() {
  static unsigned long program_switch_millis;
  static uint8_t program_type = PROGRAM_COUNT - 1;
  static LightProgram *program;
  if (millis() > program_switch_millis) {
    program_switch_millis = millis() + PROGRAM_DURATION_SECONDS * 1000;
    if (++program_type == PROGRAM_COUNT) {
      program_type = 0;
    }
    program = programs[program_type];
    program->Init();
  }
  program->Do();
}
