#pragma once
#include <condition_variable>
#include <iostream>
#include <mutex>

enum work_to_do {nothing, something};

class Semaphore {
 public:
  Semaphore(const work_to_do work)
      : work_(work) {}

  void Wait() {
    std::unique_lock<std::mutex> lock(mtx_);
    while (work_ == nothing) {
      cond_var_.wait(lock);
    }
    work_ = nothing; //work is done
  }

  void Signal() {
    std::unique_lock<std::mutex> lock(mtx_);
    work_ = something;
    cond_var_.notify_one();
  }


 private:
  work_to_do work_;
  std::condition_variable cond_var_;
  std::mutex mtx_;
};

//----------------------------------------------

class Robot {
 public:
  Robot()
      : left_sem_(something),
        right_sem_(nothing) {}


  void StepLeft() {
    left_sem_.Wait();
    std::cout<< "left" << std::endl;
    right_sem_.Signal();
    return;
  }

  void StepRight() {
    right_sem_.Wait();
    std::cout<< "right" << std::endl;
    left_sem_.Signal();
    return;
  }

 private:
  Semaphore left_sem_;
  Semaphore right_sem_;
};