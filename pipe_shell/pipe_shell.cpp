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
int ispipe(const vector<string> &words) {
  int count = 0;
  for (auto word : words) {
    if (word == "|") {
      count += 1;
    }
  }
  return count;
}
void pipehandler(char **arg) {}
int main(int argc, char **argv) {
  string line;
  while (true) {
    cout << "$ ";
    if (!getline(cin, line)) {
      if (!cin.eof()) {
        cerr << "Warning: fatal error while reading input from user" << endl;
      }
      cout << endl;
      break;
    } else if (line == "exit") {
      cout << endl;
      break;
    } else {
      auto words = sqlit(line);
      auto count = ispipe(words);
      auto arg = vec2arg(words);
      if (count != 0) {
         cout << count << endl;
      } else {
        pid_t pid = fork();
        if (pid == 0) {
          execvp(arg[0], arg);
          exit(EXIT_FAILURE);
        } else {
          int status;
          waitpid(pid, &status, 0);
        }
      }
        delete[] arg;
    }
  }
  return 0;
}

// Nothing here!
// Highly recommend you look at stdin_echo.cpp for getting started.
// The lecure example code (and the specification for this assignment) are also
// good to read.
