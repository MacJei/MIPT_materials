#pragma once
#include <condition_variable>
#include <iostream>
#include <mutex>

class Robot {
 public:
  Robot(const std::size_t num_foots)
      : next_foot_(0),
        num_foots_(num_foots) {}

  void Step(const std::size_t foot) {
    std::unique_lock<std::mutex> lock(mtx_);
    while (next_foot_ != foot) {
      cond_var_.wait(lock);
    }

    std::cout << "foot " << foot << std::endl;
    next_foot_ = (next_foot_ + 1) % num_foots_;
    cond_var_.notify_all();
    return;
  }

 private:
  std::condition_variable cond_var_;
  std::size_t next_foot_;
  std::mutex mtx_;
  const std::size_t num_foots_;
};