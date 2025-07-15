#include "./RequestQueue.hpp"
#include <optional>
#include <pthread.h>

using std::nullopt;
using std::optional;

RequestQueue::RequestQueue() {
  this->size = 0;
  head = new struct QueueNode;
  head->next = NULL;
  is_close = false;
  pthread_mutex_init(&this->cond_lock, NULL);
  pthread_cond_init(&this->cond, NULL);
  pthread_mutex_init(&this->rw_lock, NULL);
}

RequestQueue::~RequestQueue() {
  pthread_mutex_destroy(&rw_lock);
  pthread_mutex_destroy(&cond_lock);
  pthread_cond_destroy(&cond);
  while (head != NULL) {
    auto temp = head;
    head = head->next;
    delete temp;
  }
}

bool RequestQueue::add(grep_request val) {
  pthread_mutex_lock(&this->rw_lock);
  if (this->is_close == true) {
    pthread_mutex_unlock(&this->rw_lock);
    return false;
  }
  auto node = new struct QueueNode;
  node->value = val;
  auto temp = head->next;
  this->head->next = node;
  node->next = temp;
  this->size += 1;
  pthread_cond_broadcast(&this->cond);
  pthread_mutex_unlock(&this->rw_lock);
  return true;
}

void RequestQueue::close() {
  pthread_mutex_lock(&this->rw_lock);
  this->is_close = true;
  pthread_mutex_unlock(&this->rw_lock);
}

optional<grep_request> RequestQueue::remove() {
  pthread_mutex_lock(&this->rw_lock);
  if (this->size == 0) {
    pthread_mutex_unlock(&this->rw_lock);
    return nullopt;
  } else {
    auto temp = head;
    while (temp->next->next != NULL) {
      temp = temp->next;
    }
    auto t = temp->next;
    temp->next = NULL;
    auto val = t->value;
    delete t;
    this->size -= 1;
    pthread_mutex_unlock(&this->rw_lock);
    return val;
  }
}

optional<grep_request> RequestQueue::wait_remove() {
  pthread_mutex_lock(&this->rw_lock);
  if (is_close == true && this->size == 0) {
    pthread_mutex_unlock(&this->rw_lock);
    return nullopt;
  }
  while (this->size == 0) {
    pthread_cond_wait(&cond, &this->rw_lock);
  }
  auto temp = head;
  while (temp->next->next != NULL) {
    temp = temp->next;
  }
  auto t = temp->next;
  temp->next = NULL;
  auto val = t->value;
  delete t;
  this->size -= 1;
  pthread_mutex_unlock(&this->rw_lock);
  return val;
}

int RequestQueue::length() {
  pthread_mutex_lock(&this->rw_lock);
  int val = this->size;
  pthread_mutex_unlock(&this->rw_lock);
  return val;
}
