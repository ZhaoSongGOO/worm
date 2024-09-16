// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#ifndef __MESSAGE_LOOP_H__
#define __MESSAGE_LOOP_H__
#include <list>
#include <memory>

#include "auto_lock.h"
#include "closure.h"
#include "condition.h"
#include "message.h"
#include "message_heap.h"
namespace worm {

class MessageLoop {
 public:
  MessageLoop() : lock_(), condition_(lock_) {}
  void Loop();
  void Post(Closure* closure);
  void PostDelay(Closure* closure, int interval);
  void PostLoop(Closure* closure, int interval);
  void Stop();

 private:
  MessageHeap heap_;
  Lock lock_;
  Condition condition_;
  bool stop_ = false;
  void DoWork();
  std::vector<Message> temporary_poped_tasks_;
};
}  // namespace worm

#endif