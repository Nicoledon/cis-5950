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

#ifndef HTTPCONNECTION_HPP_
#define HTTPCONNECTION_HPP_

#include <cstdint>
#include <unistd.h>
#include <map>
#include <string>
#include <sys/socket.h>
#include <optional>
#include <cstring>

namespace searchserver {

// The HttpSocket class represents a connection to a single client
class HttpSocket {
 public:
 
  // Constructs a new HttpSocket to handle the
  // connection to a client on the represented file descriptor
  HttpSocket(int fd, socklen_t addr_len, sockaddr* addr) : fd_(fd), buffer_(), addr_len_(addr_len), addr_() {
    memcpy(&addr_, addr, addr_len_);

    // We now have a connection to a client.  Set the socket options to
    // enable re-use of the port.
    int optval = 1;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  }
  
  // closes the connection to the client if it is still open
  ~HttpSocket() {
    if (fd_ != -1) {
      close(fd_);
    }
    fd_ = -1;
  }

  // Read and the next HTTP request from the file descriptor fd_,
  // and returns it as a string. Returns nullopt if a full HTTP
  // request header cannot be read, in which case the caller should
  // close the connection.
  std::optional<std::string> next_request();

  // Write the response to the file descriptor fd_.  Returns true
  // if the response was successfully written, false if the
  // connection experiences an error and should be closed.
  bool write_response(const std::string& response) const;

  int fd() const { return fd_; };

  std::string client_addr() const;

  uint16_t client_port() const;

  std::string server_addr() const;

  uint16_t server_port() const;

  HttpSocket(HttpSocket&& other) : fd_(other.fd_), buffer_(std::move(other.buffer_)), addr_len_(other.addr_len_), addr_(other.addr_) {
    other.fd_ = -1;
  }

  HttpSocket& operator=(HttpSocket&& other) {
    if (this != &other) {
      fd_ = other.fd_;
      buffer_ = std::move(other.buffer_);
      addr_len_ = other.addr_len_;
      addr_ = other.addr_;
      other.fd_ = -1;
    }
    return *this;
  }

  HttpSocket(const HttpSocket& other) = delete;
  HttpSocket& operator=(const HttpSocket& other) = delete;


 private:
  // The file descriptor associated with the client.
  int fd_;

  // A buffer storing data read from the client.
  // Used for the case where we read more data than we need to process a request
  // store the excess data read into the buffer so that next time we read, we can parse from here
  std::string buffer_;
  socklen_t addr_len_;
  struct sockaddr_storage addr_;
};

}  // namespace searchserver

#endif  // HTTPCONNECTION_HPP_
