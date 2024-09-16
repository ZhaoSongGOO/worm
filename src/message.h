// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#ifndef __MESSAGE_H__
#define __MESSAGE_H__
#include <list>
#include <memory>

#include "auto_lock.h"
#include "closure.h"
#include "condition.h"
#include "timer_utils.h"
namespace worm {

class Message {
 public:
  explicit Message(Closure* closure, int interval_time, bool need_loop = false)
      : closure_(closure),
        need_loop_(need_loop),
        interval_time_(interval_time) {
    next_timeout_ = CurrentTimeMillseconds() + interval_time;
  }

  void Run() { closure_->Run(); }

  Message() = default;

  Message(const Message& other) : Message(const_cast<Message&>(other)) {}

  Message(Message& other)
      : closure_(other.closure_),
        interval_time_(other.interval_time_),
        next_timeout_(other.next_timeout_) {}

  Message(Message&& other)
      : interval_time_(other.interval_time_),
        next_timeout_(other.next_timeout_) {
    closure_.reset();
    other.closure_.swap(closure_);
  }

  Message& operator=(const Message& other) {
    closure_ = other.closure_;
    interval_time_ = other.interval_time_;
    next_timeout_ = other.next_timeout_;
    return *this;
  }

  bool operator>=(const Message& other) {
    return next_timeout_ >= other.next_timeout_;
  }

  bool operator==(const Message& other) {
    return next_timeout_ == other.next_timeout_;
  }

  bool operator<(const Message& other) { return !(*this >= other); }

 private:
  std::shared_ptr<Closure> closure_ = nullptr;
  bool need_loop_;
  int interval_time_;
  int next_timeout_;

  friend class MessageHeap;
};
}  // namespace worm

#endif