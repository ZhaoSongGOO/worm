// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#ifndef __MESSAGE_HEAP_H__
#define __MESSAGE_HEAP_H__

#include <vector>

#include "message.h"
namespace worm {
class MessageHeap {
 public:
  MessageHeap() {}
  ~MessageHeap() {}

  uint64_t NextTimeOut() {
    return !min_heap_.empty() ? min_heap_[0].next_timeout_ : ULLONG_MAX;
  }

  bool IsEmpty() { return min_heap_.empty(); }

  void Remove(Message& node);

  Message Pop();

  Message& Top() { return min_heap_[0]; }

  void Push(Message& node);

 private:
  void ShiftUp(int start);
  void ShiftDown(int start);
  Lock lock_;
  std::vector<Message> min_heap_;
};
}  // namespace worm

#endif
