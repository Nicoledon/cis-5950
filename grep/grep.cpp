#include "./grep_request.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <vector>
#include "./RequestQueue.hpp"
// You can add more includes if you want

using namespace std;

// Given a grep_request:
// 1. Prints out the grep_request
// 2. Prints out each line in the specified file that contains all
//    words that are in the query.
//    e.g. if the query is {"hello.txt", {"hi", "bye"}}
//    then it prints all lines of hello.txt that contains
//    both the word "hi" and "bye"
//
// The lines that are printed should be printed
// in the same order they show up in the file.
//
// If there are no lines that contain all words
// in the query, then nothing should be printed out
// after printing the request.
void grep(const grep_request &request);

// Reads a query from the console
// and returns it. Automatically retries
// if no system error or EOF was encountered, but
// user query was an invalid format.
//
// returns nullopt when the EOF (ctrl + d)
// is read. Meaning that the overall
// program should now terminate..
// returns some value if the reading was successful
optional<grep_request> read_query();

// you've implemented similar functions before
// We ask that you do so again
// the resulting function should return all tokens
// in a string split on whitespace. There should be
// no empty tokens in the result vector.
vector<string> split(const string &input);
void *reader_func(void *arg) {
  RequestQueue *a = reinterpret_cast<RequestQueue *> (arg);
  auto opt = read_query();
  while (opt.has_value()) {
    a->add(opt.value()); 
    opt = read_query();
  }
  a->close();
  return NULL;
}

void *writer_func(void *arg) {
  RequestQueue *a = reinterpret_cast<RequestQueue *> (arg);
  while(true){
    auto item = a->wait_remove();
    if(item.has_value()){
      grep(item.value());
    } else{
      break;
    }
  }
  return NULL;
}
// this function is given to you
int main() {
  auto queue = new RequestQueue;
  pthread_t reader, writer;
  pthread_create(&reader, NULL, reader_func, queue);
  pthread_create(&writer, NULL, writer_func, queue);
  pthread_join(reader , NULL);
  pthread_join(writer , NULL);
  delete queue;
  return EXIT_SUCCESS;
}

// We've given you part of it to get you started
vector<string> split(const string &input) {
  vector<string> tokens;
  size_t pos = input.find_first_of(" \r\t\n");
  size_t start = 0;

  while (pos != string::npos) {
    string sub = input.substr(start, pos - start);
    start = pos + 1;
    tokens.push_back(sub);
    pos = input.find_first_of(" \r\t\n", start);
  }
  tokens.push_back(input.substr(start));
  return tokens;
}

void grep(const grep_request &request) {
  std::cout << request.to_string() << std::endl;
  std::ifstream file(request.fname);
  // You probably want the above lines
  // you may want to take some inspiration from read_query
  // - how you can treat a stream like a bool in a condition
  // - using the getline function
  // TODO
  string line;
  auto words = request.queries;
  int count = 0;
  set<string> sets;
  for (auto item : words) {
    sets.insert(item);
  }
  while (getline(file, line)) {
    std::stringstream ss(line);
    string word;
    auto temp = sets;
    while (ss >> word) {
      if (temp.contains(word)) {
        temp.erase(temp.find(word));
      }
    }
    if (temp.size() == 0) {
      cout << line << endl;
    }
  }
}

// this function is given to you
optional<grep_request> read_query() {
  grep_request request;
  string line;
  getline(cin, line);

  while (cin) {
    // tokenize the string
    vector<string> tokens = split(line);

    // parse tokens into a query

    if (tokens.size() > 1) {
      request.fname = tokens.at(0);
      tokens.erase(tokens.begin());
      request.queries = std::move(tokens);
      return request;
    }
    cerr << "Parsing Error: expected a file name followed by a one or more";
    cerr << " tokens to search for" << endl;

    // try reading line again
    getline(cin, line);
  }

  return nullopt;
}
