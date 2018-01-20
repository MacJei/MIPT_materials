#pragma once

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <string>
#include <thread>
#include <queue>

class InactiveBlockingQueueRequest : public std::exception {
  virtual const std::string reason() const noexcept {
    return "Request to the inactive BlockingQueue";
  }
};

template <class T, class Container = std::deque<T>>
class BlockingQueue {
 public:
  explicit BlockingQueue(const std::size_t capacity)
      : capacity_(capacity),
        is_working_(true) {}

  void Put(T&& item) {
    std::unique_lock<std::mutex> lock(mtx_);
    while (queue_.size() >= capacity_ && is_working_.load()) {
      cv_writers_.wait(lock);
    }
    if (!is_working_) {
      throw InactiveBlockingQueueRequest();
    } else {
      queue_.push_back(std::move(item));
      cv_readers_.notify_one();
    }
    return;
  }

  bool Get(T& request_result) {
    std::unique_lock<std::mutex> lock(mtx_);
    while (queue_.empty() && is_working_) {
      cv_readers_.wait(lock);
    }
    if (is_working_ && !queue_.empty()) {
      request_result = std::move(queue_.front());
      queue_.pop_front();
      cv_writers_.notify_one();
      return true;
    }
    return false;
  }

  void Shutdown() {
    is_working_.store(false);
    cv_writers_.notify_all();
    cv_readers_.notify_all();
    return;
  }

 private:
  std::size_t capacity_;
  std::condition_variable cv_readers_;
  std::condition_variable cv_writers_;
  std::mutex mtx_;
  Container queue_;
  std::atomic<bool> is_working_;
};