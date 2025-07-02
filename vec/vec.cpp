#include "vec.hpp"
#include <stdexcept>
// TODO: add the include for corresponding hpp
vec::vec() {
  // TODO: finish the 0-argument constructor
  data_ = new std::string[vec::default_capacity];
  length_ = 0;
  capacity_ = 10;
}
vec::vec(const vec &other) {
  this->capacity_ = other.capacity_;
  this->length_ = other.length_;
  this->data_ = new std::string[other.capacity_];
  for (auto i = 0; i < other.length_; i++) {
    this->data_[i] = other.data_[i];
  }
}
vec::vec(size_t initial_capacity) {
  data_ = new std::string[initial_capacity];
  length_ = 0;
  capacity_ = initial_capacity;
}
vec::~vec() { delete[] data_; }
size_t vec::length() { return this->length_; }
std::string *vec::data() { return this->data_; }
size_t vec::capacity() { return this->capacity_; }
void vec::reserve(size_t new_cap) {
  if (new_cap > this->capacity_) {
    this->capacity_ = new_cap;
    auto newdata = new std::string[new_cap];
    for (auto i = 0; i < this->length_; i++) {
      newdata[i] = this->data_[i];
    }
    delete[] this->data_;
    this->data_ = newdata;
  }
}
void vec::push_back(const std::string &element) {
  if (this->length_ < this->capacity_) {
    this->data_[this->length_] = element;
  } else {
    this->reserve(2 * this->capacity_);
    this->data_[this->length_] = element;
  }
  this->length_ += 1;
}
std::optional<std::string> vec::pop_back() {
  if (this->length_ == 0) {
    return std::nullopt;
  } else {
    auto item = this->data_[this->length_ - 1];
    this->length_ -= 1;
    return item;
  }
}
std::string &vec::operator[](size_t index) {
  if (index >= this->length_ || index < 0) {
    throw std::out_of_range{"out of bound"};
  } else {
    return this->data_[index];
  }
}
std::string &vec::at(size_t index) {
  if (index >= this->length_ || index < 0) {
    throw std::out_of_range{"out of bound"};
  } else {
    return this->data_[index];
  }
}
size_t vec::insert(size_t index, const std::string &element) {
  if (index > this->length_) {
    throw std::out_of_range{"out of bound"};
    return -1;
  } else if (index == this->length_) {
    push_back(element);
    return index;
  } else {
    if (this->length_ == this->capacity_) {
      reserve(2 * this->capacity_);
    }
    int len = static_cast<int>(this->length_);
    for (int i = len - 1; i >= static_cast<int>(index); i--) {
      this->data_[i + 1] = this->data_[i];
    }
    this->data_[index] = element;
    this->length_ += 1;
    return index;
  }
}
size_t vec::erase(size_t index) {
  if (index >= this->length_) {
    throw std::out_of_range{"out of bound"};
    return -1;
  } else if (index == this->length_ - 1) {
    this->length_ -= 1;
    return this->length_ -1;
  } else {
    for (auto i = index; i < this->length_ - 1; i++) {
      this->data_[i] = this->data_[i + 1];
    }
    this->length_ -= 1;
    return index;
  }
}
// TODO: finish the rest
