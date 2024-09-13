
#ifndef __WORM_THREAD_IMPL_H__
#define __WORM_THREAD_IMPL_H__

namespace worm {
class ThreadImpl {
 public:
  ThreadImpl() = default;
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual ~ThreadImpl() = default;
  virtual void Run() = 0;
};
}  // namespace worm

#endif
