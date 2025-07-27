#ifndef WORD_INDEX_H_
#define WORD_INDEX_H_

#include <unordered_map>
#include <vector>
#include <string>
#include <queue>
#include<algorithm>
#include <map>
#include "./Result.hpp"

using std::string;
using std::vector;

namespace searchserver {

// A WordIndex is used to keep track of which documents contain certain words
// and how many occurances there are of that word in the document
class WordIndex {
 public:

  // Constructs an empty WordIndex that stores
  // no words or documents to start
  WordIndex();

  // default destructor
  ~WordIndex() = default;

  // Returns the number of unique words recorded in the index
  size_t num_words();
  
  // Record an occurance of a document having the specified word show up in it
  // 
  // Arguments:
  //  - word: the word found in the specified document
  //  - doc_name: the name of the document the word occurance showed up in
  //
  // Returns: None
  void record(const string& word, const string& doc_name);

  // Lookup a word in the index, getting a list of all documents that contain the word
  // and a rank which is the number of occurances of that word in the document
  //
  // Arguments
  //  - word: a word we are looking up results for
  //
  // Returns:
  //  - A list of results. Each result contains a document name and the number
  //    of recorded occurances of the specified word in that document.
  vector<Result> lookup_word(const string& word);

  // Lookup a query (multiple words) in the index, getting a list of all documents that contain each word
  // in the query and a rank which is the number of occurances of each word in the document.
  //
  // Arguments:
  //  - word: a word we are looking up results for
  //
  // Returns:
  //  - A list of results. Each result contains a document name and the sum of the
  //    number of recorded occurances of the each query word in that document.
  vector<Result> lookup_query(const vector<string>& query);

  // default move, delete copy
  WordIndex(const WordIndex& other) = default;
  WordIndex& operator=(const WordIndex& other) = default;
  WordIndex(WordIndex&& other) = default;
  WordIndex& operator=(WordIndex&& other) = default;

 private:
  // TODO: add
  std::map<std::string , std::map <std::string , int>> words; 
};

}

#endif  // WORD_INDEX_
