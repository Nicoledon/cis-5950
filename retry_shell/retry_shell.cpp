#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include<sstream>
using namespace std;
vector<string>sqlit(const string & line){
    vector<string> words;
    istringstream iss(line);
    string word;
    while(getline(iss , word ,' ')){
        words.push_back(word);
    }
    return words; 
}
int main(int argc, char **argv) {
  string line;
  while (true) {
    cout << "$ ";
    if (!getline(cin, line)) {
      if (!cin.eof()) {
        cerr << "Warning: fatal error while reading input from user" << endl;
      }
      break;
    } else {
      auto words = sqlit(line);  
      if(words.empty()){
          continue;
      }else if(words[0] == "exit"){
          break;
      }else if(words[0] == "echo"){
           for(auto i = 1 ;i < words.size() ;i ++){
               cout << words[i] << " ";
           }
           cout <<"\n";
      }else if(words[0] == "sleep"){
          auto num = stoi(words[1]);
          sleep(num);
      }
    }
  }
  return 0;
}

// Nothing here!
// Highly recommend you look at stdin_echo.cpp for getting started.
// The lecure example code (and the specification for this assignment) are also
// good to read.
