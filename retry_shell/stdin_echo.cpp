/*
 * Copyright ©2025 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Pennsylvania
 * CIT 5950 for use solely during Spring Semester 2025 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::getline;

// This program reads from stdin
// and continually prints out what
// is read in until the EOF is read
int main(int argc, char** argv) {
  cout << "booting up" << endl;

  string line;

  while(true) {

    if(!getline(cin, line)) {
      if(!cin.eof()){
        cerr << "Warning: fatal error while reading input from user" << endl;
      }
      cout << "REACHED END OF FILE" << endl;
      break;
    }
    
    cout << line << endl;
  }
}
