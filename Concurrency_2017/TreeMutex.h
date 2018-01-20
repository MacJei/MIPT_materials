#pragma once
#include <array>
#include <atomic>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

class PetersonMutex {
 public:
  PetersonMutex() {
    want_[0].store(false);
    want_[1].store(false);
    victim_.store(0);
  }

  PetersonMutex(const PetersonMutex& a) {
    want_[0].store(a.want_[0]);
    want_[1].store(a.want_[1]);
    victim_.store(a.victim_);
  }

  void Lock(std::size_t t) {
    want_[t].store(true);
    victim_.store(t);
    while(want_[1 - t].load() && victim_.load() == t) {
      std::this_thread::yield();
    }
    return;
  }

  void Unlock(std::size_t t) {
    want_[t].store(false);
    return;
  }

 private:
  std::array<std::atomic<bool>, 2> want_;
  std::atomic<size_t> victim_;
};

class TreeMutex {
 public:
  TreeMutex(std::size_t num_threads) {
    tree_depth = ceil(log2(num_threads));
    size = pow(2, tree_depth) - 1;
    for (std::size_t i = 0; i < size; i++) {
      mutex_tree.emplace_back();
    }
  }

  void lock(std::size_t current_thread_id) {
    std::size_t index = size + current_thread_id;
    while (index != 0) {
      mutex_tree[(index - 1) >> 1].Lock((index + 1) % 2);
      index = (index - 1) >> 1;
    }
    return;
  }

  void unlock(std::size_t current_thread_id) {
    std::size_t index = 0;
    for (int i = (int) tree_depth - 1; i >= 0; --i) {
      mutex_tree[index].Unlock((current_thread_id >> i) % 2);
      index = index * 2 + 1 + ((current_thread_id >> i) % 2);
     }
     return;
  }

 private:
  std::vector<PetersonMutex> mutex_tree;
  std::size_t size;
  std::size_t tree_depth;
};
