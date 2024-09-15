// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#ifndef __TASK_HEAP_H__
#define __TASK_HEAP_H__

#include <vector>

#include "auto_lock.h"
#include "task.h"
namespace worm {
class TaskHeap {
 public:
  TaskHeap() = default;
  ~TaskHeap() {}

  // uint64_t NextTimeout() {
  //   return !min_heap_.empty() ? min_heap_[0]->next_timeout_ : ULLONG_MAX;
  // }

  bool IsEmpty() { return min_heap_.empty(); }

  // void Remove(Task & node);

  Task Pop();

  void Push(Task node);

 private:
  void ShiftUp(int start);
  void ShiftDown(int start);
  std::vector<Task> min_heap_;
};
}  // namespace worm

#endif
