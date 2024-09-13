// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#include "message_loop.h"

#include <algorithm>

#include "auto_lock.h"
#include "task.h"
namespace worm {

void MessageLoop::Loop() {
  while (1) {
    std::list<Task> tmp;
    size_t size;
    {
      AutoLock lock(lock_);
      if (tasks_.size() == 0) {
        condition_.Wait();
      }
      tmp = std::move(tasks_);
      size = tasks_.size();
    }
    for (auto &task : tmp) {
      task(&size);
    }
  }
}

void MessageLoop::PostTask(Task &&task) {
  AutoLock lock(lock_);
  tasks_.push_back(std::move(task));
  condition_.Signal();
}

}  // namespace worm
