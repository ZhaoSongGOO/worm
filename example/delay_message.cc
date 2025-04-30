// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#include <unistd.h>

#include <iostream>
#include <memory>

#include "actor.h"
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

class Test {
 public:
  void Say(int x) { std::cout << " Say " << x << " from Test\n"; }
};

int main() {
  worm::ThreadPosixImpl* impl = new worm::ThreadPosixImpl();
  // worm::Thread thread("demo", impl);
  // thread.Start();
  // thread.PostDelay(worm::Bind([](int x) { std::cout << x << std::endl; },
  // 123),
  //                  1000);

  std::shared_ptr<worm::Thread> thread =
      std::make_shared<worm::Thread>("Demo", impl);
  thread->Start();

  std::shared_ptr<worm::Actor<Test>> actor =
      std::make_shared<worm::Actor<Test>>(std::make_unique<Test>(), thread);
  int x = 12345;
  actor->ActDelay([x](auto& imp) { imp->Say(x); }, 1000);

  while (1) {
  }
}
