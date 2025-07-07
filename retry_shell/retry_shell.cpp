#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
using namespace std;
vector<string> sqlit(const string &line) {
  vector<string> words;
  istringstream iss(line);
  string word;
  while (getline(iss, word, ' ')) {
    words.push_back(word);
  }
  return words;
}
char **vec2arg(const vector<string> &words) {
  char **arg = new char *[words.size() + 1];
  for (auto i = 0; i < words.size(); i++) {
    arg[i] = const_cast<char *>(words[i].c_str());
  }
  arg[words.size()] = nullptr;
  return arg;
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
    } else if (line == "exit") {
      break;
    } else {
      auto words = sqlit(line);
      auto arg = vec2arg(words);
      auto num = 1;
      if (argc == 2) {
        num += stoi(argv[1]);
      }
      for (auto i = 0; i < num; i++) {
        pid_t pid = fork();
        if (pid == 0) {
          if (execvp(arg[0], arg) == -1) {
            cout << "retrying" << endl;
          }
          exit(EXIT_FAILURE);
        } else {
          int status;
          waitpid(pid, &status, 0);
          if(status == EXIT_SUCCESS){
              break;
          }else{
              if(i == num -1) {
                  cout << "Fail to run program after retrying"<<endl;
              } else{
              cout <<"retrying" <<endl;
              }
          }
        }
      }
      delete []arg;
    }
  }
  return 0;
}

// Nothing here!
// Highly recommend you look at stdin_echo.cpp for getting started.
// The lecure example code (and the specification for this assignment) are also
// good to read.
