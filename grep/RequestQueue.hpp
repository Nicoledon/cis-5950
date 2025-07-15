#ifndef REQUEST_QUEUE_HPP_
#define REQUEST_QUEUE_HPP_

#include <pthread.h>
#include <optional>
#include "./grep_request.hpp"

///////////////////////////////////////////////////////////////////////////////
// A RequestQueue is a class that represents a queue of grep_request values
//
// The queue supports:
// - adding a request to the end of the queue
// - removing a request from the front of the queue
// - removing a request from the front of the queue and waiting
//   for a request to be added if there isn't one already.
// The queue is thread safe, with no potential for data races, or deadlocks
///////////////////////////////////////////////////////////////////////////////
class RequestQueue {
 public:
  // Constructor for a RequestQueue.
  // Initializes the queue to be empty
  // and ready to handle concurrent operations
  RequestQueue();

  // Destructor for RequestQueue.
  // Cleans up any remaining elements in the queue
  // and any synronization methods used for maintaining
  // the state of the queue.
  ~RequestQueue();

  // Adds a grep_request to the end of the queue
  // This operation is thread safe.
  //
  // Arguments:
  // - val: the grep_request value to add to the end of the queue
  //
  // Returns:
  // - true if the operation is successful
  // - false if the queue is closed
  bool add(grep_request val);

  // Closes the queue.
  //
  // Any calls to add() that happens after calling close should fail
  // and return false.
  //
  // calls to remove() or wait_remove() should return nullopt
  // if there are no elements in the queue left.
  //
  // Threads blocked on wait_remove() will be waken up to either
  // process any values left in the queue or return nullopt
  void close();

  // Removes a grep_request from the front of the queue
  // This operation is thread safe.
  //
  // Returns:
  // - the value if a value was successfully removed
  // - nullopt if there were no values in the queue
  std::optional<grep_request> remove();

  // Removes a grep_request from the front of the queue but if there is no
  // grep_request in the queue, calling thread will block or spin until there is
  // a grep_request available. If the the queue is closed and the queue is
  // empty, then it returns nullopt instead.
  //
  // This operation is thread safe.
  //
  // Arguments: None
  //
  // Returns:
  // - The value removed from the front of the queue
  // - nullopt if the queue is closed and empty
  std::optional<grep_request> wait_remove();

  // Returns the length of the queue currently
  // This operation is thread safe.
  //
  // Arguments: None
  //
  // Returns:
  // The value length of (i.e. number of elements in) the queue
  int length();

  // Feel free to ignore these
  RequestQueue(const RequestQueue& other) = delete;
  RequestQueue& operator=(const RequestQueue& other) = delete;
  RequestQueue(RequestQueue&& other) = delete;
  RequestQueue& operator=(RequestQueue&& other) = delete;

 private:
  // define a new internal type used to represent
  // nodes in the Queue
  // Queue can be implemented as a linked list
  struct QueueNode {
    QueueNode* next = nullptr;
    grep_request value;
  };

  // Fields
  // TODO: Add
  // You are not allowed to use any built in data structures to C++
  // e.g. no vector, list, deque, etc. You must implement the linked list yourself.
  int size ;
  struct QueueNode* head;
  struct QueueNode* end;
  pthread_mutex_t cond_lock;
  pthread_cond_t  cond;
  pthread_mutex_t rw_lock;
  bool is_close;
};
#endif  // REQUEST_QUEUE_HPP_
