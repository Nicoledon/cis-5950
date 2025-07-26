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

// This file contains a number of HTTP and HTML parsing routines
// that come in useful throughput the assignment.

#include <arpa/inet.h>
#include <cerrno>
#include <climits>
#include <netdb.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <array>
#include "./HttpUtils.hpp"

using std::cerr;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::vector;
using std::nullopt;
using std::optional;
using std::array;

namespace searchserver {

vector<string> split(const string& input, const string& delims) {
  vector<string> tokens;
  // You don't have to implement thius function, but likely would be helpful
  // we would give it to you if it weren't an answer to past HW assignments
  size_t pos = input.find_first_of(delims);
  size_t start = 0;
  while(pos != string::npos){
     string sub = input.substr(start , pos - start);
     start +=  pos + 1;
     tokens.push_back(sub);
     pos = input.find_first_of(delims);
  }
  tokens.push_back(input.substr(start));
  // if you want to use the URI parser you MUST implement this function
  
  return tokens;
}

optional<vector<DirEntry>> readdir(const string& dirname) {
  vector<DirEntry> entries;

  struct dirent *dirent = nullptr;
  struct stat st{};

  int res = stat(dirname.c_str(), &st);
  if (res != 0) {
    return nullopt;
  }

  if (S_ISDIR(st.st_mode)) {
    DIR* dir = opendir(dirname.c_str());
    dirent = readdir(dir);
    while (dirent != nullptr) {
      
      string path = dirname + '/';
      path += dirent->d_name;

      if (stat(path.c_str(), &st) == 0) {
        if (S_ISREG(st.st_mode)) {
          DirEntry entry = {dirent->d_name, false};
          entries.push_back(std::move(entry));
        } else if (S_ISDIR(st.st_mode)) {
          DirEntry entry = {dirent->d_name, true};
          entries.push_back(std::move(entry));
        }
      }

      dirent = readdir(dir);
    }
    closedir(dir);
  } else {
    return nullopt;
  }

  return entries;
}

void replace_all(string& input, const string& search, const string& format) {
  size_t pos = input.find(search);
  while (pos != string::npos) {
    input.replace(pos, search.size(), format);
    pos += format.size();
    pos = input.find(search, pos);
  }
}

string escape_html(const string &from) {
  string ret = from;
  // Read through the passed in string, and replace any unsafe
  // html tokens with the proper escape codes. The characters
  // that need to be escaped in HTML are the same five as those
  // that need to be escaped for XML documents. You can see an
  // example in the comment for this function in HttpUtils.h and
  // the rest of the characters that need to be replaced can be
  // looked up online.

  replace_all(ret, "&",  "&amp;");
  replace_all(ret, "\"", "&quot;");
  replace_all(ret, "\'", "&apos;");
  replace_all(ret, "<",  "&lt;");
  replace_all(ret, ">",  "&gt;");

  return ret;
}

// Look for a "%XY" token in the string, where XY is a
// hex number.  Replace the token with the appropriate ASCII
// character, but only if 32 <= dec(XY) <= 127.
string decode_URI(const string &from) {
  string retstr;

  // Loop through the characters in the string.
  for (unsigned int pos = 0; pos < from.length(); pos++) {
    // note: use pos+n<from.length() instead of pos<from.length-n
    // to avoid overflow problems with unsigned int values
    char c1 = from[pos];
    char c2 = (pos+1 < from.length()) ? static_cast<char>(toupper(from[pos+1])) : ' ';
    char c3 = (pos+2 < from.length()) ? static_cast<char>(toupper(from[pos+2])) : ' ';

    // Special case the '+' for old encoders.
    if (c1 == '+') {
      retstr.append(1, ' ');
      continue;
    }

    // Is this an escape sequence?
    if (c1 != '%') {
      retstr.append(1, c1);
      continue;
    }

    // Yes.  Are the next two characters hex digits?
    if (!((('0' <= c2) && (c2 <= '9')) ||
          (('A' <= c2) && (c2 <= 'F')))) {
      retstr.append(1, c1);
      continue;
    }
    if (!((('0' <= c3) && (c3 <= '9')) ||
           (('A' <= c3) && (c3 <= 'F')))) {
      retstr.append(1, c1);
      continue;
    }

    // Yes.  Convert to a code.
    uint8_t code = 0;
    if (c2 >= 'A') {
      code = 16 * (10 + (c2 - 'A'));
    } else {
      code = 16 * (c2 - '0');
    }
    if (c3 >= 'A') {
      code += 10 + (c3 - 'A');
    } else {
      code += (c3 - '0');
    }

    // Is the code reasonable?
    if (!((code >= 32) && (code <= 127))) {
      retstr.append(1, c1);
      continue;
    }

    // Great!  Convert and append.
    retstr.append(1, static_cast<char>(code));
    pos += 2;
  }
  return retstr;
}

void URLParser::parse(const string &url) {
  url_ = url;

  // Split the URL into the path and the args components.
  vector<string> ps;
  ps = split(url, "?");
  if (ps.empty()) {
    return;
  }

  // Store the URI-decoded path.
  path_ = decode_URI(ps[0]);

  if (ps.size() < 2)
    return;

  // Split the args into each field=val; chunk.
  vector<string> vals = split(ps[1], "&");

  // Iterate through the chunks.
  for (const auto& val : vals) {
    // Split the chunk into field, value.
    vector<string> fv = split(val, "=");
    if (fv.size() == 2) {
      // Add the field, value to the args_ map.
      args_[decode_URI(fv[0])] = decode_URI(fv[1]);
    }
  }
}

size_t wrapped_read(int fd, string *buf) {
  ssize_t res = 0;
  array<char, 1024> buffer{};
  while (true) {
    res = read(fd, buffer.data(), 1024);
    if (res == -1) {
      if ((errno == EAGAIN) || (errno == EINTR))
        continue;
    }
    break;
  }
  *buf += string(buffer.data(), res);
  return static_cast<size_t>(res);
}

size_t wrapped_write(int fd, const string& buf)  {
  ssize_t res = 0;
  ssize_t written_so_far = 0;

  while (written_so_far < buf.size()) {
    res = write(fd, buf.c_str() + written_so_far, buf.size() - written_so_far);
    if (res == -1) {
      if ((errno == EAGAIN) || (errno == EINTR))
        continue;
      break;
    }
    if (res == 0)
      break;
    written_so_far += res;
  }
  return static_cast<size_t>(written_so_far);
}

bool connect_to_server(const string &host_name, uint16_t port_num,
                     int *client_fd) {
  struct addrinfo hints{};
  struct addrinfo *results = nullptr;
  struct addrinfo *r = nullptr;
  int client_sock = 0;
  int ret_val = 0;
  string port_str{};

  // Convert the port number to a C-style string.
  port_str = std::to_string(port_num);


  // Zero out the hints data structure using memset.
  memset(&hints, 0, sizeof(hints));

  // Indicate we're happy with either AF_INET or AF_INET6 addresses.
  hints.ai_family = AF_UNSPEC;

  // Constrain the answers to SOCK_STREAM addresses.
  hints.ai_socktype = SOCK_STREAM;

  // Do the lookup.
  ret_val = getaddrinfo(host_name.c_str(),
                            port_str.c_str(),
                            &hints,
                            &results);
  if (ret_val != 0) {
    cerr << "getaddrinfo failed: ";
    cerr << gai_strerror(ret_val) << endl;
    return false;
  }

  // Loop through, trying each out until one succeeds.
  for (r = results; r != nullptr; r = r->ai_next) {
    // Try manufacturing the socket.
    client_sock = socket(r->ai_family, SOCK_STREAM, 0);
    if (client_sock == -1) {
      continue;
    }
    // Try connecting to the peer.
    if (connect(client_sock, r->ai_addr, r->ai_addrlen) == -1) {
      continue;
    }
    *client_fd = client_sock;
    freeaddrinfo(results);
    return true;
  }
  freeaddrinfo(results);
  return false;
}

uint16_t rand_port() {
  uint16_t portnum = 10000;
  portnum += static_cast<uint16_t>(getpid()) % 25000;
  std::random_device rd;
  std::mt19937_64 r64 {rd()};
  std::uniform_int_distribution<> distrib(0, 4999);
  portnum += (static_cast<uint16_t>(distrib(r64))) % 5000;
  return portnum;
}


}  // namespace searchserver
