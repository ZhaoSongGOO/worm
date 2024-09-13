
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
