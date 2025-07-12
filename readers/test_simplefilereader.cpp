#include "./SimpleFileReader.hpp"
#include "catch.hpp"
#include <errno.h>
#include <fstream>
#include <string>
#include <sys/select.h>
#include <unistd.h>

using namespace std;

static constexpr const char *kHelloFileName = "./test_files/Hello.txt";
static constexpr const char *kByeFileName = "./test_files/Bye.txt";
static constexpr const char *kLongFileName = "./test_files/war_and_peace.txt";
static constexpr const char *kGreatFileName = "./test_files/mutual_aid.txt";

TEST_CASE("Basic", "[Test_SimpleFileReader]") {
  SimpleFileReader *sf = new SimpleFileReader(kHelloFileName);
  char c = sf->get_char();
  REQUIRE('H' == c);

  // Delete SF to make sure destructor works, then award points
  // if it doesn't crash
  delete sf;
}

TEST_CASE("open_close", "[Test_SimpleFileReader]") {
  // close when already closed
  SimpleFileReader *sf = new SimpleFileReader(kHelloFileName);
  REQUIRE(sf->good());
  sf->close_file();
  REQUIRE_FALSE(sf->good());
  sf->close_file();
  REQUIRE_FALSE(sf->good());
  // open when already opened
  sf->open_file(kByeFileName);
  REQUIRE(sf->good());
  sf->open_file(kByeFileName);
  REQUIRE(sf->good());
  sf->open_file(kByeFileName);
  REQUIRE(sf->good());

  // open and close the same file over and over again
  for (size_t i = 0; i < 10; i++) {
    sf->open_file(kByeFileName);
    REQUIRE(sf->good());
    sf->close_file();
    REQUIRE_FALSE(sf->good());
    sf->close_file();
    REQUIRE_FALSE(sf->good());
    sf->open_file(kByeFileName);
    REQUIRE(sf->good());
  }
  // destructor on an already closed file
  delete sf;
}

TEST_CASE("get_char", "[Test_SimpleFileReader]") {

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
  SimpleFileReader sf(kHelloFileName);
  string contents;
  char c;
  for (size_t i = 0; i < kHelloContents.length(); i++) {

    REQUIRE(sf.tell() >= 0);
    REQUIRE(i == static_cast<size_t>(sf.tell()));
    c = sf.get_char();
    contents += c;
    REQUIRE(sf.good());
  }
  REQUIRE(kHelloContents == contents);
  c = sf.get_char();
  REQUIRE(static_cast<char>(EOF) == c);
  REQUIRE_FALSE(sf.good());

  // Goodbye test case
  sf.close_file();
  sf.open_file(kByeFileName);
  contents.clear();
  for (size_t i = 0; i < kByeContents.length(); i++) {

    REQUIRE(sf.tell() >= 0);
    REQUIRE(i == static_cast<size_t>(sf.tell()));
    c = sf.get_char();
    contents += c;
    REQUIRE(sf.good());
  }
  REQUIRE(kByeContents == contents);
  c = sf.get_char();
  REQUIRE(static_cast<char>(EOF) == c);
  REQUIRE_FALSE(sf.good());
  REQUIRE(static_cast<size_t>(sf.tell()) == kByeContents.length());

  // Long file test case
  contents.clear();
  sf.close_file();
  sf.open_file(kLongFileName);
  contents.reserve(kLongContents.length());
  for (size_t i = 0; i < kLongContents.length(); i++) {

    REQUIRE(sf.tell() >= 0);
    REQUIRE(i == static_cast<size_t>(sf.tell()));
    c = sf.get_char();
    contents += c;
    REQUIRE(sf.good());
  }
  REQUIRE(kLongContents == contents);
  c = sf.get_char();
  REQUIRE(static_cast<char>(EOF) == c);
  REQUIRE_FALSE(sf.good());
  REQUIRE(static_cast<size_t>(sf.tell()) == kLongContents.length());

  // "Great" file test case
  contents.clear();
  sf.close_file();
  sf.open_file(kGreatFileName);
  contents.reserve(kGreatContents.length());
  for (size_t i = 0; i < kGreatContents.length(); i++) {

    REQUIRE(sf.tell() >= 0);
    REQUIRE(i == static_cast<size_t>(sf.tell()));
    c = sf.get_char();
    contents += c;
    REQUIRE(sf.good());
  }
  REQUIRE(kGreatContents == contents);
  c = sf.get_char();
  REQUIRE(static_cast<char>(EOF) == c);
  REQUIRE_FALSE(sf.good());
  REQUIRE(static_cast<size_t>(sf.tell()) == kGreatContents.length());
}

TEST_CASE("get_chars", "[Test_SimpleFileReader]") {
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
  SimpleFileReader sf(kHelloFileName);
  string contents;
  string next;
  optional<string> opt;
  size_t n = 1;

  for (size_t i = 0; i < kHelloContents.length(); i += n, n++) {
    opt = sf.get_chars(n);
    REQUIRE(opt.has_value());
    next = opt.value();
    contents += next;
    if (i + n <= kHelloContents.length()) {
      REQUIRE(sf.good());
    } else {
      REQUIRE_FALSE(sf.good());
    }
  }
  REQUIRE(kHelloContents == contents);
  opt = sf.get_chars(1);
  REQUIRE_FALSE(opt.has_value());
  REQUIRE_FALSE(sf.good());
  REQUIRE(static_cast<size_t>(sf.tell()) == kHelloContents.length());

  // Bye test case
  contents.clear();
  sf.close_file();
  sf.open_file(kByeFileName);
  for (size_t i = 0; i < kByeContents.length(); i += n, n++) {
    opt = sf.get_chars(n);
    REQUIRE(opt.has_value());
    next = opt.value();
    contents += next;
    if (i + n <= kByeContents.length()) {
      REQUIRE(sf.good());
    } else {
      REQUIRE_FALSE(sf.good());
    }
  }
  REQUIRE(kByeContents == contents);
  opt = sf.get_chars(1);
  REQUIRE_FALSE(opt.has_value());
  REQUIRE_FALSE(sf.good());
  REQUIRE(static_cast<size_t>(sf.tell()) == kByeContents.length());

  // Long test case
  contents.clear();
  sf.close_file();
  sf.open_file(kLongFileName);
  for (size_t i = 0; i < kLongContents.length(); i += n, n++) {
    opt = sf.get_chars(n);
    REQUIRE(opt.has_value());
    next = opt.value();
    contents += next;
    if (i + n <= kLongContents.length()) {
      REQUIRE(sf.good());
    } else {
      REQUIRE_FALSE(sf.good());
    }
  }
  REQUIRE(kLongContents == contents);
  opt = sf.get_chars(1);
  REQUIRE_FALSE(opt.has_value());
  REQUIRE_FALSE(sf.good());
  REQUIRE(static_cast<size_t>(sf.tell()) == kLongContents.length());

  // Great test case
  contents.clear();
  sf.close_file();
  sf.open_file(kGreatFileName);
  for (size_t i = 0; i < kGreatContents.length(); i += n, n++) {
    opt = sf.get_chars(n);
    REQUIRE(opt.has_value());
    next = opt.value();
    contents += next;
    if (i + n <= kGreatContents.length()) {
      REQUIRE(sf.good());
    } else {
      REQUIRE_FALSE(sf.good());
    }
  }
  REQUIRE(kGreatContents == contents);
  opt = sf.get_chars(1);
  REQUIRE_FALSE(opt.has_value());
  REQUIRE_FALSE(sf.good());
  REQUIRE(static_cast<size_t>(sf.tell()) == kGreatContents.length());
}

TEST_CASE("Complex", "[Test_SimpleFileReader]") {
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
  SimpleFileReader sf(kHelloFileName);
  string contents;
  string next;
  optional<string> opt;
  char c;
  size_t n = 1;

  for (size_t i = 0; i < 3; i++) {
    for (size_t j = 0; j < kHelloContents.length(); n++) {
      if (j % 2) {
        c = sf.get_char();
        contents += c;
        j++;
      } else {
        opt = sf.get_chars(n);
        REQUIRE(opt.has_value());
        next = opt.value();
        contents += next;
        j += n;
      }
      if (j <= kHelloContents.length()) {
        REQUIRE(sf.good());
      } else {
        REQUIRE_FALSE(sf.good());
      }
    }
    REQUIRE(kHelloContents == contents);
    c = sf.get_char();
    REQUIRE(static_cast<char>(EOF) == c);
    REQUIRE_FALSE(sf.good());
    sf.rewind();
    contents.clear();
  }

  // Great test case

  // clear previous contents of "contents"
  contents.clear();

  // open and close repeatedly
  sf.close_file();
  sf.close_file();
  REQUIRE(sf.tell() == -1);
  sf.close_file();
  REQUIRE(sf.tell() == -1);
  sf.open_file(kGreatFileName);
  sf.close_file();
  REQUIRE(sf.tell() == -1);
  sf.open_file(kGreatFileName);
  sf.close_file();
  REQUIRE(sf.tell() == -1);
  sf.open_file(kGreatFileName);

  n = 0;
  for (size_t i = 0; i < kGreatContents.length(); n++) {
    if (!(i % 2)) {
      c = sf.get_char();
      contents += c;
      i++;
    } else {
      opt = sf.get_chars(n);
      REQUIRE(opt.has_value());
      next = opt.value();
      contents += next;
      i += n;
    }
    if (i <= kGreatContents.length()) {
      REQUIRE(sf.good());
    } else {
      REQUIRE_FALSE(sf.good());
    }
  }
  REQUIRE(kGreatContents == contents);
  opt = sf.get_chars(1);
  REQUIRE_FALSE(opt.has_value());
  REQUIRE_FALSE(sf.good());
  REQUIRE(static_cast<size_t>(sf.tell()) == kGreatContents.length());
}
