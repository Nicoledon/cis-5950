#include <unistd.h>  // for sleep
#include <cstdlib>   // for EXIT_SUCCESS
#include <iostream>  // for cout, cerr, and endl

using namespace std;

int main(int argc, char* argv[]) {
  cout << "Sleeping for 10 seconds..." << endl;
  sleep(10);
  cout << "Woke up from 10 second sleep!" << endl;
  return EXIT_SUCCESS;
}
