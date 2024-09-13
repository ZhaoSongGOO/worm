// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#include "thread_impl_posix.h"

#include <pthread.h>

#include <cstddef>
#include <iostream>

#include "thread_impl.h"

namespace worm {

void* ThreadFunc(void* params) {
  ThreadImpl* impl = reinterpret_cast<ThreadImpl*>(params);
  impl->Run();
  return NULL;
}

void ThreadPosixImpl::Start() {
  bool err = pthread_create(&thread_handler_, NULL, ThreadFunc, this);
  if (err) {
    return;
  }
}

void ThreadPosixImpl::Run() { loop_->Loop(); }

void ThreadPosixImpl::Stop() {
  if (thread_handler_) {
    pthread_join(thread_handler_, NULL);
  }
}

}  // namespace worm
