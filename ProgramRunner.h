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

class ProgramRunner {
 public:
  ProgramRunner(LightProgram* (*program_creator)(uint8_t program_index),
                uint8_t program_count, uint16_t program_duration_seconds)
   : program_count_(program_count),
    program_duration_seconds_(program_duration_seconds),
    program_index_(program_count_ - 1),
    next_switch_millis_(0),
    program_creator_(program_creator_),
    program_(NULL) {}

  void loop() {
    uint32_t now = millis();
    if (now >= next_switch_millis_) {
      switch_program();
    }
    if (now >= next_do_millis_) {
      next_do_millis_ = now + program_->Do();
    }
  }

  void switch_program() {
    uint32_t now = millis();
    next_switch_millis_ = now + program_duration_seconds_ * 1000;
    next_do_millis_ = now;
    if (++program_index_ == program_count_) {
      program_index_ = 0;
    }
    if (program_ != NULL) {
      delete program_;
    }
    program_ = program_creator_(program_index_);
  }

 private:
  uint8_t program_count_;
  uint16_t program_duration_seconds_;
  uint8_t program_index_;
  uint32_t next_switch_millis_;
  uint32_t next_do_millis_;
  LightProgram* (*program_creator_)(uint8_t program_index);
  LightProgram* program_;
};

#endif  // INCLUDE_G35_PROGRAM_RUNNER_H
