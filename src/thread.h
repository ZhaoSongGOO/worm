// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.
#ifndef __WORM_THREAD_H__
#define __WORM_THREAD_H__
#include <memory>
#include <string>

#include "thread_impl.h"

namespace worm {
class Thread {
 public:
  Thread(const std::string& name, ThreadImpl* impl)
      : name_(name), impl_(impl) {}
  void Start();
  void Stop();
  void PostTask(Task&& task) { impl_->PostTask(std::move(task)); }

 private:
  std::string name_;
  std::unique_ptr<ThreadImpl> impl_;
};
}  // namespace worm

#endif
