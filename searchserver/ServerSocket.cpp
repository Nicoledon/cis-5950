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

#include <arpa/inet.h> // for inet_ntop()
#include <cerrno>      // for errno, used by strerror()
#include <cstring>     // for memset, strerror()
#include <iostream>    // for std::cerr, etc.
#include <netdb.h>     // for getaddrinfo()
#include <stdexcept>
#include <sys/socket.h> // for socket(), getaddrinfo(), etc.
#include <sys/types.h>  // for socket(), getaddrinfo(), etc.
#include <unistd.h>     // for close(), fcntl()

#include "./ServerSocket.hpp"

using namespace std;

namespace searchserver {

ServerSocket::ServerSocket(sa_family_t family, const string &address,
                           uint16_t port)
    : port_(port), listen_sock_fd_() {
  // TODO:
  // - create a stream socket
  // - set the socket option to enable re-use of the port number
  // - bind the socket to the specified address and port
  // - mark the socket as listening

  // since the address and port are given to you, you must populate the
  // address structures necessary yourself. You only need to set the
  // port, address and family field of those structs.
  // The rest can be zero'd out.
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;      // IPv6 (also handles IPv4 clients)
  hints.ai_socktype = SOCK_STREAM; // stream
  //hints.ai_flags |= AI_V6MAPPED;   // use v4-mapped v6 if no v6 found
  hints.ai_protocol = IPPROTO_TCP; // tcp protocol
  hints.ai_canonname = nullptr;
  hints.ai_addr = nullptr;
  hints.ai_next = nullptr;
  struct addrinfo *result;
  string str_port = to_string(port);
  int res = getaddrinfo(address.c_str(), str_port.c_str(), &hints, &result);
  if (res != 0) {
    std::cerr << "getaddrinfo() failed: ";
    std::cerr << gai_strerror(res) << std::endl;
    exit(0);
  }
  int listen_fd = -1;
  for (struct addrinfo *rp = result; rp != nullptr; rp = rp->ai_next) {
    listen_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (listen_fd == -1) {
      // Creating this socket failed.  So, loop to the next returned
      // result and try again.
      std::cerr << "socket() failed: " << strerror(errno) << std::endl;
      listen_fd = 0;
      continue;
    }
    int optval = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // Try binding the socket to the address and port number returned
    // by getaddrinfo().
    if (bind(listen_fd, rp->ai_addr, rp->ai_addrlen) == 0) {
      // Bind worked!  Print out the information about what
      // we bound to.
      break;
    }
    close(listen_fd);
  }
  freeaddrinfo(result);
  if (listen(listen_fd,SOMAXCONN) != 0) {
    std::cerr << "Failed to mark socket as listening: ";
    std::cerr << strerror(errno) << std::endl;
    close(listen_fd);
    exit(0);
  }
  listen_sock_fd_ = listen_fd;
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
  // TODO  accept the next client connection and return it as an HttpSocket
  // object nullopt on error
  struct sockaddr_storage caddr;
  socklen_t caddr_len = sizeof(struct sockaddr_storage);
  int client_fd =
      accept(this->listen_sock_fd_, reinterpret_cast<struct sockaddr*>(&caddr),
             &caddr_len);
  if (client_fd < 0) {
    return nullopt;
  }
  return HttpSocket(client_fd, caddr_len,
                    reinterpret_cast<struct sockaddr*>(&caddr));
}

} // namespace searchserver
