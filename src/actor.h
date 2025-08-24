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

  /**
   * @brief  用来将用户任务交给对应的线程异步执行。
   * @param func 用户自己实现的任务
   */
  // F&& 通用引用 与 引用折叠。& && -> &, && && -> &&
  // 假设 func 的类型是 X
  // 用户传入左值，F 解析成左值引用 X &，func 的类型是 X & && -> X& 左值引用。
  // 用户传入右值，F 解析成非引用类型 X，func 的类型是 X && 右值引用。
  template <typename F>
  void Act(F&& func) {
    if (!enable_) {
      return;
    }
    // std::forward 用来完美转发，因为 func
    // 本身是一个左值。想要在捕获阶段转换成对应的值，就需要使用 forward.
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
      // func 接收的是个左值。
      func(impl_);
    }
  }
  std::unique_ptr<T> impl_;
  std::shared_ptr<Thread> thread_;
  bool enable_ = true;
};
}  // namespace worm

#endif  // FIBER_FIBER_ACTOR_H
