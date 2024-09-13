
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

void ThreadPosixImpl::Run() { std::cout << "Run from here\n"; }

void ThreadPosixImpl::Stop() {
  if (thread_handler_) {
    pthread_join(thread_handler_, NULL);
  }
}

}  // namespace worm
