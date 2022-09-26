#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
  ThreadPool(uint16_t thread_count = std::thread::hardware_concurrency());

  ~ThreadPool();

  template<typename Func>
  void EnqueueJob(Func job);

  template<typename Func, typename... Arg>
  void EnqueueJob(const Func& job, const Arg&... args);

  void JoinThreadPool();

  uint16_t GetThreadCount() const;

  void StopThreadPool();

  void StartThreadPool(uint thread_count = std::thread::hardware_concurrency());

private: 
  std::vector<std::thread> thread_pool_;
  std::queue<std::function<void()>> jobs_queue_;
  std::mutex jobs_queue_mtx_;
  std::condition_variable mutex_cond_;
  std::atomic<bool> pool_terminate_ = {false};

  void SetupThreadPool(uint16_t thread_count);

  void ThreadLoop();
};

template<typename Func>
void ThreadPool::EnqueueJob(Func job) {
  if(pool_terminate_) {
    return;
  }
  {
    std::unique_lock<std::mutex> lock(jobs_queue_mtx_);
    jobs_queue_.push(std::function<void()>(job));
  }
  mutex_cond_.notify_one();
}

template<typename Func, typename... Arg>
void ThreadPool::EnqueueJob(const Func& job, const Arg&... args) {
  EnqueueJob([job, args...]{job(args...);});
}

#endif /* THREADPOOL_HPP */