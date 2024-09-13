
#ifndef __WORM_THREAD_H__
#define __WORM_THREAD_H__
#include <memory>
#include <string>

#include "thread_impl.h"

namespace worm {
class Thread {
 public:
  Thread(const std::string& name, ThreadImpl* impl)
      : name_(name), impl_(impl) {}
  void Start();
  void Stop();

 private:
  std::string name_;
  std::unique_ptr<ThreadImpl> impl_;
};
}  // namespace worm

#endif
