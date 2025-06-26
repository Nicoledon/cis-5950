#include "simple_string.hpp"
simple_string from_cstring(const char* cstring){
  simple_string item;
  auto len = strlen(cstring);
  item.data = new char[len];
  strncpy(item.data, cstring, len + 1);
  item.len = len + 1;
  return item;
}
simple_string duplicate(const simple_string to_copy) {
  simple_string item;
  item.len = to_copy.len;
  item.data = new char[item.len];
  strncpy(item.data, to_copy.data, to_copy.len);
  return item;
}
simple_string substr(const simple_string self, size_t start_index,
                     size_t length) {
  simple_string item;
  int len = 0;
  if (start_index + length >= self.len || length == simple_string::npos) {
    len = self.len - start_index;
    item.data = new char[len];
    strcpy(item.data, self.data + len);
  } else {
    len = length;
    item.data = new char[len];
    strncpy(item.data, self.data + len, length);
  }
  return item;
}
size_t find(const simple_string self , char target) { 
     for (size_t i = 0 ; i < self.len ; i ++) {
         if(self.data[i] == target) {
             return i ;
         }
     }
       return simple_string::npos;
}
bool operator==(const simple_string lhs , const simple_string rhs) {
     if(lhs.len != rhs.len) {
         return false;
     } else{
         for (auto i = 0 ; i< lhs.len ; i ++) {
              if(lhs.data[i] != rhs.data[i]) {
                  return false;
              }
         }
     }
     return true;
}
char at(const simple_string self , size_t index) {
     return self.data[index];
}
void destruct(simple_string to_destroy) {
     delete []to_destroy.data;
}
std::ostream& operator<<(std::ostream &os , const simple_string to_printf) {
     os << to_printf.data << std::endl;
     return os;
}
