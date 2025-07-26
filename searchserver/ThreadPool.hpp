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

#ifndef THREADPOOL_HPP_
#define THREADPOOL_HPP_

extern "C" {
  #include <pthread.h>  // for the pthread threading/mutex functions
}

#include <cstdint>  // for uint32_t, etc.
#include <deque>    // for std::deque
#include <vector>   // for std::vector

namespace searchserver {

// A ThreadPool is, well, a pool of threads. ;)  A ThreadPool is an
// abstraction that allows customers to dispatch tasks to a set of
// worker threads.  Tasks are queued, and as a worker thread becomes
// available, it pulls a task off the queue and invokes a function
// pointer in the task to process it.  When it is done processing the
// task, the thread returns to the pool to receive and process the next
// available task.
class ThreadPool {
 public:
  // Construct a new ThreadPool with a certain number of worker
  // threads.
  //
  // Arguments:
  //
  //  - num_threads:  the number of threads in the pool.
  explicit ThreadPool(size_t num_threads);

  // destructs the theadpool
  // makes sure any threads are joined in
  // if there is any work left in the queue, the destructor will process them.
  ~ThreadPool();

  // This inner struct defines what a Task is.  A worker thread will
  // pull a task off the task queue and invoke the thread_task_fn
  // function pointer inside of it, passing it the task_arg as an
  // argument.
  //
  // argument is a void* to support "generic" arguments similar to pthread
  // (Note: there are nicer ways to handle this in C++ but this is probably
  //  simpler for what we have covered in class)
  typedef void (*thread_task_fn)(void *arg);

  struct Task {
    // The dispatch function.
    thread_task_fn func_;
    void* arg_;
  };

  // Customers use dispatch() to enqueue a Task for dispatch to a
  // worker thread.
  void dispatch(Task t);

  // The following fields are public so that
  // the worker threads can easily get access to these

  // A lock and condition variable that worker threads and the
  // dispatch function use to guard the Task queue.
  pthread_mutex_t q_lock_;
  pthread_cond_t  q_cond_;

  // The queue of Tasks waiting to be dispatched to a worker thread.
  std::deque<Task> work_queue_;

  // This should be set to "true" when it is time for the worker
  // threads to kill themselves, i.e., when the ThreadPool is
  // destroyed.  A worker thread will check this variable before
  // picking up its next piece of work; if it is true, the worker
  // threads will kill themselves off.
  bool killthreads_;

  // This variable stores how many threads exist.
  uint32_t num_threads_;

  // disable move and copying otherwise this becomes a headache.
  ThreadPool& operator=(const ThreadPool& other) = delete;
  ThreadPool& operator=(ThreadPool&& other) = delete;
  ThreadPool(const ThreadPool& other) = delete;
  ThreadPool(ThreadPool&& other) = delete;

 private:
  // The pthreads pthread_t structures representing each thread.
  std::vector<pthread_t> thread_vec_;
};

}  // namespace searchserver

#endif  // THREADPOOL_HPP_
