// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#ifndef __MESSAGE_LOOP_H__
#define __MESSAGE_LOOP_H__
#include <list>

#include "auto_lock.h"
#include "condition.h"
#include "task.h"
namespace worm {

class MessageLoop {
 public:
  MessageLoop() : lock_(), condition_(lock_) {}
  void Loop();
  void PostTask(Task&& task);

 private:
  std::list<Task> tasks_;
  Lock lock_;
  Condition condition_;
};
}  // namespace worm

#endif