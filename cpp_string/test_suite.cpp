#define CATCH_CONFIG_MAIN
#include "./catch.hpp"
#include <cstring>  // Does not do our cstring.h this includes C's <string.h>
#include <cstdlib>  // include's C's <stdlib.h>
#include <iostream> // for C++ printing

#include "simple_string.hpp"

using namespace std;

TEST_CASE("one-argument constructor", "[simple_string]") {
  char* s1 = strdup("Seamus");
  char* s2 = strdup("Echoes");
  char* s3 = strdup("Chumbawamba");
  char* s4 = strdup(
      "What shalllll    we use          to   fill       the            "
      "emptyyyy           spaces");

  simple_string ss1(s1);
  REQUIRE(ss1.len() == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(ss1.data() != nullptr);
  REQUIRE(strcmp(ss1.data(), s1) == 0);

  simple_string ss2(s2);
  REQUIRE(ss2.len() == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(ss2.data() != nullptr);
  REQUIRE(strcmp(ss2.data(), s2) == 0);

  simple_string ss3(s3);
  REQUIRE(ss3.len() == 11U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(ss3.data() != nullptr);
  REQUIRE(strcmp(ss3.data(), s3) == 0);

  simple_string ss4(s4);
  REQUIRE(ss4.len() == strlen(s4));
  REQUIRE(simple_string::npos == -1);
  REQUIRE(ss4.data() != nullptr);
  REQUIRE(strcmp(ss4.data(), s4) == 0);

  REQUIRE(strcmp("Seamus", s1) == 0);
  REQUIRE(strcmp("Echoes", s2) == 0);
  REQUIRE(strcmp("Chumbawamba", s3) == 0);
  REQUIRE(strcmp("What shalllll    we use          to   fill       the         "
                 "   emptyyyy           spaces",
                 s4) == 0);

  free(s1);
  free(s2);
  free(s3);
  free(s4);

  char uninitialized[5];
  uninitialized[0] = 'H';
  uninitialized[1] = 'i';
  uninitialized[2] = '\0';
  // last 2 chars in array are purposefullly unitialized
  // this is done to help check for valgrind errors
  // for accessing something you should not access

  simple_string ss5(uninitialized);
  REQUIRE(ss5.len() == 2U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss5.data(), "Hi") == 0);

  REQUIRE(strcmp(uninitialized, "Hi") == 0);

  char s6[] = "";
  simple_string ss6(s6);
  REQUIRE(ss6.len() == 0U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(ss6.data() != nullptr);
  REQUIRE(strcmp(ss6.data(), s6) == 0);
  REQUIRE(strcmp("", s6) == 0);
}