#include "./RequestQueue.hpp"
#include <pthread.h>
#include <optional>

using std::optional;
using std::nullopt;


RequestQueue::RequestQueue() {
}

RequestQueue::~RequestQueue() {
}

bool RequestQueue::add(grep_request val) {
  return true;
}

void RequestQueue::close() {
}

optional<grep_request> RequestQueue::remove() {
  return nullopt;
}

optional<grep_request> RequestQueue::wait_remove() {
  return nullopt;
}

int RequestQueue::length() {
  return -1;
}
