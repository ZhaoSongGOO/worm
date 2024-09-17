// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#include <unistd.h>

#include <iostream>

#include "auto_lock.h"
#include "closure.h"
#include "condition.h"
#include "thread.h"
#include "thread_impl_posix.h"

void wait(worm::Thread& thread) {
  worm::Lock local_lock;
  worm::Condition local_condition(local_lock);
  thread.Post(worm::Bind([&local_condition]() { local_condition.Signal(); }));
  local_condition.Wait();
}

int main() {
  worm::ThreadPosixImpl* impl = new worm::ThreadPosixImpl();
  worm::Thread thread("demo", impl);
  thread.Start();
  thread.PostSync(worm::Bind([](int x) { std::cout << x << std::endl; }, 123),
                  3000);
}
