/*
 * Copyright ©2025 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Pennsylvania
 * CIT 5950 for use solely during Spring Semester 2025 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#ifndef BUFFER_CHECKER_HPP_
#define BUFFER_CHECKER_HPP_

#include <string>

#include "BufferedFileReader.hpp"

class BufferChecker {
 public:
  BufferChecker(const BufferedFileReader& bfr) : bf_(bfr) {}

  // Returns true if there is a detectable error
  // False if an error was not detected
  bool check_char_errors(char to_check, off_t file_offset) {
    size_t index = file_offset % BufferedFileReader::BUF_SIZE;
    if (index == BufferedFileReader::BUF_SIZE - 1) {
      // give some flexibility on how the last character is handled
      return false;
    }

    return bf_.buffer_.at(index) != to_check;
  }

  // Returns true if there is a detectable error
  // False if an error was not detected
  bool check_token_errors(const std::string& token, off_t file_offset) {
    size_t end_index =
        (file_offset + token.length()) % BufferedFileReader::BUF_SIZE;
    size_t start_index = file_offset % BufferedFileReader::BUF_SIZE;

    // Check if we can even check if the token is in the buffer.
    // Can't be checked if token is clipped by buffer length, or token length is
    // greater than buffer length, and other edge cases
    if (start_index > end_index || end_index - start_index != token.length()) {
      return false;
    }

    for (off_t i = 0; start_index + i < end_index; i++) {
      if (token.at(i) != bf_.buffer_.at(i + start_index)) {
        return true;
      }
    }
    return false;
  }

  int fd() const {
    return bf_.fd_;
  }

  const std::array<char, BufferedFileReader::BUF_SIZE>& buffer() const {
    return bf_.buffer_;
  }

  size_t curr_length() const {
    return bf_.curr_length_;
  }

  size_t curr_index() const {
    return bf_.curr_index_;
  }

  bool good() const {
    return bf_.good_;
  }

 private:
  const BufferedFileReader& bf_;
};

#endif  // BUFFER_CHECKER_HPP_
