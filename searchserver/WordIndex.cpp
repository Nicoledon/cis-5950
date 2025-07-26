#include "./WordIndex.hpp"

#include <algorithm>

namespace searchserver {

WordIndex::WordIndex() { }

size_t WordIndex::num_words() {
  // TODO
  return 0;
}

void WordIndex::record(const string& word, const string& doc_name) {
  // TODO
}

vector<Result> WordIndex::lookup_word(const string& word) {
  vector<Result> result;
  // TODO
  return result;
}

vector<Result> WordIndex::lookup_query(const vector<string>& query) {
  vector<Result> results;
  // TODO
  return results;
}

}  // namespace searchserver
