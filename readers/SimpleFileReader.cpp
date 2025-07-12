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

#include "./SimpleFileReader.hpp"
SimpleFileReader::SimpleFileReader(const std::string &fname) {
    this->fd_ = open(fname.c_str(), O_RDONLY);
    if (this->fd_ > 0) {
        this->good_ =true;
    }
}
SimpleFileReader::~SimpleFileReader() {
    close(this->fd_);
    this->fd_ = -1;
    this->good_ = false;
}
void SimpleFileReader::open_file(const std::string &fname) {
    if (this->good_ == true) {
        close(this->fd_);
    }
    this->fd_ = open(fname.c_str() , O_RDONLY);
    this->good_ = true;
}

void SimpleFileReader::close_file() {
    close(this->fd_);
    this->fd_ = -1;
    this->good_ = false;
}
char SimpleFileReader::get_char() {
     char ch;
     if (read(this->fd_ , &ch , 1) == 0) {
         this->good_ = false;
         return EOF;
     }else {
         return ch ;
     }
}
std::optional<std::string> SimpleFileReader::get_chars(size_t n) {
    std:: string str;
    if (this->good_ == false) {
        return std::nullopt;
    }
    char ch;
    for (auto i = 0 ; i < n ; i++) {
        auto num = read(this->fd_ , &ch , 1);
        if (num == 0) {
            this->good_ = false;
            break;
        }
        str += ch;
    }
    if (str.length() == 0) {
        return std::nullopt;
    }
    return str;
}
bool SimpleFileReader::good() const {
     return this->good_;
}
void SimpleFileReader::rewind() {
     lseek(this->fd_ , 0 ,SEEK_SET);
     this->good_ = true;
}
int SimpleFileReader::tell() const {
    if (this->fd_ >= 0) {
        return lseek(this->fd_ , 0 , SEEK_CUR);
    }
    return -1;
}


