// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.
#ifndef __WORM_THREAD_H__
#define __WORM_THREAD_H__
#include <memory>
#include <string>

#include "closure.h"
#include "thread_impl.h"

namespace worm {
class Thread {
 public:
  Thread(const std::string& name, ThreadImpl* impl)
      : name_(name), impl_(impl) {}
  void Start();
  void Stop();
  void Post(Closure* closure) { impl_->Post(closure); }
  void PostDelay(Closure* closure, int interval_time) {
    impl_->PostDelay(closure, interval_time);
  }
  void PostLoop(Closure* closure, int interval_time) {
    impl_->PostLoop(closure, interval_time);
  }

  void PostSync(Closure* closure, int interval_time = 0) {
    impl_->PostSync(closure, interval_time);
  }

 private:
  std::string name_;
  std::unique_ptr<ThreadImpl> impl_;
};
}  // namespace worm

#endif
