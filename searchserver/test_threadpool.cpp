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


#include "./ThreadPool.hpp"
#include "./catch.hpp"


using searchserver::ThreadPool;


uint32_t workcount = 0;
static pthread_mutex_t mtx;

// This is the function that each dispatched thread from the thread
// pool is sent to execute.
void TestTaskFn(void *arg) {
  pthread_mutex_lock(&mtx);
  workcount++;
  pthread_mutex_unlock(&mtx);

  if (workcount % 5 == 1) {
    usleep(250000);  // 0.25s
  }
}

TEST_CASE("Basic", "[Test_ThreadPool]") {
  pthread_mutex_init(&mtx, nullptr);
  ThreadPool *tp = new ThreadPool(10);

  // Try dispatching some work.  Make sure we dispatch enough that
  // there will be a queue of pending tasks in the threadpool, so
  // that we can test the "delete before all tasks are done" case.
  for (int i = 0; i < 300; i++) {
    ThreadPool::Task next_t = {TestTaskFn, nullptr};

    tp->dispatch(next_t);
  }
  usleep(1250000);  // 1.25s

  // Make sure that there are still tasks pending.
  REQUIRE(static_cast<uint32_t>(300) > workcount);

  // Kill off the threadpool, which should force the rest of the
  // pending tasks to be finished serially.
  delete tp;

  // Make sure all 300 tasks finished successfully.
  REQUIRE(static_cast<uint32_t>(300) == workcount);
  pthread_mutex_destroy(&mtx);
}

static std::string str;
static pthread_mutex_t lock;

struct ConcurrentTaskArg {
  char message;
  int secs;
};

void TestConcurrentTaskFn(void* arg) {
  auto taskarg = reinterpret_cast<ConcurrentTaskArg*>(arg);
  if (taskarg->secs > 0) {
    sleep(taskarg->secs);
  }

  pthread_mutex_lock(&lock);
  str += taskarg->message;
  pthread_mutex_unlock(&lock);

  delete taskarg;
}


TEST_CASE("Concurrent", "[Test_ThreadPool]") {
  pthread_mutex_init(&lock, nullptr);
  ThreadPool *tp = new ThreadPool(2);

  ConcurrentTaskArg* arg0 = new ConcurrentTaskArg();
  arg0->message = 'A';
  arg0->secs    =  0;

  ConcurrentTaskArg* arg1 = new ConcurrentTaskArg();
  arg1->message = 'i';
  arg1->secs    =  4;

  ConcurrentTaskArg* arg2 = new ConcurrentTaskArg();
  arg2->message = 'o';
  arg2->secs    =  2;

  ThreadPool::Task task0 = {TestConcurrentTaskFn, arg0};
  ThreadPool::Task task1 = {TestConcurrentTaskFn, arg1};
  ThreadPool::Task task2 = {TestConcurrentTaskFn, arg2};

  tp->dispatch(task0);
  tp->dispatch(task1);
  tp->dispatch(task2);
  usleep(4250000);  // 4.25s

  int tries = 0;
  for ( ; tries < 5; ++tries) {
    size_t len;
    pthread_mutex_lock(&lock);
    len = str.size();
    pthread_mutex_unlock(&lock);

    if (len == 3) {
      break;
    }
    sleep(1);
  }

  // The threads should have finished
  // but they did not
  REQUIRE(tries < 5);

  delete tp;

  // Make sure characters were added in expected order.
  REQUIRE(str == "Aoi");

  // added to the threadpool in order of Aio
  // but task that ads i should have slept long enough
  // that the o task finishes first

  pthread_mutex_destroy(&lock);
}
