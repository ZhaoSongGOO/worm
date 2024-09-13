// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#include "condition.h"

#include "timer_utils.h"

namespace worm {
Condition::Condition(Lock &lock) : lock_(lock) {
  pthread_cond_init(&condition_, NULL);
}

Condition::~Condition() { pthread_cond_destroy(&condition_); }

void Condition::Wait() { pthread_cond_wait(&condition_, &lock_.mutex_); }

void Condition::Wait(u_int64_t time) {
  timespec next_time = ToTimeSpecFronNow(time);
  pthread_cond_timedwait(&condition_, &lock_.mutex_, &next_time);
}

void Condition::Signal() { pthread_cond_signal(&condition_); }

void Condition::BroadCast() { pthread_cond_broadcast(&condition_); }
}  // namespace worm
