// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#include "message_loop.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "auto_lock.h"
#include "closure.h"
#include "message.h"
namespace worm {

void MessageLoop::Loop() {
  while (1) {
    std::vector<Message> tmp;
    size_t size;
    {
      AutoLock lock(lock_);
      if (stop_) {
        break;
      }
      if (heap_.IsEmpty()) {
        condition_.Wait();
        if (stop_) {
          break;
        }
      }
      while (!heap_.IsEmpty()) {
        tmp.push_back(heap_.Pop());
      }
    }
    for (auto &message : tmp) {
      message.Run();
    }
  }
}

void MessageLoop::Post(Closure *closure) {
  AutoLock lock(lock_);
  Message message(closure, 0);
  heap_.Push(message);
  condition_.Signal();
}

void MessageLoop::Stop() {
  AutoLock lock(lock_);
  stop_ = true;
  condition_.Signal();
}

}  // namespace worm
