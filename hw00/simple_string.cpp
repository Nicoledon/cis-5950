#include "simple_string.hpp"

simple_string from_cstring(const char *cstring) {
  simple_string item;
  auto len = strlen(cstring);
  item.data = new char[len];
  strcpy(item.data, cstring, len + 1);
  item.len = len + 1;
  return item;
}
simple_string duplicate(const simple_string to_copy) {
  simple_string item;
  item.len = to_copy.len;
  item.data = new char[item.len];
  strcpy(item.data, to_copy.data, to_copy);
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
    strcpy(item.data, self.data + len, length);
  }
  return item;
}
