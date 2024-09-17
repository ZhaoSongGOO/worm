// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.
#ifndef __WORM_THREAD_IMPL_H__
#define __WORM_THREAD_IMPL_H__

#include <memory>

#include "message_loop.h"
#include "task.h"
namespace worm {
class ThreadImpl {
 public:
  ThreadImpl() : loop_(new MessageLoop()) {}
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual ~ThreadImpl() = default;
  virtual void Run() = 0;
  void Post(Closure* closure) { loop_->Post(closure); }
  void PostDelay(Closure* closure, int interval) {
    loop_->PostDelay(closure, interval);
  }
  void PostLoop(Closure* closure, int interval) {
    loop_->PostLoop(closure, interval);
  }

  void PostSync(Closure* closure, int interval_time = 0) {
    loop_->PostSync(closure, interval_time);
  }

 protected:
  std::unique_ptr<MessageLoop> loop_ = nullptr;
};
}  // namespace worm

#endif
