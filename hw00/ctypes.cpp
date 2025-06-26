#include "./ctypes.hpp"

bool isdigit(char c) {
  return (c >= '0') && (c <= '9');
}

bool isspace(char c) {
  return (c == ' ') || (c == '\n') || (c == '\t') || (c == '\r') || (c == '\f') || (c == '\v');
}
