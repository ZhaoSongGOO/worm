// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#include "message_loop.h"

#include <algorithm>
#include <climits>
#include <iostream>
#include <memory>
#include <vector>

#include "auto_lock.h"
#include "closure.h"
#include "message.h"
namespace worm {

void MessageLoop::Loop() {
  while (1) {
    {
      AutoLock lock(lock_);
      if (stop_) {
        break;
      }

      DoWork();

      if (heap_.NextTimeOut() == ULLONG_MAX) {
        condition_.Wait();
      } else {
        int64_t wait_time = heap_.NextTimeOut() - CurrentTimeMillseconds();
        wait_time = wait_time > 0 ? wait_time : 0;
        condition_.Wait(wait_time);
      }
    }
  }
}

void MessageLoop::DoWork() {
  while (!heap_.IsEmpty() && heap_.NextTimeOut() <= CurrentTimeMillseconds()) {
    auto node = heap_.Pop();
    if (node.need_loop_) {
      uint64_t now = CurrentTimeMillseconds();
      if (node.interval_time_ != 0) {
        uint64_t interval_to_next_fire_time =
            node.interval_time_ -
            (now - node.next_timeout_) % node.interval_time_;
        node.next_timeout_ = now + interval_to_next_fire_time;
      } else {
        node.next_timeout_ = now;
      }
    }

    node.Run();

    if (node.need_loop_) {
      temporary_poped_tasks_.push_back(node);
    }
  }

  for (size_t i = 0; i < temporary_poped_tasks_.size(); ++i) {
    Message node = temporary_poped_tasks_[i];
    heap_.Push(node);
  }

  temporary_poped_tasks_.clear();
}

void MessageLoop::Post(Closure *closure) {
  AutoLock lock(lock_);
  Message message(closure, 0);
  heap_.Push(message);
  condition_.Signal();
}

void MessageLoop::PostDelay(Closure *closure, int interval) {
  AutoLock lock(lock_);
  Message message(closure, interval);
  heap_.Push(message);
  condition_.Signal();
}

void MessageLoop::PostLoop(Closure *closure, int interval) {
  AutoLock lock(lock_);
  Message message(closure, interval, true);
  heap_.Push(message);
  condition_.Signal();
}

void MessageLoop::PostSync(Closure *closure, int interval_time) {
  worm::Lock local_lock;
  worm::Condition local_condition(local_lock);
  Closure *closure_wrapper = worm::Bind(
      [&local_condition, closure = std::shared_ptr<Closure>(closure)]() {
        closure->Run();
        local_condition.Signal();
      });
  Message message(closure_wrapper, interval_time);
  heap_.Push(message);
  local_condition.Wait();
}

void MessageLoop::Stop() {
  AutoLock lock(lock_);
  stop_ = true;
  condition_.Signal();
}

}  // namespace worm
