#ifndef GREP_REQUEST_HPP_
#define GREP_REQUEST_HPP_

#include <sstream>
#include <string>
#include <vector>

struct grep_request {
  std::string fname;
  std::vector<std::string> queries;

  std::string to_string() const {
    std::stringstream ss;
    ss << this->fname << ": [";
    if (!this->queries.empty()) {
      ss << this->queries.at(0);
      for (std::size_t i = 1; i < this->queries.size(); ++i) {
        ss << ", " << queries.at(i);
      }
    }
    ss << "]";

    return ss.str();
  }
};

#endif  // GREP_REQUEST_HPP_
