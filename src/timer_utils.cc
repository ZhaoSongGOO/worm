// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#include "timer_utils.h"

#include <sys/time.h>

#include <chrono>
#include <cstdlib>

namespace worm {
uint64_t CurrentTimeMillseconds() {
  const auto time = std::chrono::steady_clock::now().time_since_epoch();
  return std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
}

timespec ToTimeSpecFronNow(uint64_t interval_time) {
  timespec out_time;
  struct timeval now;
  uint64_t absmsec;
  gettimeofday(&now, nullptr);

  absmsec = now.tv_sec * 1000LL + now.tv_usec / 1000LL;

  out_time.tv_sec = static_cast<time_t>(absmsec / 1000LL);
  out_time.tv_nsec = static_cast<long>(absmsec % 1000LL * 1000000LL);
  return out_time;
}
}  // namespace worm
