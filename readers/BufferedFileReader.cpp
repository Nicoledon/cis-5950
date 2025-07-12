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
#include <stdio.h>
#include <string.h>
#include "BufferedFileReader.hpp"

// one provided function since this one has funky syntax
// it is just a wrapper around the good function though.

BufferedFileReader::BufferedFileReader(const std::string &fname) {
     this->fd_ = open(fname.c_str() , O_RDONLY);
     if(this->fd_ >= 0){
        this->good_ = true;
     }
     this->fill_buffer();
     this->count_ = 0; 
}
BufferedFileReader::~BufferedFileReader() {
    
    close(this->fd_);
     
}
BufferedFileReader::BufferedFileReader(BufferedFileReader && other) {
    this->curr_length_ = other.curr_length_;
    this->curr_index_ = other.curr_index_;
    this->fd_ = other.fd_;
    this->good_ = other.good_;
    this->count_ = other.count_;
    for(auto i = 0 ; i < other.curr_length_ ; i ++){
        this->buffer_[i] = other.buffer_[i];
    }
    other.close_file();
}
BufferedFileReader &BufferedFileReader::operator=(BufferedFileReader &&other) {
    if(other == *this) {
        return *this;
    }
    this->curr_length_ = other.curr_length_;
    this->curr_index_ = other.curr_index_;
    this->fd_ = other.fd_;
    this->good_ = other.good_;
    this->count_ = other.count_;
    for(auto i = other.curr_index_ ; i < other.curr_length_; i ++){
        this->buffer_[i] = other.buffer_[i];
    }
    other.close_file();
    return *this;
}

void BufferedFileReader::open_file(const std::string &fname) {
     if(this->good_ == true){
        close(this->fd_);
     }
     this->fd_ = open(fname.c_str() , O_RDONLY);
     this->fill_buffer();
     this->good_ = true;
     this->count_ = 0; 
}
void BufferedFileReader::close_file() {
     close(this->fd_);
     this->good_ = false;
     this->fd_ = -1;
     this->count_ = 0 ;
     this->curr_index_ = 0 ;
     this->curr_length_ = 0;
}
char BufferedFileReader::get_char() {
    if(this->good_ == false){
        return EOF;
    }
    if(this->curr_index_ >= this->curr_length_){
        fill_buffer();
        if(this->good_ == false){
           return EOF;
        }
    }
    this->count_ +=1;
    return this->buffer_.data()[this->curr_index_ ++];
}
std::optional<std::string> BufferedFileReader::get_token(const std::string &delims) {
     if(this->fd_ < 0 || !this->good_){
        return std::nullopt;
     }
    std::string str;
    char ch ;
    while(!delims.contains(ch = get_char())){
        if(ch == EOF){
            break;
        }
        str += ch;
    } 
    return str;
}
int BufferedFileReader::tell() const {
    if(this->fd_ >= 0){
        return this->count_;
    }
    return -1;
}
void BufferedFileReader::rewind() {
     if(this->fd_ >= 0){
        lseek(this->fd_ , 0 , SEEK_SET);
        fill_buffer();
        this->count_ = 0 ;
        this->good_ = true;
     }
}
bool BufferedFileReader::good() const {
    return this->good_;
}
BufferedFileReader::operator bool() const {
  return this->good();
}
void BufferedFileReader::fill_buffer(){
     auto num = read(this->fd_ , this->buffer_.data() , this->BUF_SIZE);
     if(num == 0){
        this->good_ = false;
        return ;
     }
     this->curr_index_ = 0 ;
     this->curr_length_ = num; 
}