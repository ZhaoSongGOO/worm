#include "thread.h"

namespace worm {

void Thread::Start() { impl_->Start(); }

void Thread::Stop() { impl_->Stop(); }

}  // namespace worm