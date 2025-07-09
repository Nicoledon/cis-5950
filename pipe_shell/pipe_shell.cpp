#include <array>
#include <cstdlib>
#include <cstring>
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
    arg[i] = strdup(words[i].c_str());
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
vector<char **> singlepipe(const vector<string> &words) {
  vector<char **> pip;
  vector<string> item;
  for (auto word : words) {
    if (word != "|") {
      item.push_back(word);
    } else {
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
void starter(array<int, 2> &fd, pid_t &processes, vector<char **> &container,
             const int &number) {
  if (pipe(fd.data()) < 0) {
    cerr << "pipe error" << strerror(errno) << endl;
    exit(EXIT_FAILURE);
  }
  processes = fork();
  if (processes == 0) {
    close(fd.at(0));
    int ret = dup2(fd.at(1), STDOUT_FILENO);
    if (ret < 0) {
      cerr << "dup2: " << strerror(errno) << endl;
      exit(EXIT_FAILURE);
    }
    close(fd.at(1));
    execvp(container[number][number], container[number]);
  }
  close(fd.at(1));
}
void ender(array<int, 2> &fd, pid_t &processes, vector<char **> &container,
           const int &number) {
           processes = fork();
           if(processes == 0){
            int ret = dup2(fd.at(0) , STDIN_FILENO);
            if(ret < 0){
              cerr <<"dup2: " <<strerror(errno) <<endl;
              exit(EXIT_FAILURE);
            }
            close(fd.at(0));
            execvp(container[number][0] , container[number]);
            exit(EXIT_FAILURE);
           }
          close(fd.at(0));
}
void working(array<int,2>& former , array <int,2> & curr , pid_t & processes , vector<char **> & container , const int &number){
     if(pipe(curr.data()) < 0){
        cerr << "pipe error" <<strerror(errno) << endl;
        exit(EXIT_FAILURE);
     }
     processes = fork();
     if(processes == 0){
      close(curr.at(0));
      int ret = dup2(former.at(0), STDIN_FILENO);
      if(ret < 0){
        cerr << "dup2: " <<strerror(errno) <<endl;
        exit(EXIT_FAILURE);
      }
      close(former.at(0));
      ret = dup2(curr.at(1), STDOUT_FILENO);
      if(ret < 0){
        cerr << "dup2: "<<strerror(errno)<<endl;
        exit(EXIT_FAILURE);
      }
      close(curr.at(1));
      execvp(container[number][0],container[number]);
      exit(EXIT_FAILURE);
     }
     close(former.at(0));
     close(curr.at(1));
}
void pipehandler(const vector<string> &arg, int number) {
  auto container = singlepipe(arg);
  vector<array<int, 2>> fds(number);
  vector<pid_t> processes(container.size());
  for(auto i = 0 ; i < container.size() ; i ++){
     if(i == 0){
      starter(fds[i], processes[i], container, i);
    }else if(i == container.size() -1 ){
      ender(fds[fds.size() - 1], processes[i],container ,i);
    } else{
      working(fds[i - 1] , fds[i] , processes[i] , container , i);
    }
  }
  int status;
  for (auto i = 0; i < processes.size(); i++) {
    waitpid(processes[i], &status, 0);
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
      break;
    } else {
      auto words = sqlit(line);
      auto count = ispipe(words);
      if (count != 0) {
        pipehandler(words, count);
      } else {
        auto arg = vec2arg(words);
        pid_t pid = fork();
        if (pid == 0) {
          execvp(arg[0], arg);
          cout << "No such file or directory"<<endl;
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
