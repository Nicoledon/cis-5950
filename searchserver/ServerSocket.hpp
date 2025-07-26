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

#ifndef SERVERSOCKET_HPP_
#define SERVERSOCKET_HPP_

#include <netdb.h>       // for AF_UNSPEC, AF_INET, AF_INET6
#include <cstdint>      // for uint16_t, etc.
#include <sys/types.h>   // for AF_UNSPEC, AF_INET, AF_INET6
#include <sys/socket.h>  // for AF_UNSPEC, AF_INET, AF_INET6
#include <string>
#include <optional>
#include "./HttpSocket.hpp"

namespace searchserver {

// A ServerSocket class abstracts away the messy details of creating a
// TCP listening socket at a specific port and on a (hopefully)
// externally visible IP address.  As well, a ServerSocket helps
// customers accept incoming client connections on the listening
// socket.
class ServerSocket {
 public:
  // This constructor creates a new ServerSocket object and associates
  // it with the provided port number and local address.
  // The given address is of the specified family.
  //
  // The constructor causes the ServerSocket to attempt to create a
  // listening socket and to bind it to the given port number and
  // specified IP address.
  //
  // The ServerSocket is setup so that afterwards its ready to accept connections
  // with the accept method.
  //
  // If the specified family is not ipv4 or ipv6, invalid_argument is thrown
  //
  // If there is an error during setting up the socket then throws runtime_error
  ServerSocket(sa_family_t family, const std::string& address, uint16_t port);

  // The destructor closes the listening socket if it is open.
  ~ServerSocket();

  // This function causes the ServerSocket to attempt to accept
  // an incoming connection from a client.  On failure, returns nullopt.
  // On success, it returns a new HttpSocket.
  std::optional<HttpSocket> accept_client() const;

  // returns the underlying file descriptor
  int fd() const { return listen_sock_fd_; };

  // Move Constructor
  ServerSocket(ServerSocket&& other) : port_(other.port_), listen_sock_fd_(other.listen_sock_fd_) {
    other.listen_sock_fd_ = -1;
  }

  // Move Assignment
  ServerSocket& operator=(ServerSocket&& other) {
    if (this != &other) {
      port_ = other.port_;
      listen_sock_fd_ = other.listen_sock_fd_;
      other.listen_sock_fd_ = -1;
    }
    return *this;
  }

  // Copy Constructor & Copy assignment disabled
  ServerSocket(const ServerSocket& other) = delete;
  ServerSocket& operator=(const ServerSocket& other) = delete;

 private:
  uint16_t port_;
  int listen_sock_fd_;
};

}  // namespace searchserver

#endif  // SERVERSOCKET_HPP_
