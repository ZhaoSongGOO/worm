// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#include "task_heap.h"

#include "task.h"

namespace worm {
Task TaskHeap::Pop() {
  if (min_heap_.empty()) {
    return nullptr;
  }
  Task node = min_heap_[0];
  min_heap_[0] = min_heap_.back();
  min_heap_.pop_back();
  ShiftDown(0);
  return node;
}

// void TaskHeap::Remove(Task &node) {
//   int index = -1;
//   for (int i = 0; i < min_heap_.size(); ++i) {
//     if (min_heap_[i] == node) {
//       min_heap_[i] = min_heap_.back();
//       min_heap_.pop_back();
//       index = i;
//       break;
//     }
//   }

//   if (index != -1) {
//     ShiftDown(index);
//   }
// }

void TaskHeap::Push(Task node) {
  min_heap_.push_back(node);
  ShiftUp(min_heap_.size() - 1);
}

void TaskHeap::ShiftUp(int start) {
  int current = start;
  int parent = (current - 1) / 2;
  while (current > 0 &&
         min_heap_[current]->next_timeout_ < min_heap_[parent]->next_timeout_) {
    auto node = min_heap_[current];
    min_heap_[current] = min_heap_[parent];
    min_heap_[parent] = node;
    current = parent;
    parent = (current - 1) / 2;
  }
}

void TaskHeap::ShiftDown(int start) {
  if (min_heap_.empty()) {
    return;
  }

  while (true) {
    int left_child = start * 2 + 1;
    int right_child = start * 2 + 2;
    int index = 0;
    if (right_child <= min_heap_.size() - 1) {
      if (min_heap_[start]->next_timeout_ <=
              min_heap_[left_child]->next_timeout_ &&
          min_heap_[start]->next_timeout_ <=
              min_heap_[right_child]->next_timeout_) {
        break;
      } else {
        if (min_heap_[left_child]->next_timeout_ <
            min_heap_[right_child]->next_timeout_) {
          index = left_child;
        } else {
          index = right_child;
        }
      }
    } else if (left_child == min_heap_.size() - 1) {
      if (min_heap_[left_child]->next_timeout_ <
          min_heap_[start]->next_timeout_) {
        index = left_child;
      } else {
        break;
      }
    } else {
      break;
    }
    auto node = min_heap_[start];
    min_heap_[start] = min_heap_[index];
    min_heap_[index] = node;
    start = index;
  }
}

}  // namespace worm
