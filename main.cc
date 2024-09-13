// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#include "thread.h"
#include "thread_impl_posix.h"

int main() {
  worm::ThreadPosixImpl* impl = new worm::ThreadPosixImpl();
  worm::Thread thread("demo", impl);
  thread.Start();
  while (1) {
    thread.PostTask([](void* params) {
      // do something
    });
  }
}
