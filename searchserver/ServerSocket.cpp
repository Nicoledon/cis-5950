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

#include <unistd.h>      // for close(), fcntl()
#include <sys/types.h>   // for socket(), getaddrinfo(), etc.
#include <sys/socket.h>  // for socket(), getaddrinfo(), etc.
#include <arpa/inet.h>   // for inet_ntop()
#include <netdb.h>       // for getaddrinfo()
#include <cerrno>        // for errno, used by strerror()
#include <cstring>       // for memset, strerror()
#include <iostream>      // for std::cerr, etc.
#include <stdexcept>

#include "./ServerSocket.hpp"

using namespace std;

namespace searchserver {

ServerSocket::ServerSocket(sa_family_t family, const string& address, uint16_t port) : port_(port), listen_sock_fd_() {
  // TODO:
  // - create a stream socket
  // - set the socket option to enable re-use of the port number
  // - bind the socket to the specified address and port
  // - mark the socket as listening
  
  // since the address and port are given to you, you must populate the
  // address structures necessary yourself. You only need to set the
  // port, address and family field of those structs.
  // The rest can be zero'd out.

}

ServerSocket::~ServerSocket() {
  // Close the listening socket if it's not zero.  The rest of this
  // class will make sure to zero out the socket if it is closed
  // elsewhere.
  if (listen_sock_fd_ != -1)
    close(listen_sock_fd_);
  listen_sock_fd_ = -1;
}


optional<HttpSocket> ServerSocket::accept_client() const {
  // TODO accept the next client connection and return it as an HttpSocket object
  // nullopt on error
  return nullopt;
}

}  // namespace searchserver
