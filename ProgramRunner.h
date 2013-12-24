/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  By Mike Tsao <github.com/sowbug>.

  See README for complete attributions.
*/

#ifndef INCLUDE_G35_PROGRAM_RUNNER_H
#define INCLUDE_G35_PROGRAM_RUNNER_H

#include <LightProgram.h>

// ProgramRunner manages a collection of LightPrograms.
//
// It gives the current program a slice of time to run, decides when it's
// time to switch to the next program, and asks the program_creator callback
// to give it the next program when it's time. In Arduino terms, it's what
// you want to call in your loop() method.
//
// switch_program() is public because the application might sometimes want
// to change programs more frequently, for example if you've implemented
// a remote control receiver.
class ProgramRunner {
 public:
 ProgramRunner(LightProgram* (*program_creator)(uint8_t program_index),
	       uint8_t program_count, uint16_t program_duration_seconds)
   : program_count_(program_count),
    program_duration_seconds_(program_duration_seconds),
    program_index_(program_count_ - 1),
    next_switch_millis_(0),
    program_creator_(program_creator),
    program_(NULL),
    is_switch_time_based_(true) {}

  // Stops automatic, time-based switching, leaving you to call
  // switch_program_to() yourself to switch to specific light programs. Call
  // this once during initialization.
  void disable_time_based_switching() {
    is_switch_time_based_ = false;
  }

  // Calls the correct light program as often as needed (e.g., every few
  // milliseconds or however long the program defines an animation frame to be).
  // You should call this method as often as you can.
  void loop() {
    uint32_t now = millis();
    if (is_switch_time_based() && now >= next_switch_millis_) {
      switch_program();
    } else {
      // This is the first loop() with manual switching. We need to have some
      // program ready at first, so we'll pick the first one. If you don't want
      // this behavior, just call switch_program_to() before your first loop().
      if (program_ == NULL) {
        switch_program_to(0);
      }
    }
    if (now >= next_do_millis_) {
      next_do_millis_ = now + program_->Do();
    }
  }

  // Switches to a specific light program.
  void switch_program_to(uint8_t program_index) {
    uint32_t now = millis();
    if (is_switch_time_based()) {
      next_switch_millis_ = now + (uint32_t)(program_duration_seconds_) * 1000;
    }
    next_do_millis_ = now;
    if (program_ != NULL) {
      delete program_;
    }
    program_index_ = program_index;
    program_ = program_creator_(program_index_);
  }

  // Switches to the next light program according to the program_creator
  // method.
  void switch_program() {
    switch_program_to(++program_index_);
    if (program_index_ == program_count_) {
      program_index_ = 0;
    }
  }

 private:
  bool is_switch_time_based() { return is_switch_time_based_; }

  uint8_t program_count_;
  uint16_t program_duration_seconds_;
  uint8_t program_index_;
  uint32_t next_switch_millis_;
  uint32_t next_do_millis_;
  LightProgram* (*program_creator_)(uint8_t program_index);
  LightProgram* program_;
  bool is_switch_time_based_;
};

#endif  // INCLUDE_G35_PROGRAM_RUNNER_H
