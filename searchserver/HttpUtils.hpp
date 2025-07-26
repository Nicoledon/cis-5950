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

#ifndef HTTPUTILS_HPP_
#define HTTPUTILS_HPP_

#include <cstdint>

#include <string>
#include <utility>
#include <map>
#include <vector>
#include <optional>

namespace searchserver {

// splits a string
std::vector<std::string> split(const std::string& input, const std::string& delims);

struct DirEntry {
  std::string name;
  bool is_dir;
};

// Given a string that corresponds to a directory name it returns a vector of directory entries
// which are all the files that are in that directory
std::optional<std::vector<DirEntry>> readdir(const std::string& dirname);

// Replaces all instance of search inside of the string input with the string format
void replace_all(std::string& input, const std::string& search, const std::string& format);

// This function performs HTML escaping in place.  It scans a string
// for dangerous HTML tokens (such as "<") and replaces them with the
// escaped HTML equivalent (such as "&lt;").  This helps to prevent
// XSS attacks.
std::string escape_html(const std::string &from);

// This function performs URI decoding.  It scans a string for
// the "%" escape character and converts the token to the
// appropriate ASCII character.  See the wikipedia article on
// URL encoding for an explanation of what's going on here:
//
//    http://en.wikipedia.org/wiki/Percent-encoding
//
std::string decode_URI(const std::string &from);

// A URL that's part of a web request has the following structure:
//
//   /foo/bar/baz?field=value&field2=value2
//
//      path     ?   args
//
// This class accepts a URL and splits it into these components and
// URIDecode()'s them, allowing the caller to access the components
// through convenient methods.
class URLParser {
 public:
  URLParser() = default;

  void parse(const std::string &url);

  // Return the "path" component of the url, post-uri-decoding.
  std::string path() const { return path_; }

  // Return the "args" component of the url post-uri-decoding.
  // The args component is parsed into a map from field to value.
  std::map<std::string, std::string> args() const { return args_; }

 private:
  std::string url_;
  std::string path_;
  std::map<std::string, std::string> args_;
};

// A wrapper around the write() system call that shields the caller
// from dealing with the ugly issues of partial writes, EINTR, EAGAIN,
// and so on.
//
// Writes the specified string to the file descriptor fd, not including
// the null terminator.  Blocks the caller until either all bytes have been written,
// or an error is encountered.  Returns the total number of bytes written;
// if this number is less than write_len, it's because some fatal error
// was encountered, like the connection being dropped.
size_t wrapped_write(int fd, const std::string& buf);

// A wrapper around the read() system call that shields the caller
// from dealing with the ugly issues of partial reads, EINTR, EAGAIN,
// and so on.
//
// Reads 1024 bytes from the file descriptor fd onto the end of
// the buffer string "buf".  Returns the number of bytes actually
// read.  On fatal error, returns -1.  If EOF is hit and no
// bytes have been read, returns 0.  Might read fewer bytes
// than requested.
size_t wrapped_read(int fd, std::string *buf);

// Below is used to test server socket

// A convenience routine to manufacture a (blocking) socket to the
// host_name and port number provided as arguments.  Hostname can
// be a DNS name or an IP address, in string form.  On success,
// returns a file descriptor thorugh "client_fd" and returns true.
// On failure, returns false.  Caller is responsible for close()'ing
// the file descriptor.
bool connect_to_server(const std::string &host_name, uint16_t port_num,
                    int *client_fd);

// Return a randomly generated port number between 10000 and 40000.
uint16_t rand_port();

}  // namespace searchserver

#endif  // HTTPUTILS_HPP_
