#pragma once

#include <atomic>
#include <thread>

template <class T>
struct Node {
  T data_;
  std::atomic<Node*> next_;

  Node(T item)
      : data_(item),
        next_(nullptr) {}
};


template <class T>
class LockFreeStack {
 public:
  LockFreeStack()
      : top_(nullptr),
        junk_top_(nullptr) {}

  ~LockFreeStack() {
    Node<T>* top_to_delete = top_.load();
    Node<T>* junk_to_delete = junk_top_.load();
    DeleteStack(top_to_delete);
    DeleteStack(junk_to_delete);
  }

  void Push(T item) {
    Node<T>* cur_top = top_.load();
    Node<T>* new_item = new Node<T>(item);
    new_item->next_ = cur_top; //???
    while (!top_.compare_exchange_weak(cur_top, new_item)) {
      new_item->next_ = cur_top;
    }
    return;
  }


  bool Pop(T& item) {
    Node<T>* cur_top = top_.load();
    while (true) {
      if (cur_top == nullptr) {
        return false;
      }
      if (top_.compare_exchange_weak(cur_top, cur_top->next_)) {
        item = cur_top->data_;
        PushToJunk(cur_top);
        return true;
      }
    }
  }

 private:
  std::atomic<Node<T>*> top_;
  std::atomic<Node<T>*> junk_top_;

  void PushToJunk(Node<T>* junk_node) {
    Node<T>* cur_junk_top = junk_top_.load();
    junk_node->next_ = cur_junk_top;
    while (!junk_top_.compare_exchange_weak(cur_junk_top, junk_node)) {
      junk_node->next_ = cur_junk_top;
    }
    return;
  }

  void DeleteStack(Node<T>* top_stack) {
    Node<T>* cur_top = top_stack;
    while (cur_top != nullptr) {
      Node<T>* temp_top = cur_top;
      cur_top = cur_top->next_;
      delete temp_top;
    }
    delete cur_top;
  }
};

template <class T>
using ConcurrentStack = LockFreeStack<T>;