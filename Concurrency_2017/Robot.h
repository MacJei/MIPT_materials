#pragma once
#include <condition_variable>
#include <iostream>
#include <mutex>
enum next_leg {left_leg, right_leg};
class Robot {
 public:
  Robot() : next_leg_(left_leg) {}

  void StepLeft() {
    std::unique_lock<std::mutex> lock(mtx_);
    while (next_leg_ != left_leg) {
      cond_var_.wait(lock);
    }

    std::cout << "left" << std::endl;
    next_leg_ = right_leg;
    cond_var_.notify_one();
  }

  void StepRight() {
    std::unique_lock<std::mutex> lock(mtx_);
    while (next_leg_ != right_leg) {
      cond_var_.wait(lock);
    }
    std::cout << "right" << std::endl;
    next_leg_ = left_leg;
    cond_var_.notify_one();
  }

 private:
  std::condition_variable cond_var_;
  std::mutex mtx_;
  next_leg next_leg_;

};