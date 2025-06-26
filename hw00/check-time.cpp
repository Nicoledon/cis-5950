#include "simple_string.hpp"
#include "./ctypes.hpp"

#include <iostream>  // for cerr, cout, and endl

using namespace std;

int main(int argc, char* argv[]) {
  // checks that the correct number of command line args are provided
  if (argc != 2) {
    cerr << "Usage: ./check-time <string_to_check>" << endl;
    return EXIT_FAILURE;
  }

  simple_string input = from_cstring(argv[1]);

  // you probably don't need this line  ....
  cout << input << endl;

  // TODO: THE REST
  auto output = duplicate(input);
  auto substr1 = substr(input ,1 ,2);
  auto finder = find(input,'h');
  auto ch = at(input,1);
  cout <<"output: "<<output<<endl;
  cout <<"substr: "<<substr1<<endl;
  cout <<"finder: "<<finder<<endl;
  cout <<"ch: " <<ch <<endl; 
  return EXIT_SUCCESS;
}
