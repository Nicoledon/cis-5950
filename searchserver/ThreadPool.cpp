/*
 * Copyright ©2025 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Pennsylvania
 * CIT 5950 for use solely during Spring Semester 2025 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <iostream>
#include <unistd.h>

#include "./ThreadPool.hpp"

namespace searchserver {

// This is the thread start routine, i.e., the function that threads
// are born into.
void *thread_loop(void *t_pool);

ThreadPool::ThreadPool(size_t num_threads)
    : q_lock_(), q_cond_(), work_queue_(), killthreads_(false),
      num_threads_(num_threads), thread_vec_(num_threads) {
  // Initialize our member variables.
  // TODO
  pthread_mutex_init(&q_lock_, NULL);
  pthread_cond_init(&q_cond_, NULL);
  for (size_t i = 0; i < num_threads; i++) {
    pthread_t thd;
    thread_vec_.push_back(thd);
  }
  for (size_t i = 0; i < num_threads; i++) {
    pthread_create(&thread_vec_[i], NULL, thread_loop, this);
  }
}

ThreadPool::~ThreadPool() {
  // TODO
  killthreads_ = true;
  for (size_t i = 0; i < this->num_threads_; i++) {
    pthread_join(this->thread_vec_[i], NULL);
  }
  while (!this->work_queue_.empty()) {
    auto val = this->work_queue_.at(0);
    this->work_queue_.pop_front();
    val.func_(val.arg_);
  }
  pthread_mutex_destroy(&q_lock_);
  pthread_cond_destroy(&q_cond_);
}

// Enqueue a Task for dispatch.
void ThreadPool::dispatch(Task t) {
  // TODO
  pthread_mutex_lock(&this->q_lock_);
  this->work_queue_.push_back(t);
  pthread_cond_broadcast(&this->q_cond_);
  pthread_mutex_unlock(&this->q_lock_);
}

// This is the main loop that all worker threads are born into.  They
// wait for a signal on the work queue condition variable, then they
// grab work off the queue.  Threads return (i.e., kill themselves)
// when they notice that killthreads_ is true.
void *thread_loop(void *t_pool) {
  // TODO
  ThreadPool *t = reinterpret_cast<ThreadPool *>(t_pool);
  while (1) {
    pthread_mutex_lock(&t->q_lock_);
    if (t->killthreads_) {
      pthread_mutex_unlock(&t->q_lock_);
      break;
    }
    while (t->work_queue_.size() == 0) {
      pthread_cond_wait(&t->q_cond_, &t->q_lock_);
    }
    auto val = t->work_queue_.at(0);
    t->work_queue_.pop_front();
    pthread_mutex_unlock(&t->q_lock_);
    val.func_(val.arg_);
  }
  return nullptr;
}
} // namespace searchserver
