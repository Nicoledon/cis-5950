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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

#include "./HttpUtils.hpp"

#include "./catch.hpp"

using std::string;

using searchserver::URLParser;
using searchserver::decode_URI;
using searchserver::wrapped_read;
using searchserver::wrapped_write;
using searchserver::escape_html;

TEST_CASE("escape_html", "[Test_HttpUtils]") {
  

  string noReplace = "Strom Static Sleep Antennas";
  REQUIRE(noReplace == escape_html(noReplace));

  string ampersand = "Triumph & Disaster";
  REQUIRE("Triumph &amp; Disaster" == escape_html(ampersand));

  string quotes = "\"HKE 2048\"";
  REQUIRE("&quot;HKE 2048&quot;" == escape_html(quotes));

  string apos = "\'Animals\'";
  REQUIRE("&apos;Animals&apos;" == escape_html(apos));

  string angleBrackets = "vectroid<int>";
  REQUIRE("vectroid&lt;int&gt;" == escape_html(angleBrackets));

  string all = "<\"Clouds\" & \'Nevermind The Name\'>";
  string expected;
  expected = "&lt;&quot;Clouds&quot; &amp; &apos;Nevermind The Name&apos;&gt;";
  REQUIRE(expected == escape_html(all));

  
}

TEST_CASE("wrapped_read_write", "[Test_HttpUtils]") {
  string filedata = "This is a test; this is only a test.\n";

  // Make sure the file we'll write/read is deleted.
  unlink("test_files/test.txt");

  // Open the file and write to it.
  int file_fd = open("test_files/test.txt",
                     O_RDWR | O_CREAT,
                     S_IRUSR | S_IWUSR);
  REQUIRE(file_fd != -1);
  REQUIRE(static_cast<int>(filedata.size()) ==
            wrapped_write(file_fd, filedata));
  close(file_fd);

  // Reopen the file and read it in.
  string readstr;
  file_fd = open("test_files/test.txt", O_RDONLY);
  REQUIRE(file_fd != -1);
  REQUIRE(static_cast<int>(filedata.size()) ==
            wrapped_read(file_fd, &readstr));
  close(file_fd);
  REQUIRE(readstr == filedata);

  // Delete the file.
  unlink("test_files/test.txt");
}

TEST_CASE("decode_URI", "[Test_HttpUtils]") {
  // Test out URIDecoding.
  string empty("");
  string plain("foo");
  string two("%74%77%6f");
  string twoupper("%74%77%6F");
  string nope("%16nope");
  string broken("%broken%1");
  string spacey("%20+blah blah");
  REQUIRE(string("") == decode_URI(empty));
  REQUIRE(string("foo") == decode_URI(plain));
  REQUIRE(string("two") == decode_URI(two));
  REQUIRE(string("two") == decode_URI(twoupper));
  REQUIRE(string("%16nope") == decode_URI(nope));
  REQUIRE(string("%broken%1") == decode_URI(broken));
  REQUIRE(string("  blah blah") == decode_URI(spacey));
}

TEST_CASE("URLParser", "[Test_HttpUtils]") {
  // Test out URL parsing.
  string easy("/foo/bar");
  string tricky("/foo/bar?");
  string query("/foo/bar?foo=blah+blah");
  string many("/foo/bar?foo=bar&bam=baz");
  string manyshort("/foo/bar?foo=%22bar%22&bam=baz");

  URLParser p;
  p.parse(easy);
  REQUIRE("/foo/bar" == p.path());
  REQUIRE((unsigned) 0 == p.args().size());

  p.parse(tricky);
  REQUIRE("/foo/bar" == p.path());
  REQUIRE((unsigned) 0 == p.args().size());

  p.parse(query);
  REQUIRE("/foo/bar" == p.path());
  REQUIRE((unsigned) 1 == p.args().size());
  REQUIRE("blah blah" == p.args()["foo"]);

  p.parse(many);
  REQUIRE("/foo/bar" == p.path());
  REQUIRE((unsigned) 2 == p.args().size());
  REQUIRE("bar" == p.args()["foo"]);
  REQUIRE("baz" == p.args()["bam"]);

  p.parse(manyshort);
  REQUIRE("/foo/bar" == p.path());
  REQUIRE((unsigned) 2 == p.args().size());
  REQUIRE("\"bar\"" == p.args()["foo"]);
  REQUIRE("baz" == p.args()["bam"]);
}

