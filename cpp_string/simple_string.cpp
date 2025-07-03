#include "./simple_string.hpp"
#include <stdexcept>
using namespace std;

simple_string::simple_string() {
  // TODO: finish the zero arg constructor
  this->data_ = new char[1];
  this->data_[0] = '\0';
  this->len_ = 0;
}
size_t length(const char *cstring) {
  size_t count = 0;
  while (cstring[count] != '\0') {
    count += 1;
  }
  return count;
}
char *copy(const char *cstring, size_t length) {
  char *data = new char[length + 1];
  for (auto i = 0; i < length; i++) {
    data[i] = cstring[i];
  }
  data[length] = '\0';
  return data;
}
simple_string::simple_string(const char *cstring) {
  this->len_ = length(cstring);
  auto data = copy(cstring, this->len_);
  this->data_ = data;
}
simple_string::simple_string(const simple_string &other) {
  this->len_ = length(other.data_);
  this->data_ = new char[this->len_ + 1];
  for (auto i = 0; i < this->len_; i++) {
    this->data_[i] = other.data_[i];
  }
  this->data_[this->len_] = '\0';
}
simple_string::~simple_string() { delete[] this->data_; }
const char *simple_string::data() const { return this->data_; }
size_t simple_string::len() const { return this->len_; }
simple_string simple_string::substr(size_t start_index, size_t length) const {
  if (start_index >= this->len_) {
    throw out_of_range{"out of range"};
  } else if (length == simple_string::npos ||
             length > this->len_ - start_index) {
    return simple_string(this->data_ + start_index);
  } else {
    auto item = simple_string();
    item.len_ = length;
    item.data_ = new char[length + 1];
    for (auto i = start_index; i < start_index + length; i++) {
      item.data_[i - start_index] = this->data_[i];
    }
    item.data_[length] = '\0';
    return item;
  }
}
bool simple_string::operator==(const simple_string &other) const {
  if (other.len_ != this->len_) {
    return false;
  }
  for (auto i = 0; i < other.len_; i++) {
    if (other.data_[i] != this->data_[i]) {
      return false;
    }
  }
  return true;
}
char &simple_string::at(size_t index) {
  if (index >= this->len_) {
    throw out_of_range{"out of range"};
  } else {
    return this->data_[index];
  }
}
char &simple_string::operator[](size_t index) {
  if (index >= this->len_) {
    throw out_of_range{"out of range"};
  } else {
    return this->data_[index];
  }
}
size_t simple_string::find(char target) const {
  for (auto i = 0; i < this->len_; i++) {
    if (this->data_[i] == target) {
      return i;
    }
  }
  return simple_string::npos;
}
vector<char> chs(const simple_string &chars) {
  vector<char> vec;
  auto data = chars.data();
  for (auto i = 0; i < chars.len(); i++) {
    vec.push_back(data[i]);
  }
  return vec;
}
bool inchar(const vector<char> &vec, char ch) {
  for (auto item : vec) {
    if (item == ch) {
      return true;
    }
  }
  return false;
}
size_t simple_string::find_first_of(const simple_string &chars,
                                    size_t pos) const {
  auto vec = chs(chars);
  auto data = this->data_;
  if(pos >= this->len_) {
    throw out_of_range{"out of range"};
  }
  for (auto i = pos; i < this->len_; i++) {
    if (inchar(vec, data[i])) {
      return i;
    }
  }
  return simple_string::npos;
}
// TODO: the other functions
