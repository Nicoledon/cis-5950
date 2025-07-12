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

#include "./BufferChecker.hpp"
#include "./BufferedFileReader.hpp"
#include "catch.hpp"
#include <errno.h>
#include <fstream>
#include <string>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
static constexpr const char *kHelloFileName = "./test_files/Hello.txt";
static constexpr const char *kByeFileName = "./test_files/Bye.txt";
static constexpr const char *kLongFileName = "./test_files/war_and_peace.txt";
static constexpr const char *kGreatFileName = "./test_files/mutual_aid.txt";

// helper functions

static bool verify_token(const string &actual, const string &expected_contents,
                         const string &delims, off_t *offset) {
  off_t off = *offset;
  string expected = expected_contents.substr(off, actual.length());
  if (actual != expected) {
    *offset = off;
    return false;
  }

  off += actual.length();
  if (off >= static_cast<off_t>(expected_contents.length())) {
    // eof reached
    *offset = off;
    return true;
  }

  if (delims.find(expected_contents[off]) == string::npos) {
    *offset = off + 1;
    return false;
  }

  off++;

  *offset = off;
  return true;
}

TEST_CASE("Basic", "[Test_BufferedFileReader]") {
  BufferedFileReader *bf = new BufferedFileReader(kHelloFileName);
  char c = bf->get_char();
  REQUIRE('H' == c);

  // Check that the buffer has 'h' in it
  BufferChecker bc(*bf);
  string expected("H");
  REQUIRE_FALSE(bc.check_token_errors(expected, 0));

  // Delete SF to make sure destructor works, then award points
  // if it doesn't crash
  delete bf;
}

TEST_CASE("open_close", "[Test_BufferedFileReader]") {
  // close when already closed
  BufferedFileReader *bf = new BufferedFileReader(kHelloFileName);
  REQUIRE(bf->good());
  bf->close_file();
  REQUIRE_FALSE(bf->good());
  REQUIRE(bf->tell() == -1);
  bf->close_file();
  REQUIRE_FALSE(bf->good());

  // open when already opened
  bf->open_file(kByeFileName);
  REQUIRE(bf->good());
  bf->open_file(kByeFileName);
  REQUIRE(bf->good());
  bf->open_file(kByeFileName);
  REQUIRE(bf->good());

  // open and close the same file over and over again
  for (size_t i = 0; i < 10; i++) {
    bf->open_file(kByeFileName);
    REQUIRE(bf->good());
    bf->close_file();
    REQUIRE_FALSE(bf->good());
    bf->close_file();
    REQUIRE_FALSE(bf->good());
    bf->open_file(kByeFileName);
    REQUIRE(bf->good());
  }

  // destructor on an already closed file
  delete bf;
}

TEST_CASE("get_char", "[Test_BufferedFileReader]") {

  // file contents
  string kHelloContents{};
  string kByeContents{};
  string kLongContents{};
  string kGreatContents{};

  // flil the above strings with the actual contents of the file
  ifstream hello_ifs(kHelloFileName);
  kHelloContents.assign((std::istreambuf_iterator<char>(hello_ifs)),
                        (std::istreambuf_iterator<char>()));
  ifstream bye_ifs(kByeFileName);
  kByeContents.assign((std::istreambuf_iterator<char>(bye_ifs)),
                      (std::istreambuf_iterator<char>()));
  ifstream long_ifs(kLongFileName);
  kLongContents.assign((std::istreambuf_iterator<char>(long_ifs)),
                       (std::istreambuf_iterator<char>()));
  ifstream great_ifs(kGreatFileName);
  kGreatContents.assign((std::istreambuf_iterator<char>(great_ifs)),
                        (std::istreambuf_iterator<char>()));

  // Hello test case
  BufferedFileReader bf(kHelloFileName);
  BufferChecker bc(bf);
  string contents;
  char c;
  for (size_t i = 0; i < kHelloContents.length(); i++) {
    REQUIRE(bf.tell() >= 0);
    REQUIRE(i == static_cast<size_t>(bf.tell()));
    c = bf.get_char();
    contents += c;
    REQUIRE(bf.good());
    REQUIRE_FALSE(bc.check_char_errors(c, i));
  }
  REQUIRE(kHelloContents == contents);
  c = bf.get_char();
  REQUIRE(static_cast<char>(EOF) == c);
  REQUIRE_FALSE(bf.good());

  // Goodbye test case
  bf.close_file();
  bf.open_file(kByeFileName);
  contents.clear();
  for (size_t i = 0; i < kByeContents.length(); i++) {
    REQUIRE(bf.tell() >= 0);
    REQUIRE(i == static_cast<size_t>(bf.tell()));
    c = bf.get_char();
    contents += c;
    REQUIRE(bf.good());
    REQUIRE_FALSE(bc.check_char_errors(c, i));
  }
  REQUIRE(kByeContents == contents);
  c = bf.get_char();
  REQUIRE(static_cast<char>(EOF) == c);
  REQUIRE_FALSE(bf.good());

  // Long file test case
  contents.clear();
  bf.close_file();
  REQUIRE(bf.tell() == -1);
  bf.open_file(kLongFileName);
  contents.reserve(kLongContents.length());
  for (size_t i = 0; i < kLongContents.length(); i++) {
    REQUIRE(bf.tell() >= 0);
    REQUIRE(i == static_cast<size_t>(bf.tell()));
    c = bf.get_char();
    contents += c;
    REQUIRE(bf.good());
    REQUIRE_FALSE(bc.check_char_errors(c, i));
  }
  REQUIRE(kLongContents == contents);
  c = bf.get_char();
  REQUIRE(static_cast<char>(EOF) == c);
  REQUIRE_FALSE(bf.good());

  // "Great" file test case
  contents.clear();
  bf.close_file();
  REQUIRE(bf.tell() == -1);
  bf.open_file(kGreatFileName);
  contents.reserve(kGreatContents.length());
  for (size_t i = 0; i < kGreatContents.length(); i++) {
    REQUIRE(bf.tell() >= 0);
    REQUIRE(i == static_cast<size_t>(bf.tell()));
    c = bf.get_char();
    contents += c;
    REQUIRE(bf.good());
    REQUIRE_FALSE(bc.check_char_errors(c, i));
  }
  REQUIRE(kGreatContents == contents);
  c = bf.get_char();
  REQUIRE(static_cast<char>(EOF) == c);
  REQUIRE_FALSE(bf.good());
}

TEST_CASE("get_token", "[Test_BufferedFileReader]") {
  string token{};
  optional<string> opt{};
  string delims{"\t "};
  off_t offset{0};
  // file contents
  string kHelloContents{};
  string kLongContents{};

  // flil the above strings with the actual contents of the file
  ifstream hello_ifs(kHelloFileName);
  kHelloContents.assign((std::istreambuf_iterator<char>(hello_ifs)),
                        (std::istreambuf_iterator<char>()));
  ifstream long_ifs(kLongFileName);
  kLongContents.assign((std::istreambuf_iterator<char>(long_ifs)),
                       (std::istreambuf_iterator<char>()));
  BufferedFileReader bf(kHelloFileName);
  BufferChecker bc(bf);

  while (bf.good()) {
    opt = bf.get_token(delims);
    REQUIRE(opt.has_value());
    token = opt.value();
    REQUIRE_FALSE(bc.check_token_errors(token, offset));
    REQUIRE(verify_token(token, kHelloContents, delims, &offset));
    REQUIRE(offset == static_cast<off_t>(bf.tell()));
  }

  REQUIRE(static_cast<off_t>(kHelloContents.length()) == offset);

  offset = 0;
  bf.open_file(kLongFileName);
  while (bf.good()) {
    opt = bf.get_token(delims);
    REQUIRE(opt.has_value());
    token = opt.value();
    REQUIRE_FALSE(bc.check_token_errors(token, offset));
    REQUIRE(verify_token(token, kLongContents, delims, &offset));
    REQUIRE(offset == static_cast<off_t>(bf.tell()));
  }

  REQUIRE(static_cast<off_t>(kLongContents.length()) == offset);
  opt = bf.get_token();
  REQUIRE_FALSE(opt.has_value());
}

TEST_CASE("Complex", "[Test_BufferedFileReader]") {

  optional<string> opt{};
  string token;
  string delims = ",\n ";
  off_t offset = 0;
  string kLongContents{};
  ifstream long_ifs(kLongFileName);
  kLongContents.assign((std::istreambuf_iterator<char>(long_ifs)),
                       (std::istreambuf_iterator<char>()));

  BufferedFileReader bf(kLongFileName);
  BufferChecker bc(bf);

  for (int i = 0; i < 3; i++) {
    while (bf.good()) {
      opt = bf.get_token(delims);
      REQUIRE(opt.has_value());
      token = opt.value();
      REQUIRE_FALSE(bc.check_token_errors(token, offset));
      REQUIRE(verify_token(token, kLongContents, delims, &offset));
      REQUIRE(offset == static_cast<off_t>(bf.tell()));
    }

    REQUIRE(static_cast<off_t>(kLongContents.length()) == offset);
    opt = bf.get_token();
    REQUIRE_FALSE(opt.has_value());
    offset = 0;
    bf.rewind();
  }
}

TEST_CASE("Move Ctor", "[Test_BufferedFileReader]") {
  string kGreatContents{};
  ifstream long_ifs(kGreatFileName);
  kGreatContents.assign((std::istreambuf_iterator<char>(long_ifs)),
                       (std::istreambuf_iterator<char>()));

  // use new so we can control when it is destructed
  BufferedFileReader* bf = new BufferedFileReader(kGreatFileName);

  BufferedFileReader& bf0 = *bf;
  BufferChecker bc0(bf0);

  int fd = bc0.fd();
  auto opt = bf0.get_token();
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == "Project");

  auto arr = bc0.buffer();

  // move construct a new bf
  BufferedFileReader bf1(std::move(bf0));
  BufferChecker bc1(bf1);

  REQUIRE(fd == bc1.fd());

  // fd of the original needs to change so that if
  // it closes, then it doesn't close the file in the move destination.
  REQUIRE(fd != bc0.fd());

  // check that the file is not open already
  int old_fd = bc0.fd();
  int res = fcntl(old_fd, F_GETFL);
  int err = errno;
  REQUIRE(res < 0);
  REQUIRE(err == EBADF);

  // open the file descriptor
  // but only if a valid num was used.
  res = dup2(STDOUT_FILENO, old_fd);

  // make sure that the destructor for the moved from bf does not close it
  delete bf;

  // make sure that if we could re-open it
  // that it was not closed by the destructor
  if (res != -1) {
    res = fcntl(old_fd, F_GETFL);
    err = errno;
    REQUIRE(res >= 0);
    REQUIRE(err != EBADF);

    close(old_fd);
  }


  for (size_t i = bc1.curr_index(); i < bc1.curr_length(); i++) {
    REQUIRE(arr[i] == bc1.buffer()[i]);
  }

  off_t offset = 0;
  string token = opt.value();
  REQUIRE(verify_token(token, kGreatContents, " \t\n\r\v\f", &offset));

  // make sure reading the file is ok after moving
  while (bf1.good()) {
    opt = bf1.get_token();
    REQUIRE(opt.has_value());
    token = opt.value();
    REQUIRE_FALSE(bc1.check_token_errors(token, offset));
    REQUIRE(verify_token(token, kGreatContents, " \t\n\r\v\f", &offset));
    REQUIRE(offset == static_cast<off_t>(bf1.tell()));
  }

  REQUIRE(static_cast<off_t>(kGreatContents.length()) == offset);
}

TEST_CASE("Move op=", "[Test_BufferedFileReader]") {
  string kGreatContents{};
  ifstream long_ifs(kGreatFileName);
  kGreatContents.assign((std::istreambuf_iterator<char>(long_ifs)),
                       (std::istreambuf_iterator<char>()));

  // use new so we can control when it is destructed
  BufferedFileReader* bf = new BufferedFileReader(kGreatFileName);

  BufferedFileReader& bf0 = *bf;
  BufferChecker bc0(bf0);

  int fd = bc0.fd();
  auto opt = bf0.get_token();
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == "Project");

  auto arr = bc0.buffer();

  // move assign a new bf
  BufferedFileReader bf1(kHelloFileName);
  bf1 = std::move(bf0);
  BufferChecker bc1(bf1);

  REQUIRE(fd == bc1.fd());

  // fd of the original needs to change so that if
  // it closes, then it doesn't close the file in the move destination.
  REQUIRE(fd != bc0.fd());

  // check that the file is not open already
  int old_fd = bc0.fd();
  int res = fcntl(old_fd, F_GETFL);
  int err = errno;
  REQUIRE(res < 0);
  REQUIRE(err == EBADF);

  // open the file descriptor
  // but only if a valid num was used.
  res = dup2(STDOUT_FILENO, old_fd);

  // make sure that the destructor for the moved from bf does not close it
  delete bf;

  // make sure that if we could re-open it
  // that it was not closed by the destructor
  if (res != -1) {
    res = fcntl(old_fd, F_GETFL);
    err = errno;
    REQUIRE(res >= 0);
    REQUIRE(err != EBADF);

    close(old_fd);
  }


  for (size_t i = bc1.curr_index(); i < bc1.curr_length(); i++) {
    REQUIRE(arr[i] == bc1.buffer()[i]);
  }

  off_t offset = 0;
  string token = opt.value();
  REQUIRE(verify_token(token, kGreatContents, " \t\n\r\v\f", &offset));

  // make sure reading the file is ok after moving
  while (bf1.good()) {
    opt = bf1.get_token();
    REQUIRE(opt.has_value());
    token = opt.value();
    REQUIRE_FALSE(bc1.check_token_errors(token, offset));
    REQUIRE(verify_token(token, kGreatContents, " \t\n\r\v\f", &offset));
    REQUIRE(offset == static_cast<off_t>(bf1.tell()));
  }

  REQUIRE(static_cast<off_t>(kGreatContents.length()) == offset);
}
