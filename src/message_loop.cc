// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#include "message_loop.h"

#include <algorithm>
#include <memory>

#include "auto_lock.h"
#include "closure.h"
namespace worm {

void MessageLoop::Loop() {
  while (1) {
    std::list<std::shared_ptr<Closure>> tmp;
    size_t size;
    {
      AutoLock lock(lock_);
      if (stop_) {
        break;
      }
      if (tasks_.size() == 0) {
        condition_.Wait();
        if (stop_) {
          break;
        }
      }
      tmp = std::move(tasks_);
      size = tasks_.size();
    }
    for (auto &task : tmp) {
      task->Run();
    }
  }
}

void MessageLoop::Post(Closure *closure) {
  AutoLock lock(lock_);
  tasks_.push_back(std::shared_ptr<Closure>(closure));
  condition_.Signal();
}

void MessageLoop::Stop() {
  AutoLock lock(lock_);
  stop_ = true;
  condition_.Signal();
}

}  // namespace worm
