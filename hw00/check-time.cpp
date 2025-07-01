#include "simple_string.hpp"
#include "./ctypes.hpp"

#include <iostream>  // for cerr, cout, and endl
#define MAX 10000000
using namespace std;
void invalid(simple_string & str);
void valid(simple_string &str);
bool TonumberFormer(simple_string & str);
bool TonumberLatter(simple_string & str);
void invalid(simple_string & str) {
     cout << "INVALID" << endl;
     destruct(str);
     exit(0);
}
void valid(simple_string & str) {
     cout << "VALID" << endl;
     destruct(str);
     exit(0);
}
bool TonumberFormer(simple_string & str) {
     for (auto i = 0 ; i < str.len ; i ++) {
         if(!isdigit(str.data[i])) return false;
     }
     auto number = atoi(str.data);
     if(number < 0 || number > 23) return false ;
     return true;
}
bool TonumberLatter(simple_string & str) {
     if(str.len != 2) return false;
     for(auto i = 0 ; i < str.len ; i ++) {
         if(!isdigit(str.data[i]))return false;
     }
     auto number = atoi(str.data);
     if(number < 0 || number > 59) return false;
     return true;
}
int main(int argc, char* argv[]) {
  // checks that the correct number of command line args are provided
  if (argc != 2) {
    cerr << "Usage: ./check-time <string_to_check>" << endl;
    return EXIT_FAILURE;
  }

  simple_string input = from_cstring(argv[1]);

  // TODO: THE REST
  auto colon = find(input , ':');
  if (colon == simple_string::npos) invalid(input);
  auto forme = substr(input , 0 , colon);  
  if(!TonumberFormer(forme)) {
      destruct(forme);
      invalid(input);
  }
  auto latter = substr(input , colon + 1 ,MAX);   
  if(!TonumberLatter(latter)) {
      destruct(latter);
      invalid(input);
  }
  destruct(forme);
  destruct(latter);
  valid(input);
  return EXIT_SUCCESS;
}
