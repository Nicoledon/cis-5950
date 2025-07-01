#include <stdexcept>
#include "vec.hpp"
// TODO: add the include for corresponding hpp
vec::vec() {
  // TODO: finish the 0-argument constructor
  data_  = new std::string[vec::default_capacity];
  length_ = 0;
  capacity_=10; 
}
vec::vec(size_t initial_capacity){
   data_ = new std::string[initial_capacity];
   length_ = 0;
   capacity_ = initial_capacity;
}
size_t vec::length(){
   return this->length_;
}
std::string* vec::data(){
   return this->data_;
}
size_t vec::capacity(){
   return this->capacity_;
}

// TODO: finish the rest

