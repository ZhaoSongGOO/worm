// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#ifndef __CONDITION_H__
#define __CONDITION_H__
#include <pthread.h>

#include "auto_lock.h"
namespace worm {
class Condition {
 public:
  Condition(Lock& lock);
  ~Condition();
  void Wait();
  void Wait(u_int64_t time);
  void Signal();
  void BroadCast();

 private:
  Lock& lock_;
  pthread_cond_t condition_;
};
}  // namespace worm
#endif
