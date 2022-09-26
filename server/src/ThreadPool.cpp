#include "../include/ThreadPool.hpp"

/* PUBLIC METHODS IMPLEMENTATIONS */

ThreadPool::ThreadPool(uint16_t thread_count) {
  SetupThreadPool(thread_count);
}

ThreadPool::~ThreadPool() {
  pool_terminate_ = true;
  JoinThreadPool();
}

void ThreadPool::JoinThreadPool() {
  for(auto& thread : thread_pool_) {
    thread.join();
  }
}

uint16_t ThreadPool::GetThreadCount() const {
  return thread_pool_.size();
}

void ThreadPool::StopThreadPool() {
  pool_terminate_ = true;
  JoinThreadPool();
}

void ThreadPool::StartThreadPool(uint thread_count) {
  if(!pool_terminate_) {
     return;
  }
  pool_terminate_ = false;
  SetupThreadPool(thread_count);
}

/* PRIVATE METHODS IMPLEMENTIONS */

void ThreadPool::SetupThreadPool(uint16_t thread_count) {
  thread_pool_.clear();
  for(uint i = 0; i < thread_count; ++i) {
    thread_pool_.emplace_back(&ThreadPool::ThreadLoop, this);
  }
}

void ThreadPool::ThreadLoop() {
  std::function<void()> job;
  while (!pool_terminate_) {
    {
      std::unique_lock<std::mutex> lock(jobs_queue_mtx_);
      mutex_cond_.wait(lock, [this]() {
        return !jobs_queue_.empty() || pool_terminate_;
      });
      if(pool_terminate_) {
        return;
      }
      job = jobs_queue_.front();
      jobs_queue_.pop();
    }
    job();
  }
}
