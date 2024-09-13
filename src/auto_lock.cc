// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#include "auto_lock.h"

namespace worm {
Lock::Lock() { pthread_mutex_init(&mutex_, NULL); }

void Lock::Release() { pthread_mutex_unlock(&mutex_); }

void Lock::Acquire() { pthread_mutex_lock(&mutex_); }

Lock::~Lock() { pthread_mutex_destroy(&mutex_); }
}  // namespace worm
