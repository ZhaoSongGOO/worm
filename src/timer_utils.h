// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#ifndef __TIMER_UTILS_H__
#define __TIMER_UTILS_H__

#include <time.h>

#include <cstdint>
namespace worm {
uint64_t CurrentTimeMillseconds();

timespec ToTimeSpecFronNow(uint64_t interval_time);
}  // namespace worm
#endif
