// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#ifndef FIBER_FIBER_ACTOR_H
#define FIBER_FIBER_ACTOR_H

#include <memory>

#include "thread.h"

namespace worm {
template <typename T>
class Actor : public std::enable_shared_from_this<Actor<T>> {
 public:
  Actor(std::unique_ptr<T> impl, const std::shared_ptr<Thread>& thread,
             bool enable = true)
      : impl_(std::move(impl)), thread_(thread), enable_(enable) {}

  ~Actor() {}

  template <typename F>
  void Act(F&& func) {
    if (!enable_) {
      return;
    }

    thread_->Post(Bind([self = this->shared_from_this(),
                            func = std::forward<F>(func)]() mutable {
      self->Invoke(std::forward<F>(func));
    }));
  }

  template <typename F>
  void ActDelay(F&& func, int64_t delay_time) {
    if (!enable_) {
      return;
    }

    thread_->PostDelay(Bind([self = this->shared_from_this(),
                                   func = std::forward<F>(func)]() mutable {
                               self->Invoke(std::forward<F>(func));
                             }),
                             delay_time);
  }

 private:
  template <typename F>
  void Invoke(F&& func) {
    if (impl_ != nullptr) {
      func(impl_);
    }
  }
  std::unique_ptr<T> impl_;
  std::shared_ptr<Thread> thread_;
  bool enable_ = true;
};
}  // namespace fiber

#endif  // FIBER_FIBER_ACTOR_H
