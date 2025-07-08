#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <array>
#include<cstring>
#include<cstdlib>
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
vector<char **> singlepipe(const vector<string> & words){
      vector<char **> pip ;
      vector<string> item;
      for(auto word : words){
          if(word != "|"){
             item.push_back(word); 
          }else{
              auto elem = item;
              auto ch = vec2arg(elem);
              item.clear(); 
              pip.push_back(ch);
          }
      }
      auto ch = vec2arg(item);
      pip.push_back(ch);
      return pip;
}
void pipehandler(const vector<string> 
        &arg, int number) {
     if(number == 1){
       auto container =  singlepipe(arg);
       array<int,2> pipe_fds{-1,-1 };
       if(pipe(pipe_fds.data()) < 0){
         cerr << "pipe error: " << strerror(errno) << endl;
         exit(EXIT_FAILURE);
       }
       pid_t pid0 = fork();
       if(pid0 == 0){
           close(pipe_fds.at(0));
           int ret = dup2(pipe_fds.at(1), STDOUT_FILENO);
           if(ret < 0){
               cerr << "dup2: " << strerror(errno) <<endl;
               exit(EXIT_FAILURE);
           }
           close(pipe_fds.at(1));
           execvp(container[0][0],container[0]);
           exit(EXIT_FAILURE);
       }
       close(pipe_fds.at(1));
       pid_t pid1 = fork();
       if(pid1 == 0){
           int ret  = dup2(pipe_fds.at(0) , STDIN_FILENO);
           if(ret < 0){
               cerr << "dup2: " << strerror(errno) <<endl;
               exit(EXIT_FAILURE);
           }
          close(pipe_fds.at(0));
          execvp(container[1][0],container[1]);
          exit(EXIT_FAILURE);
       }
       close(pipe_fds.at(0));
       int status;
       waitpid(pid0 , &status, 0);
       waitpid(pid1, &status , 0);
     }
}
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
      if (count != 0) {
         pipehandler(words , count);
      } else {
        auto arg = vec2arg(words);
        pid_t pid = fork();
        if (pid == 0) {
          execvp(arg[0], arg);
          exit(EXIT_FAILURE);
        } else {
          int status;
          waitpid(pid, &status, 0);
        }
        delete[] arg;
      }
    }
  }
  return 0;
}

// Nothing here!
// Highly recommend you look at stdin_echo.cpp for getting started.
// The lecure example code (and the specification for this assignment) are also
// good to read.
