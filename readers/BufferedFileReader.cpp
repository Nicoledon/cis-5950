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

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "BufferedFileReader.hpp"

// one provided function since this one has funky syntax
// it is just a wrapper around the good function though.

BufferedFileReader::BufferedFileReader(const std::string &fname) {

}
BufferedFileReader::~BufferedFileReader() {

}
BufferedFileReader::BufferedFileReader(BufferedFileReader && other) {

}
BufferedFileReader &BufferedFileReader::operator=(BufferedFileReader &&other) {
    return *this;
}

void BufferedFileReader::open_file(const std::string &fname) {

}
void BufferedFileReader::close_file() {

}
char BufferedFileReader::get_char() {
    return EOF;
}
std::optional<std::string> BufferedFileReader::get_token(const std::string &delims) {
     return std::nullopt;
}
int BufferedFileReader::tell() const {
    return -1;
}
void BufferedFileReader::rewind() {

}
bool BufferedFileReader::good() const {
    return false;
}
BufferedFileReader::operator bool() const {
  return this->good();
}
