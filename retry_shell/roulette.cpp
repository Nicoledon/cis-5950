#include <cstdlib>  // for abort , EXIT_SUCCESS, EXIT_FAILURE and exit;
#include <random>   // for random_device, mt19937_64, uniform_int_distribution
#include <iostream> // for cout, cerr, and endl

using namespace std;

// this program essentially rolls a 6-sided die and 
// only exits successfully if it rolled a 6.
int main() {
  random_device dev;
  mt19937_64 rng(dev());
  uniform_int_distribution dist(1, 6);

  int roll = dist(rng);

  if (roll == 6) {
    cout << "SUCCESS: ROLLED A 6" << endl;
    return EXIT_SUCCESS;
  }

  cout << "FAILED: ROLLED A " << roll << endl;
  abort();
}
