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

#include <cstdint>
#include <string>
#include <arpa/inet.h>
#include <array>
#include <cstring>

#include "./HttpSocket.hpp"
#include "./HttpUtils.hpp"

using std::map;
using std::string;
using std::vector;
using std::optional;
using std::nullopt;
using std::array;

namespace searchserver {

static const char * const kHeaderEnd = "\r\n\r\n";
static const int kHeaderEndLen = 4;

optional<string> HttpSocket::next_request() {
  // Use "wrapped_read" to read data into the buffer_
  // instance variable.  Keep reading data until either the
  // connection drops or you see a "\r\n\r\n" that demarcates
  // the end of the request header. Be sure to try and read in
  // a large amount of bytes each time you call wrapped_read.
  //
  // Once you've seen the request header, use parse_request()
  // to parse the header into the *request argument.
  //
  // Very tricky part:  clients can send back-to-back requests
  // on the same socket.  So, you need to preserve everything
  // after the "\r\n\r\n" in buffer_ for the next time the
  // caller invokes next_request()!

  // TODO
  size_t end_of_req = 0;

  return std::nullopt;
}

bool HttpSocket::write_response(const std::string& response) const {
  // TODO
  return false;
}

// Below functions are given to you
// they just get some information about the connection.
string HttpSocket::client_addr() const {
  array<char, INET6_ADDRSTRLEN> addrbuf{};
  if (addr_.ss_family == AF_INET) {
    inet_ntop(AF_INET, &(reinterpret_cast<const struct sockaddr_in*>(&addr_)->sin_addr), addrbuf.data(), INET_ADDRSTRLEN);
  } else {
    inet_ntop(AF_INET6, &(reinterpret_cast<const struct sockaddr_in6*>(&addr_)->sin6_addr), addrbuf.data(), INET6_ADDRSTRLEN);
  }
  return {addrbuf.data()};
}

uint16_t HttpSocket::client_port() const {
  if (addr_.ss_family == AF_INET) {
    return ntohs(reinterpret_cast<const struct sockaddr_in*>(&addr_)->sin_port);
  }
  return ntohs(reinterpret_cast<const struct sockaddr_in6*>(&addr_)->sin6_port);
}

string HttpSocket::server_addr() const {
  struct sockaddr_storage srvr{};
  socklen_t srvrlen = sizeof(srvr);
  getsockname(fd_, reinterpret_cast<struct sockaddr *>(&srvr), &srvrlen);

  array<char, INET6_ADDRSTRLEN> addrbuf{};
  if (srvr.ss_family == AF_INET) {
    inet_ntop(AF_INET, &(reinterpret_cast<const struct sockaddr_in*>(&srvr)->sin_addr), addrbuf.data(), INET_ADDRSTRLEN);
  } else {
    inet_ntop(AF_INET6, &(reinterpret_cast<const struct sockaddr_in6*>(&srvr)->sin6_addr), addrbuf.data(), INET6_ADDRSTRLEN);
  }
  return {addrbuf.data()};
}

uint16_t HttpSocket::server_port() const {
  struct sockaddr_storage srvr{};
  socklen_t srvrlen = sizeof(srvr);
  getsockname(fd_, reinterpret_cast<struct sockaddr *>(&srvr), &srvrlen);
  if (srvr.ss_family == AF_INET) {
    return ntohs(reinterpret_cast<const struct sockaddr_in*>(&srvr)->sin_port);
  }
  return ntohs(reinterpret_cast<const struct sockaddr_in6*>(&srvr)->sin6_port);
}

}  // namespace searchserver
