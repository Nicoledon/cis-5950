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

#include "./catch.hpp"

#include "./CrawlFileTree.hpp"
#include "./WordIndex.hpp"

using searchserver::WordIndex;
using searchserver::crawl_filetree;


TEST_CASE("ReadsFromDisk", "[Test_CrawlFileTree]") {
  std::optional<WordIndex> res;

  // Test that it detects a valid directory.
  res = crawl_filetree("./test_tree/bash-4.2/support");
  REQUIRE(res);

  // Test that it detects a non-existant directory.
  res = crawl_filetree("./nonexistent/");
  REQUIRE_FALSE(res);

  // Test that it rejects files (instead of directories).
  res = crawl_filetree("./test_suite.c");
  REQUIRE_FALSE(res);
}

// Tests that WordIndex and CrawlFileTree
// are properly integrated to work with each other.
TEST_CASE("Integration", "[Test_CrawlFileTree]") {
  std::optional<WordIndex> res;
  WordIndex idx;

  string q1 = "equations";
  vector<string> q2 = {"report", "normal"};
  vector<string> q3 = {"report", "suggestions", "normal"};
  vector<string> q4 = {"report", "normal", "foobarbaz"};

  // Crawl the test tree.
  res = crawl_filetree("./test_tree/bash-4.2/support");
  REQUIRE(res);
  idx = res.value();
  REQUIRE(idx.num_words() == 12328);

  // Process query 1, check results.
  auto res1_word = idx.lookup_word(q1);
  auto it = res1_word.begin();

  REQUIRE(res1_word.size() == 2U);
  REQUIRE(it->doc_name == "./test_tree/bash-4.2/support/texi2html");
  it++;
  REQUIRE(it->doc_name == "./test_tree/bash-4.2/support/man2html.c");

  // what happens if we look up q1 again but stored in a vector of size 1?
  vector<string> q1_query {q1};
  auto res1_query = idx.lookup_query(q1_query);
  it = res1_query.begin();
  REQUIRE(res1_query.size() == 2U);
  REQUIRE(it->doc_name == "./test_tree/bash-4.2/support/texi2html");
  REQUIRE(it->rank == 2);
  it++;
  REQUIRE(it->doc_name == "./test_tree/bash-4.2/support/man2html.c");
  REQUIRE(it->rank == 1);

  // Process query 2, check results.
  auto res2 = idx.lookup_query(q2);
  it = res2.begin();
  REQUIRE(res2.size() == 2U);
  REQUIRE(it->rank == 3);
  REQUIRE(it->doc_name == "./test_tree/bash-4.2/support/man2html.c");
  it++;
  REQUIRE(it->rank == 2);
  REQUIRE(it->doc_name == "./test_tree/bash-4.2/support/texi2html");

  // Process query 3, check results.
  auto res3 = idx.lookup_query(q3);
  it = res3.begin();
  REQUIRE(res3.size() == 1U);
  REQUIRE(it->doc_name == "./test_tree/bash-4.2/support/texi2html");
  REQUIRE(it->rank == 3);

  // Process query 4, check results.
  auto res4 = idx.lookup_query(q4);
  REQUIRE(res4.size() == 0U);
}
