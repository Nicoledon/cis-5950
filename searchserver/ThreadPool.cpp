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

#include <unistd.h>
#include <iostream>

#include "./ThreadPool.hpp"

namespace searchserver {

// This is the thread start routine, i.e., the function that threads
// are born into.
void *thread_loop(void *t_pool);

ThreadPool::ThreadPool(size_t num_threads) : q_lock_(), q_cond_(), work_queue_(), killthreads_(false), num_threads_(num_threads), thread_vec_(num_threads) {
  // Initialize our member variables.

  // TODO
}

ThreadPool:: ~ThreadPool() {
  // TODO
}

// Enqueue a Task for dispatch.
void ThreadPool::dispatch(Task t) {
  // TODO
}

// This is the main loop that all worker threads are born into.  They
// wait for a signal on the work queue condition variable, then they
// grab work off the queue.  Threads return (i.e., kill themselves)
// when they notice that killthreads_ is true.
void *thread_loop(void *t_pool) {
  // TODO
  return nullptr;
}

}  // namespace searchserver
