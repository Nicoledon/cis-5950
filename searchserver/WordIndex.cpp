#include "./WordIndex.hpp"

#include <algorithm>

namespace searchserver {

WordIndex::WordIndex() {}

size_t WordIndex::num_words() {
  // TODO
  return words.size();
}

void WordIndex::record(const string &word, const string &doc_name) {
  // TODO
  words[word][doc_name] += 1;
}

vector<Result> WordIndex::lookup_word(const string &word) {
  vector<Result> result;
  // TODO
  auto items = words[word];
  for (auto const &item : items) {
    auto obj = Result(item.first, item.second);
    result.push_back(obj);
  }
  return result;
}

vector<Result> WordIndex::lookup_query(const vector<string> &query) {
  vector<Result> results;
  // TODO
  for (auto const &word : query) {
    auto items = words[word];
    for (auto const &item : items) {
      auto obj = Result(item.first, item.second);
      results.push_back(obj);
    }
  }
  return results;
}

} // namespace searchserver
