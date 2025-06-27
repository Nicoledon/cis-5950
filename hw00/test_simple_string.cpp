#include "./catch.hpp"
#include <cstring>  // Does not do our cstring.h this includes C's <string.h>
#include <cstdlib>  // include's C's <stdlib.h>
#include <iostream> // for C++ printing

#include "simple_string.hpp"

using namespace std;

TEST_CASE("from_cstring", "[simple_string]") {
  char *s1 = strdup("Seamus");
  char *s2 = strdup("Echoes");
  char *s3 = strdup("Chumbawamba");
  char *s4 = strdup("What shalllll    we use          to   fill       the            emptyyyy           spaces");

  simple_string ss = from_cstring(s1);
  REQUIRE(ss.len == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s1) == 0);
  delete[] ss.data;

  ss = from_cstring(s2);
  REQUIRE(ss.len == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s2) == 0);
  delete[] ss.data;

  ss = from_cstring(s3);
  REQUIRE(ss.len == 11U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s3) == 0);
  delete[] ss.data;

  ss = from_cstring(s4);
  REQUIRE(ss.len == strlen(s4));
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s4) == 0);
  delete[] ss.data;

  REQUIRE(strcmp("Seamus", s1) == 0);
  REQUIRE(strcmp("Echoes", s2) == 0);
  REQUIRE(strcmp("Chumbawamba", s3) == 0);
  REQUIRE(strcmp("What shalllll    we use          to   fill       the            emptyyyy           spaces", s4) == 0);

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

  ss = from_cstring(uninitialized);
  REQUIRE(ss.len == 2U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, "Hi") == 0);
  delete[] ss.data;

  REQUIRE(strcmp(uninitialized, "Hi") == 0);
}

TEST_CASE("destruct", "[simple_string]") {
  char *s1 = strdup("Seamus");
  char *s2 = strdup("Echoes");
  char *s3 = strdup("Chumbawamba");
  char *s4 = strdup("What shalllll    we use          to   fill       the            emptyyyy           spaces");

  simple_string ss = from_cstring(s1);
  REQUIRE(ss.len == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s1) == 0);
  destruct(ss);

  ss = from_cstring(s2);
  REQUIRE(ss.len == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s2) == 0);
  destruct(ss);

  ss = from_cstring(s3);
  REQUIRE(ss.len == 11U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s3) == 0);
  destruct(ss);

  ss = from_cstring(s4);
  REQUIRE(ss.len == strlen(s4));
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s4) == 0);
  destruct(ss);

  REQUIRE(strcmp("Seamus", s1) == 0);
  REQUIRE(strcmp("Echoes", s2) == 0);
  REQUIRE(strcmp("Chumbawamba", s3) == 0);
  REQUIRE(strcmp("What shalllll    we use          to   fill       the            emptyyyy           spaces", s4) == 0);

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

  ss = from_cstring(uninitialized);
  REQUIRE(ss.len == 2U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, "Hi") == 0);
  destruct(ss);

  REQUIRE(strcmp(uninitialized, "Hi") == 0);

  char str[] = "";
  ss = from_cstring(str);
  REQUIRE(strcmp(ss.data, "") == 0);
  REQUIRE(strcmp(str, "") == 0);
  REQUIRE(ss.len == 0U);
  destruct(ss);
}

TEST_CASE("at", "[simple_string]") {
  char *s1 = strdup("Seamus");
  char *s2 = strdup("Echoes");
  char *s3 = strdup("Chumbawamba");

  simple_string ss = from_cstring(s1);
  REQUIRE(ss.len == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s1) == 0);
  REQUIRE(at(ss, 0) == 'S');
  REQUIRE(at(ss, 1) == 'e');
  REQUIRE(at(ss, 2) == 'a');
  REQUIRE(at(ss, 3) == 'm');
  REQUIRE(at(ss, 4) == 'u');
  REQUIRE(at(ss, 5) == 's');
  destruct(ss);

  ss = from_cstring(s2);
  REQUIRE(ss.len == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s2) == 0);
  REQUIRE(at(ss, 0) == 'E');
  REQUIRE(at(ss, 1) == 'c');
  REQUIRE(at(ss, 2) == 'h');
  REQUIRE(at(ss, 3) == 'o');
  REQUIRE(at(ss, 4) == 'e');
  REQUIRE(at(ss, 5) == 's');
  destruct(ss);

  ss = from_cstring(s3);
  REQUIRE(ss.len == 11U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s3) == 0);
  REQUIRE(at(ss, 0) == 'C');
  REQUIRE(at(ss, 1) == 'h');
  REQUIRE(at(ss, 2) == 'u');
  REQUIRE(at(ss, 3) == 'm');
  REQUIRE(at(ss, 4) == 'b');
  REQUIRE(at(ss, 5) == 'a');
  REQUIRE(at(ss, 6) == 'w');
  REQUIRE(at(ss, 7) == 'a');
  REQUIRE(at(ss, 8) == 'm');
  REQUIRE(at(ss, 9) == 'b');
  REQUIRE(at(ss,10) == 'a');
  destruct(ss);

  REQUIRE(strcmp("Seamus", s1) == 0);
  REQUIRE(strcmp("Echoes", s2) == 0);
  REQUIRE(strcmp("Chumbawamba", s3) == 0);

  free(s1);
  free(s2);
  free(s3);
}

TEST_CASE("duplicate", "[simple_string]") {
  char *s1 = strdup("Seamus");
  char *s2 = strdup("Echoes");
  char *s3 = strdup("Chumbawamba");
  char *s4 = strdup("What shalllll    we use          to   fill       the            emptyyyy           spaces");

  simple_string ss = from_cstring(s1);
  simple_string copy = duplicate(ss);
  REQUIRE(ss.len == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s1) == 0);
  REQUIRE(copy.len == ss.len);
  REQUIRE(strcmp(copy.data, ss.data) == 0);
  destruct(ss);
  destruct(copy);

  ss = from_cstring(s2);
  copy = duplicate(ss);
  REQUIRE(ss.len == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s2) == 0);
  REQUIRE(copy.len == ss.len);
  REQUIRE(strcmp(copy.data, ss.data) == 0);
  destruct(ss);
  destruct(copy);

  ss = from_cstring(s3);
  copy = duplicate(ss);
  REQUIRE(ss.len == 11U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s3) == 0);
  REQUIRE(copy.len == ss.len);
  REQUIRE(strcmp(copy.data, ss.data) == 0);
  destruct(ss);
  destruct(copy);

  ss = from_cstring(s4);
  copy = duplicate(ss);
  REQUIRE(ss.len == strlen(s4));
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s4) == 0);
  REQUIRE(copy.len == ss.len);
  REQUIRE(strcmp(copy.data, ss.data) == 0);
  destruct(ss);
  destruct(copy);

  REQUIRE(strcmp("Seamus", s1) == 0);
  REQUIRE(strcmp("Echoes", s2) == 0);
  REQUIRE(strcmp("Chumbawamba", s3) == 0);
  REQUIRE(strcmp("What shalllll    we use          to   fill       the            emptyyyy           spaces", s4) == 0);

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

  ss = from_cstring(uninitialized);
  copy = duplicate(ss);
  REQUIRE(ss.len == 2U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, "Hi") == 0);
  REQUIRE(copy.len == ss.len);
  REQUIRE(strcmp(copy.data, ss.data) == 0);
  destruct(ss);
  destruct(copy);

  REQUIRE(strcmp(uninitialized, "Hi") == 0);

  char str[] = "";
  ss = from_cstring(str);
  copy = duplicate(ss);
  REQUIRE(strcmp(ss.data, "") == 0);
  REQUIRE(strcmp(str, "") == 0);
  REQUIRE(ss.len == 0U);
  REQUIRE(copy.len == ss.len);
  REQUIRE(strcmp(copy.data, ss.data) == 0);
  destruct(ss);
  destruct(copy);
}

TEST_CASE("find", "[simple_string]") {
  char arr1[] = "Hello";
  char arr2[] = "Xen Gorilla";
  char arr3[] = {'X', 'e', 'n', '\0', 'y', 'e', 's'};

  simple_string s1 = from_cstring(arr1);
  simple_string s2 = from_cstring(arr2);
  simple_string s3 = from_cstring(arr3);

  REQUIRE(find(s1, 'H') == 0);
  REQUIRE(find(s1, 'l') == 2);
  REQUIRE(find(s1, 'h') == simple_string::npos);

  REQUIRE(find(s2, ' ') == 3);
  REQUIRE(find(s2, 'G') == 4);

  REQUIRE(find(s3, 'e') == 1);
  REQUIRE(find(s3, 'y') == simple_string::npos);
  REQUIRE(find(s3, 's') == simple_string::npos);

  destruct(s1);
  destruct(s2);
  destruct(s3);

  s1 = from_cstring("");
  REQUIRE(find(s1, '\0') == simple_string::npos);
  REQUIRE(find(s1, 'a') == simple_string::npos);
  destruct(s1);

  REQUIRE(simple_string::npos == -1);

  char* cs1 = strdup("Seamus");
  char* cs2 = strdup("Echoes");
  char* cs3 = strdup("Chumbawamba");
  char* cs4 = strdup(
      "What shalllll    we use          to   fill       the            "
      "emptyyyy           spaces");

  s1 = from_cstring(cs1);
  s2 = from_cstring(cs2);
  s3 = from_cstring(cs3);
  simple_string s4 = from_cstring(cs4);

  REQUIRE(find(s1, 'S') == 0);
  REQUIRE(find(s1, 's') == 5);
  REQUIRE(find(s1, '\0') == simple_string::npos);

  REQUIRE(find(s2, 'c') == 1);
  REQUIRE(find(s2, '.') == simple_string::npos);
  REQUIRE(find(s2, 'e') == 4);

  REQUIRE(find(s3, 'm') == 3);
  REQUIRE(find(s3, 'a') == 5);
  REQUIRE(find(s3, '4') == simple_string::npos);

  REQUIRE(find(s4, ' ') == 4);
  REQUIRE(find(s4, 'y') == 68);
  REQUIRE(find(s4, 'R') == simple_string::npos);
  REQUIRE(find(s4, '\0') == simple_string::npos);

  free(cs1);
  free(cs2);
  free(cs3);
  free(cs4);
  destruct(s1);
  destruct(s2);
  destruct(s3);
  destruct(s4);

  char uninitialized[5];
  uninitialized[0] = 'H';
  uninitialized[1] = 'i';
  uninitialized[2] = '\0';

  simple_string uninit = from_cstring(uninitialized);

  REQUIRE(find(uninit, 'H') == 0);
  REQUIRE(find(uninit, 'a') == simple_string::npos);
  REQUIRE(find(uninit, '\0') == simple_string::npos);

  destruct(uninit);
}

TEST_CASE("operator==", "[simple_string]") {
  simple_string s1 = from_cstring("");
  simple_string s2 = from_cstring("");

  REQUIRE(s1 == s2);
  REQUIRE(s2 == s1);
  REQUIRE(s1 == s1);
  REQUIRE(s2 == s2);

  destruct(s1);
  destruct(s2);

  s1 = from_cstring("hello");
  s2 = from_cstring("hello");

  REQUIRE(s1 == s2);
  REQUIRE(s2 == s1);
  REQUIRE(s1 == s1);
  REQUIRE(s2 == s2);

  destruct(s1);
  destruct(s2);

  s1 = from_cstring("hella");
  s2 = from_cstring("hellb");
  simple_string s3 = from_cstring("hellc");

  REQUIRE_FALSE(s1 == s2);
  REQUIRE_FALSE(s1 == s3);
  REQUIRE_FALSE(s2 == s3);

  destruct(s1);
  destruct(s2);
  destruct(s3);

  s1 = from_cstring("hello");
  s2 = from_cstring("helloa");

  REQUIRE_FALSE(s1 == s2);
  REQUIRE_FALSE(s2 == s1);
  REQUIRE(s1 == s1);
  REQUIRE(s2 == s2);

  destruct(s1);
  destruct(s2);

  s1 = from_cstring("abc");
  s2 = from_cstring("xbc");
  s3 = from_cstring("axc");
  simple_string s4 = from_cstring("abx");

  REQUIRE_FALSE(s1 == s2);
  REQUIRE_FALSE(s2 == s3);
  REQUIRE_FALSE(s1 == s4);
  REQUIRE_FALSE(s2 == s4);
  REQUIRE_FALSE(s2 == s3);
  REQUIRE_FALSE(s3 == s4);

  destruct(s1);
  destruct(s2);
  destruct(s3);
  destruct(s4);

  s1 = from_cstring("hello!");
  s2 = from_cstring("hello?");
  REQUIRE_FALSE(s2 == s1);
  REQUIRE_FALSE(s1 == s2);

  destruct(s1);
  destruct(s2);

  s1 = from_cstring("");
  s2 = from_cstring("nonempty");
  REQUIRE_FALSE(s2 == s1);
  REQUIRE_FALSE(s1 == s2);

  destruct(s1);
  destruct(s2);
}

TEST_CASE("substr", "[simple_string]") {
  char *s1 = strdup("Seamus");
  char *s2 = strdup("Echoes");
  char *s3 = strdup("Chumbawamba");

  simple_string ss = from_cstring(s1);
  
  simple_string sub = substr(ss, 0, 6);
  REQUIRE(sub.len == 6U);
  REQUIRE(strcmp(sub.data, "Seamus") == 0);
  destruct(sub);

  sub = substr(ss, 1, 5);
  REQUIRE(sub.len == 5U);
  REQUIRE(strcmp(sub.data, "eamus") == 0);
  destruct(sub); 

  sub = substr(ss, 2, 4);
  REQUIRE(sub.len == 4U);
  REQUIRE(strcmp(sub.data, "amus") == 0);
  destruct(sub); 

  sub = substr(ss, 3, 3);
  REQUIRE(sub.len == 3U);
  REQUIRE(strcmp(sub.data, "mus") == 0);
  destruct(sub); 

  sub = substr(ss, 4, 2);
  REQUIRE(sub.len == 2U);
  REQUIRE(strcmp(sub.data, "us") == 0);
  destruct(sub); 

  sub = substr(ss, 5, 1);
  REQUIRE(sub.len == 1U);
  REQUIRE(strcmp(sub.data, "s") == 0);
  destruct(sub); 

  REQUIRE(ss.len == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s1) == 0);
  destruct(ss);

  ss = from_cstring(s2);

  sub = substr(ss, 1, 0);
  REQUIRE(sub.len == 0U);
  REQUIRE(strcmp(sub.data, "") == 0);
  destruct(sub); 

  sub = substr(ss, 1, 100);
  REQUIRE(sub.len == 5U);
  REQUIRE(strcmp(sub.data, "choes") == 0);
  destruct(sub); 

  sub = substr(ss, 4, simple_string::npos);
  REQUIRE(sub.len == 2U);
  REQUIRE(strcmp(sub.data, "es") == 0);
  destruct(sub); 

  sub = substr(ss, 3, 43125132);
  REQUIRE(sub.len == 3U);
  REQUIRE(strcmp(sub.data, "oes") == 0);
  destruct(sub); 

  sub = substr(ss, 0, simple_string::npos);
  REQUIRE(sub.len == 6U);
  REQUIRE(strcmp(sub.data, "Echoes") == 0);
  destruct(sub); 

  REQUIRE(ss.len == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s2) == 0);
  destruct(ss);

  ss = from_cstring(s3);

  sub = substr(ss, 4, 43125132);
  REQUIRE(sub.len == 7U);
  REQUIRE(strcmp(sub.data, "bawamba") == 0);
  destruct(sub); 

  sub = substr(ss, 1, 10);
  REQUIRE(sub.len == 10U);
  REQUIRE(strcmp(sub.data, "humbawamba") == 0);
  destruct(sub); 

  sub = substr(ss, 0, simple_string::npos);
  REQUIRE(sub.len == 11U);
  REQUIRE(strcmp(sub.data, "Chumbawamba") == 0);
  destruct(sub); 

  sub = substr(ss, 10, 1);
  REQUIRE(sub.len == 1U);
  REQUIRE(strcmp(sub.data, "a") == 0);
  destruct(sub); 

  sub = substr(ss, 7, 2);
  REQUIRE(sub.len == 2U);
  REQUIRE(strcmp(sub.data, "am") == 0);
  destruct(sub); 

  REQUIRE(ss.len == 11U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss.data, s3) == 0);
  destruct(ss);

  REQUIRE(strcmp("Seamus", s1) == 0);
  REQUIRE(strcmp("Echoes", s2) == 0);
  REQUIRE(strcmp("Chumbawamba", s3) == 0);

  free(s1);
  free(s2);
  free(s3);
}

