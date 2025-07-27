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
  sort(result.begin(), result.end());
  return result;
}

vector<Result> WordIndex::lookup_query(const vector<string> &query) {
  vector<Result> results;
  // TODO
  std::unordered_map<string,int>maps;
  std::unordered_map<string,int>sets;
  for(auto const & check : query) {
      auto vec = lookup_word(check);
      for(auto const & result : vec){
         maps[result.doc_name] += result.rank;
         sets[result.doc_name] += 1;
      }
  }
  for(auto item : maps){
     if(sets[item.first] == query.size()){
        auto obj = Result(item.first , item.second);
        results.push_back(obj);
     }
  }
  sort(results.begin() , results.end());
  return results;
}

} // namespace searchserver
