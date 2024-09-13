// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.
#ifndef __WORM_THREAD_IMPL_POSIX_H__
#define __WORM_THREAD_IMPL_POSIX_H__

#include <pthread.h>

#include "thread_impl.h"

namespace worm {
class ThreadPosixImpl : public ThreadImpl {
 public:
  ThreadPosixImpl() = default;
  void Start() override;
  void Stop() override;
  virtual ~ThreadPosixImpl() = default;
  void Run() override;

 private:
  pthread_t thread_handler_;
};
}  // namespace worm

#endif
