#include "thread.h"
#include "thread_impl_posix.h"

int main() {
  worm::ThreadPosixImpl* impl = new worm::ThreadPosixImpl();
  worm::Thread thread("demo", impl);
  thread.Start();
  while (1) {
  }
}
