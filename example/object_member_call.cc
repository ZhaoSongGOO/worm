// Copyright 2025 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#include <unistd.h>

#include <iostream>

#include "closure.h"
#include "thread.h"
#include "thread_impl_posix.h"

class Object {
 public:
  void action(int x) { std::cout << "message from " << x << std::endl; }
};

int main() {
  worm::ThreadPosixImpl* impl = new worm::ThreadPosixImpl();
  worm::Thread thread("demo", impl);
  thread.Start();
  Object* object = new Object();

  thread.PostLoop(worm::BindMember(object, &Object::action, 123), 1000);

  while (1) {
  }
}
