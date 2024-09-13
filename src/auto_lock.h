// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#ifndef __AUTO_LOCK_H__
#define __AUTO_LOCK_H__

#include <pthread.h>

namespace worm {

class Lock {
 public:
  Lock();
  ~Lock();
  void Acquire();
  void Release();

  friend class Condition;

 private:
  pthread_mutex_t mutex_;
};

class AutoLock {
 public:
  explicit AutoLock(Lock& lock) : lock_(lock) { lock_.Acquire(); }
  ~AutoLock() { lock_.Release(); }

 private:
  Lock& lock_;
};

}  // namespace worm

#endif
