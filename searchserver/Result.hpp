#ifndef RESULT_HPP_
#define RESULT_HPP_

#include <string>

using std::string;

namespace searchserver {

// This class represents a Result from looking up in the index
// It contains a document name and a rank which is typically the
// number of times certain word(s) show up in the document
struct Result {
 public:
  string doc_name;
  int rank;

  Result() : doc_name(), rank(0) { }

  Result(string doc_name, int rank) : doc_name(doc_name), rank(rank) { }

  // Sort so that bibgger rank comes first
  bool operator<(const Result& other) const {
    return other.rank < this->rank;
  }

  // the synthtesized cctor and op= are fine here
};

}

#endif  // RESULT_HPP_
