#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <unistd.h>

using namespace std;

constexpr int NUM_THREADS

void* thread_function(void* arg) {
  sleep(10);
  cout << "I'm the thread!" << endl;

  return new int (3);
}

int main() {
  pthread_t thd;

  pthread_create(&thd, nullptr, thread_function, nullptr);

  cout << "I'm the parent thread" << endl;

  int* ret_val = nullptr;
  pthread_join(thd, reinterpret_cast<void**>(&ret_val));


  cout << "I joined the child" << endl;
  cout << "child returned:" << *ret_val << endl;

  return EXIT_SUCCESS;
}
