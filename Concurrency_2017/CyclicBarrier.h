#pragma once

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>

template <class ConditionVariable = std::condition_variable>
class CyclicBarrier {
 public:
  CyclicBarrier(std::size_t num_threads)
      : num_threads_(num_threads),
        num_approached_threads_(0),
        round_(0) {}

  void Pass() {
    std::atomic<size_t> new_round;
    new_round.store(round_.load());
    std::unique_lock<std::mutex> lock(mtx_);
    ++num_approached_threads_;
    if (num_approached_threads_ == num_threads_) {
      ++round_;
      num_approached_threads_ = 0;
      pass_.notify_all();
    } else {
      while (new_round.load() == round_.load()) {
        pass_.wait(lock);
      }
    }
  }

 private:
  std::size_t num_threads_;
  std::size_t num_approached_threads_;
  std::atomic<size_t> round_;
  std::mutex mtx_;
  ConditionVariable pass_;
};