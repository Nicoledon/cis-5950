/*
 * Copyright ©2023 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Pennsylvania
 * CIT 5950 for use solely during Spring Semester 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <optional>

#include "./RequestQueue.hpp"
#include "./catch.hpp"

using std::cerr;
using std::endl;
using std::optional;
using std::ostream;

struct ThreadArg {
  RequestQueue* queue;
  int& num_read;
  int& num_write;
  pthread_mutex_t& rw_lock;
};

static void* read_grep_requests(void* queue);

static void* read_closed(void* queue);

// statics
/*constexpr grep_request kOne {1.0};
constexpr grep_request kZero {0.0};
constexpr grep_request kNegative {-1.25};
constexpr grep_request kPi {3.14};
constexpr grep_request kRs {152.0};
*/

const grep_request kOne {"one", {"k"}};
const grep_request kZero {"zero", {}};
const grep_request kNegative {"./negative", {"No", "Negative", "str"}};
const grep_request kPi {"./pi", {"Phi", "Life", "Cypher"}};
const grep_request kRs {"./RuneLite", {"In", "The", "End", "it", "doesn't", "even", "matter"}};

bool operator==(const grep_request& lhs, const grep_request& rhs) {
  if (lhs.fname != rhs.fname) {
    return false;
  }
  if (lhs.queries.size() != rhs.queries.size()) {
    return false;
  }
  for (std::size_t i = 0; i < lhs.queries.size(); i++) {
    if (lhs.queries.at(i) != rhs.queries.at(i)) {
      return false;
    }
  }
  return true;
}

ostream& operator<<(ostream& lhs, const grep_request& rhs) {
  return lhs << rhs.to_string();
}

TEST_CASE("add_remove", "[Test_RequestQueue]") {
  RequestQueue* q = new RequestQueue();
  optional<grep_request> opt{};

  // try removing before anything has happened
  opt = q->remove();
  REQUIRE_FALSE(opt.has_value());

  REQUIRE(q->add(kOne));
  opt = q->remove();
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == kOne);
  

  REQUIRE(0 == q->length());
  // try removing from an empty queue
  opt = q->remove();
  REQUIRE_FALSE(opt.has_value());

  REQUIRE(q->add(kZero));
  REQUIRE(q->add(kRs));
  REQUIRE(q->add(kNegative));
  REQUIRE(q->add(kPi));
  REQUIRE(4 == q->length());

  opt = q->remove();
  REQUIRE(opt.has_value());
  REQUIRE(kZero == opt.value());
  REQUIRE(3 == q->length());

  opt = q->remove();
  REQUIRE(opt.has_value());
  REQUIRE(kRs == opt.value());
  REQUIRE(2 == q->length());

  // Delete q to make sure destructor works, then award points
  // if it doesn't crash
  delete q;

  // re-construct and test destructing a queue with 1 elment
  q = new RequestQueue();
  REQUIRE(q->add(kPi));
  delete q;

  // re-construct and test destructing an empty queue
  q = new RequestQueue();
  delete q;
}

TEST_CASE("wait_remove", "[Test_RequestQueue]") {
  // lock used for both num_write num_read and cerr
  pthread_mutex_t rw_lock;
  int num_read = -1;
  int num_write = 0;

  RequestQueue* q = new RequestQueue();
  pthread_mutex_init(&rw_lock, nullptr);

  ThreadArg* arg = new ThreadArg({q, num_read, num_write, rw_lock});

  pthread_t reader;
  pthread_create(&reader, nullptr, read_grep_requests, arg);

  // first test that the reader thread sleeps/waits
  // when it tries to read and there is nothing on the queue

  // sleep for a bit to give a chance for reader to start 
  // and to try and wait_remove from q
  sleep(1);

  // ensure that the reader thread
  // has started and not read anything
  pthread_mutex_lock(&rw_lock);
  while (num_read != 0) {
    pthread_mutex_unlock(&rw_lock);
    sleep(1);
    pthread_mutex_lock(&rw_lock);
  }
  pthread_mutex_unlock(&rw_lock);

  // add and sleep so that reader
  // has a chance to read
  REQUIRE(q->add(kPi));
  sleep(1);

  pthread_mutex_lock(&rw_lock);
  num_write = 1;
  if (num_read != 1) {
    cerr << "wait_remove seemingly doesn't notice the addition of";
    cerr << "a grep_request to the queue. Possible deadlock or not implemented";
    cerr << endl;
  }
  REQUIRE(1 == num_read);
  pthread_mutex_unlock(&rw_lock);

  // next, test the case where there are already
  // values in the queue when the reader calls wait_remove.
  pthread_mutex_lock(&rw_lock);
  REQUIRE(q->add(kOne));
  REQUIRE(q->add(kNegative));
  REQUIRE(q->add(kZero));
  REQUIRE(q->add(kRs));
  num_write += 4;
  pthread_mutex_unlock(&rw_lock);

  // sleep for a bit to give a chance for reader to start 
  // and to try and wait_remove from q
  sleep(3);

  int secs_waited;
  for (secs_waited = 3; secs_waited < 30; secs_waited += 3) {
    pthread_mutex_lock(&rw_lock);
    if (num_read == 5) {
      pthread_mutex_unlock(&rw_lock);
      break;
    }
    pthread_mutex_unlock(&rw_lock);
    sleep(3);
  }

  // make sure that we didn't time out
  // waiting for the reader thread to
  // read 4 more times.
  REQUIRE(secs_waited < 30);
  
  bool* reader_result;
  pthread_join(reader, reinterpret_cast<void**>(&reader_result));

  // make sure the reader had no errors
  REQUIRE(*reader_result);

  delete reader_result;
  delete q;
  delete arg;
}


TEST_CASE("close", "[Test_RequestQueue]") {
  // lock used for both num_write num_read and cerr
  // lock used for both num_write num_read and cerr
  pthread_mutex_t rw_lock;
  int num_read = 0;
  int num_write = 0;

  RequestQueue* q = new RequestQueue[2];
  RequestQueue* second_q = q + 1; // since we created two queues.
  pthread_mutex_init(&rw_lock, nullptr);

  ThreadArg* arg = new ThreadArg({q, num_read, num_write, rw_lock});

  pthread_t reader;
  pthread_create(&reader, nullptr, read_closed, arg);

  // first test that the reader thread sleeps/waits
  // when it tries to read and there is nothing on the queue

  // sleep for a bit to give a chance for reader to start 
  // and to try and wait_remove from q
  sleep(1);

  // ensure that the reader thread
  // has not read anything
  pthread_mutex_lock(&rw_lock);
  REQUIRE(num_read == 0);
  pthread_mutex_unlock(&rw_lock);

  // add and sleep so that reader
  // has a chance to read
  REQUIRE(q->add(kPi));
  sleep(1);

  pthread_mutex_lock(&rw_lock);
  num_write = 1;
  if (num_read != 1) {
    cerr << "wait_remove seemingly doesn't notice the addition of";
    cerr << "a grep_request to the queue. Possible deadlock or not implemented";
    cerr << endl;
  }
  REQUIRE(1 == num_read);
  pthread_mutex_unlock(&rw_lock);

  // next, test the case where there are already
  // values in the queue when the reader calls wait_remove.
  pthread_mutex_lock(&rw_lock);
  REQUIRE(q->add(kOne));
  REQUIRE(q->add(kRs));
  REQUIRE(q->add(kPi));
  q->close();
  REQUIRE_FALSE(q->add(kNegative));
  REQUIRE_FALSE(q->add(kZero));
  num_write += 3;
  pthread_mutex_unlock(&rw_lock);

  // sleep for a bit to give a chance for reader to start 
  // and to try and wait_remove from q
  sleep(3);

  int secs_waited;
  for (secs_waited = 3; secs_waited < 30; secs_waited += 3) {
    pthread_mutex_lock(&rw_lock);
    if (num_read == 5) {
      pthread_mutex_unlock(&rw_lock);
      break;
    }
    pthread_mutex_unlock(&rw_lock);
    sleep(3);
  }

  // make sure that we didn't time out
  // waiting for the reader thread to
  // read 3 more times.
  REQUIRE(secs_waited < 30);
  
  // start testing the second q 
  pthread_mutex_lock(&rw_lock);
  REQUIRE(second_q->add(kRs));
  num_read = 0;
  num_write = 1;
  pthread_mutex_unlock(&rw_lock);

  sleep(5);

  // check that the second queue actually read what we sent, and
  // should be blocked on wait_remove now.
  // if it all looks good, close it and wait for the child.
  pthread_mutex_lock(&rw_lock);
  REQUIRE(num_read == 1);
  second_q->close();
  REQUIRE_FALSE(second_q->add(kNegative));
  num_write = 2;
  pthread_mutex_unlock(&rw_lock);

  // sleep for a bit to give a chance for reader to unblock
  // from wait_remove after q is closed.
  sleep(3);

  for (secs_waited = 3; secs_waited < 30; secs_waited += 3) {
    pthread_mutex_lock(&rw_lock);
    if (num_read == -1) {
      pthread_mutex_unlock(&rw_lock);
      break;
    }
    pthread_mutex_unlock(&rw_lock);
    sleep(3);
  }

  // make sure that we didn't time out
  // waiting for the reader thread to
  // wake up after queue is closed
  REQUIRE(secs_waited < 30);

  bool* reader_result;
  pthread_join(reader, reinterpret_cast<void**>(&reader_result));

  // make sure the reader had no errors
  REQUIRE(*reader_result);

  delete reader_result;
  delete[] q;
  delete arg;
}

void* read_grep_requests(void* arg) {
  ThreadArg* args = static_cast<ThreadArg*>(arg);
  RequestQueue* q = args->queue;
  int& num_read = args->num_read;
  int& num_write = args->num_write;
  pthread_mutex_t& rw_lock = args->rw_lock;

  optional<grep_request> value;

  // will be returned to make sure everything worked
  // form the perspective of the reader
  bool* result = new bool(true);

  pthread_mutex_lock(&rw_lock);
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kPi) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kPi << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  while (true) {
    pthread_mutex_lock(&rw_lock);
    if (num_write == 5) {
      pthread_mutex_unlock(&rw_lock);
      break;
    }
    pthread_mutex_unlock(&rw_lock);
    sleep(1);
  }


  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kOne) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kOne << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kNegative) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kNegative << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kZero) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kZero << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kRs) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kRs << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);
  
  return result;
}

void* read_closed(void* arg) {
  ThreadArg* args = static_cast<ThreadArg*>(arg);
  RequestQueue* q = args->queue;
  RequestQueue* second_q = q + 1; // since we created two queues.
  int& num_read = args->num_read;
  int& num_write = args->num_write;
  pthread_mutex_t& rw_lock = args->rw_lock;

  optional<grep_request> value;

  // will be returned to make sure everything worked
  // form the perspective of the reader
  bool* result = new bool(true);

  // should get it once fine from calling wait_remove
  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kPi) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kPi << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  // loop till num_write is 4, to indicate that parent has written 3 more things
  // and closed the queue.
  while (true) {
    pthread_mutex_lock(&rw_lock);
    if (num_write == 4) {
      pthread_mutex_unlock(&rw_lock);
      break;
    }
    pthread_mutex_unlock(&rw_lock);
    sleep(1);
  }

  // at this point, the queue should be closed but there are three values in it.
  // get one with wait_remove, another with remove and the last with wait_remove.
  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kOne) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kOne << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  value = q->remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kRs) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kRs << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kPi) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kPi << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  // should get nullopt when we call wait_remove in the future since it is now closed
  for (int i = 0; i < 3; i++) {
    value = q->wait_remove();
    pthread_mutex_lock(&rw_lock);
    if (value.has_value()) {
      *result = false;
      cerr << "Incorrect value from returned from call to wait_remove()";
      cerr << "from the reader thread" << endl;
      cerr << "\tExpected: nullopt" << endl;
      cerr << "\tActual: ";
      if (value.has_value()) {
        cerr << value.value() << endl;
      } else {
        cerr << "nullopt" << endl;
      }
    }
    pthread_mutex_unlock(&rw_lock);
  }

  // should get nullopt when we call remove in the future since it is now closed
  for (int i = 0; i < 3; i++) {
    value = q->remove();
    pthread_mutex_lock(&rw_lock);
    if (value.has_value()) {
      *result = false;
      cerr << "Incorrect value from returned from call to remove()";
      cerr << "from the reader thread" << endl;
      cerr << "\tExpected: nullopt" << endl;
      cerr << "\tActual: ";
      if (value.has_value()) {
        cerr << value.value() << endl;
      } else {
        cerr << "nullopt" << endl;
      }
    }
    pthread_mutex_unlock(&rw_lock);
  }

  // test a few more cases with the second queue
  
  // increment num_read to indicate we are ready for the next queues
  pthread_mutex_lock(&rw_lock);
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  // get a value from the queue fine
  value = second_q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kRs) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kRs << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;

  // ensure only 1 value has been written so far
  // for this test we count "close" as a write
  if (num_write != 1) {
    cerr << "TEST ERROR: value of num_write should be 1, parent should not have written to queue yet." << endl;
    cerr << "Contact Travis unless you modified the tests" << endl;
    pthread_mutex_unlock(&rw_lock);
    *result = false;
    return result;
  }

  // call remove on an empty (but not closed) queue and get nullopt
  value = second_q->remove();

  if (value.has_value()) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: nullopt" << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  pthread_mutex_unlock(&rw_lock);

  // call wait_remove and block since the queue is empty (but not closed)
  // we should get woken up by the call to close()
  // in the producer thread.
  value = second_q->wait_remove();

  pthread_mutex_lock(&rw_lock);
  if (value.has_value()) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: nullopt" << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }

  if (num_write != 2) {
    cerr << "child returned from wait_remove without parent calling close." << endl;
    *result = false;
  }

  // set this num_read to -1 to indicate that we have terminated.
  num_read = -1;
  pthread_mutex_unlock(&rw_lock);

  return result;
}
