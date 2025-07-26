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

#include <cstdlib>
#include <vector>
#include <iostream>


#include "./catch.hpp"

#include "./WordIndex.hpp"

using std::string;
using std::vector;

using searchserver::WordIndex;

TEST_CASE("Simple", "[Test_WordIndex]") {
  
  string doc_name1 = "./HK.hke";
  string doc_name2 = "./Odyssey.home";

  // We need to assign these logically-constant strings into a non-const
  // pointers because the compiler won't let me cast away the const
  // qualifier on a string literal.
  string bananas = "bananas";
  string pears = "pears";
  string apples = "apples";
  string grapes = "grapes";

  WordIndex index;

  // Document 1 has bananas, pears, and apples.
  index.record(bananas, doc_name1);
  index.record(bananas, doc_name1);
  index.record(pears, doc_name1);
  index.record(apples, doc_name1);
  index.record(apples, doc_name1);
  index.record(apples, doc_name1);

  // Document 2 only has apples and bananas.
  index.record(apples, doc_name2);
  index.record(bananas, doc_name2);

  REQUIRE(3U == index.num_words());

  // No results.
  vector<string> q1 {grapes};
  auto res1 = index.lookup_word(grapes);
  REQUIRE(0U == res1.size());
  res1 = index.lookup_query(q1);
  REQUIRE(0U == res1.size());

  

  // One resultant document.
  vector<string> q2{pears};
  auto res2 = index.lookup_query(q2);
  REQUIRE(1U == res2.size());
  auto it = res2.begin();
  REQUIRE(doc_name1 == it->doc_name);
  REQUIRE(1 == it->rank);

  

  // Multiple resultant documents.
  vector<string> q3 {apples};
  auto res3 = index.lookup_query(q3);
  REQUIRE(2U == res3.size());
  it = res3.begin();
  REQUIRE(doc_name1 == it->doc_name);
  REQUIRE(3 == it->rank);
  it++;
  REQUIRE(doc_name2 == it->doc_name);
  REQUIRE(1 == it->rank);

  

  // Multiple search terms.
  vector<string> q4 {apples, bananas};
  auto res4 = index.lookup_query(q4);
  REQUIRE(2U == res4.size());
  it = res4.begin();
  REQUIRE(doc_name1 == it->doc_name);
  REQUIRE(5 == it->rank);
  it++;
  REQUIRE(doc_name2 == it->doc_name);
  REQUIRE(2 == it->rank);
 

  // Multiple search terms: testing different term order.
  vector<string> q5 {bananas, apples};
  auto res5 = index.lookup_query(q5);
  REQUIRE(2U == res5.size());
  it = res5.begin();
  REQUIRE(doc_name1 == it->doc_name);
  REQUIRE(5 == it->rank);
  it++;
  REQUIRE(doc_name2 == it->doc_name);
  REQUIRE(2 == it->rank);
  

  // Multiple search terms: not all documents should be results.
  vector<string> q6 {pears};
  auto res6 = index.lookup_query(q6);
  REQUIRE(1U == res6.size());
  it = res6.begin();
  REQUIRE(doc_name1 == it->doc_name);
  REQUIRE(1 == it->rank);

}


