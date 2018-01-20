#pragma once
#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <forward_list>
#include <mutex>
#include <vector>

template <class T, class Hash = std::hash<T>>
class StripedHashSet {
 public:
  explicit StripedHashSet(std::size_t concurrency_level,
                          std::size_t growth_factor = 3,
                          double max_load_factor = 0.75)
      : table_size_(0),
        concurrency_level_(concurrency_level),
        growth_factor_(growth_factor),
        max_load_factor_(max_load_factor),
        buckets_(concurrency_level * 5),
        mtx_(concurrency_level) {}


//-------------------------------------------

  bool Contains(const T& item) {
    std::size_t hash_key = hash_(item);
    std::size_t stripe_index = GetStripeIndex(hash_key);
    std::unique_lock<std::mutex> lock(mtx_[stripe_index]);

    return ContainsWithoutMutex(item);
  }

//--------------------------------------------

  bool Insert(const T& item) {
    std::size_t hash_key = hash_(item);
    std::size_t stripe_index = GetStripeIndex(hash_key);
    std::unique_lock<std::mutex> lock(mtx_[stripe_index]);

    if (ContainsWithoutMutex(item)) {
      return false;
    }

    std::size_t bucket_index = GetBucketIndex(hash_key);
    buckets_[bucket_index].emplace_front(item);
    table_size_.fetch_add(1);

    if (GetCurrentLoading() > max_load_factor_) {
      lock.unlock();
      Expand();
    }

    return true;
  }

//--------------------------------------------

  bool Remove(const T& item) {
    std::size_t hash_key = hash_(item);
    std::size_t stripe_index =  GetStripeIndex(hash_key);
    std::unique_lock<std::mutex> lock(mtx_[stripe_index]);
    if (ContainsWithoutMutex(item)) {
      std::size_t bucket_index = GetBucketIndex(hash_key);
      auto it = std::find(buckets_[bucket_index].begin(),
                          buckets_[bucket_index].end(), item);
      buckets_[bucket_index].remove(*it);
      table_size_.fetch_sub(1);
      return true;
    }
    return false;
  }

//--------------------------------------------

  std::size_t Size() {
    return table_size_.load();
  }

//--------------------------------------------

 private:
  std::atomic<std::size_t> table_size_;
  const std::size_t concurrency_level_;
  const std::size_t growth_factor_;
  double max_load_factor_;
  std::vector<std::forward_list<T>> buckets_;
  std::vector<std::mutex> mtx_;
  std::mutex expand_mtx_;
  Hash hash_;

//--------------------------------------------

  std::size_t GetBucketIndex(const std::size_t hash_key) {
    return hash_key % buckets_.size();
  }

//--------------------------------------------

  std::size_t GetStripeIndex(const std::size_t hash_key) {
    return hash_key % concurrency_level_;
  }

//--------------------------------------------

  std::size_t GetCurrentLoading() {
    return table_size_ / buckets_.size();
  }

//--------------------------------------------

  void Expand() {
    std::unique_lock<std::mutex> expand_lock(expand_mtx_);
    if (GetCurrentLoading() <= max_load_factor_) {
      return;
    }

    std::vector<std::unique_lock<std::mutex>> lock;
    for (std::size_t i = 0; i < concurrency_level_; ++i) {
      lock.emplace_back(mtx_[i]);
    }

    std::vector<std::forward_list<T>> temp_buckets(buckets_.size()
                                                   * growth_factor_);
    for (std::size_t i = 0; i < buckets_.size(); ++i) {
      for (auto iter = buckets_[i].begin(); iter != buckets_[i].end(); ++iter) {
        std::size_t hash_key = hash_(*iter);
        std::size_t new_bucket_index = hash_key % temp_buckets.size();
        temp_buckets[new_bucket_index].emplace_front(*iter);
      }
    }
    std::swap(temp_buckets, buckets_);
  }

//--------------------------------------------

  bool ContainsWithoutMutex(const T& item) {
    std::size_t hash_key = hash_(item);
    std::size_t bucket_index = GetBucketIndex(hash_key);
    return std::find(buckets_[bucket_index].begin(),
                     buckets_[bucket_index].end(),
                     item) != buckets_[bucket_index].end();
  }
};

template <typename T>
using ConcurrentSet = StripedHashSet<T>;