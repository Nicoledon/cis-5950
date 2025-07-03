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

TEST_CASE("at", "[simple_string]") {
  char* s1 = strdup("Seamus");
  char* s2 = strdup("Echoes");
  char* s3 = strdup("Chumbawamba");

  simple_string ss1(s1);
  REQUIRE(ss1.len() == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss1.data(), s1) == 0);
  REQUIRE(ss1.at(0) == 'S');
  REQUIRE(ss1.at(1) == 'e');
  REQUIRE(ss1.at(2) == 'a');
  REQUIRE(ss1.at(3) == 'm');
  REQUIRE(ss1.at(4) == 'u');
  REQUIRE(ss1.at(5) == 's');

  REQUIRE_THROWS_AS(ss1.at(6), out_of_range);
  REQUIRE_THROWS_AS(ss1.at(100), out_of_range);
  REQUIRE_THROWS_AS(ss1.at(-1), out_of_range);

  simple_string ss2(s2);
  REQUIRE(ss2.len() == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss2.data(), s2) == 0);
  REQUIRE(ss2.at(0) == 'E');
  REQUIRE(ss2.at(1) == 'c');
  REQUIRE(ss2.at(2) == 'h');
  REQUIRE(ss2.at(3) == 'o');
  REQUIRE(ss2.at(4) == 'e');
  REQUIRE(ss2.at(5) == 's');

  REQUIRE_THROWS_AS(ss2.at(6), out_of_range);
  REQUIRE_THROWS_AS(ss2.at(100), out_of_range);
  REQUIRE_THROWS_AS(ss2.at(-1), out_of_range);

  simple_string ss3(s3);
  REQUIRE(ss3.len() == 11U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss3.data(), s3) == 0);
  REQUIRE(ss3.at(0) == 'C');
  REQUIRE(ss3.at(1) == 'h');
  REQUIRE(ss3.at(2) == 'u');
  REQUIRE(ss3.at(3) == 'm');
  REQUIRE(ss3.at(4) == 'b');
  REQUIRE(ss3.at(5) == 'a');
  REQUIRE(ss3.at(6) == 'w');
  REQUIRE(ss3.at(7) == 'a');
  REQUIRE(ss3.at(8) == 'm');
  REQUIRE(ss3.at(9) == 'b');
  REQUIRE(ss3.at(10) == 'a');

  REQUIRE_THROWS_AS(ss3.at(11), out_of_range);
  REQUIRE_THROWS_AS(ss3.at(100), out_of_range);
  REQUIRE_THROWS_AS(ss3.at(-1), out_of_range);

  REQUIRE(strcmp("Seamus", s1) == 0);
  REQUIRE(strcmp("Echoes", s2) == 0);
  REQUIRE(strcmp("Chumbawamba", s3) == 0);

  free(s1);
  free(s2);
  free(s3);
}

TEST_CASE("copy constructor", "[simple_string]") {
  char* s1 = strdup("Seamus");
  char* s2 = strdup("Echoes");
  char* s3 = strdup("Chumbawamba");
  char* s4 = strdup(
      "What shalllll    we use          to   fill       the            "
      "emptyyyy           spaces");

  simple_string ss1(s1);
  simple_string copy1(ss1);
  REQUIRE(ss1.len() == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(ss1.data() != nullptr);
  REQUIRE(strcmp(ss1.data(), s1) == 0);
  REQUIRE(copy1.len() == ss1.len());
  REQUIRE(strcmp(copy1.data(), ss1.data()) == 0);
  REQUIRE_FALSE(ss1.data() == copy1.data());

  simple_string ss2(s2);
  simple_string copy2(ss2);
  REQUIRE(ss2.len() == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(ss2.data() != nullptr);
  REQUIRE(strcmp(ss2.data(), s2) == 0);
  REQUIRE(copy2.len() == ss2.len());
  REQUIRE(strcmp(copy2.data(), ss2.data()) == 0);
  REQUIRE_FALSE(ss2.data() == copy2.data());

  simple_string ss3(s3);
  simple_string copy3(ss3);
  REQUIRE(ss3.len() == 11U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(ss3.data() != nullptr);
  REQUIRE(strcmp(ss3.data(), s3) == 0);
  REQUIRE(copy3.len() == ss3.len());
  REQUIRE(strcmp(copy3.data(), ss3.data()) == 0);
  REQUIRE_FALSE(ss3.data() == copy3.data());

  simple_string ss4(s4);
  simple_string copy4(ss4);
  REQUIRE(ss4.len() == strlen(s4));
  REQUIRE(simple_string::npos == -1);
  REQUIRE(ss4.data() != nullptr);
  REQUIRE(strcmp(ss4.data(), s4) == 0);
  REQUIRE(copy4.len() == ss4.len());
  REQUIRE(strcmp(copy4.data(), ss4.data()) == 0);
  REQUIRE_FALSE(ss4.data() == copy4.data());

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
  simple_string copy5(ss5);
  REQUIRE(ss5.len() == 2U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(ss5.data() != nullptr);
  REQUIRE(strcmp(ss5.data(), "Hi") == 0);
  REQUIRE(copy5.len() == ss5.len());
  REQUIRE(strcmp(copy5.data(), ss5.data()) == 0);
  REQUIRE_FALSE(ss5.data() == copy5.data());

  REQUIRE(strcmp(uninitialized, "Hi") == 0);

  char str[] = "";
  simple_string ss6(str);
  simple_string copy6(ss6);
  REQUIRE(ss1.data() != nullptr);
  REQUIRE(strcmp(ss6.data(), "") == 0);
  REQUIRE(strcmp(str, "") == 0);
  REQUIRE(ss6.len() == 0U);
  REQUIRE(copy6.len() == ss6.len());
  REQUIRE(strcmp(copy6.data(), ss6.data()) == 0);
  REQUIRE_FALSE(ss6.data() == copy6.data());
}

TEST_CASE("find", "[simple_string]") {
  char arr1[] = "Hello";
  char arr2[] = "Xen Gorilla";
  char arr3[] = {'X', 'e', 'n', '\0', 'y', 'e', 's'};

  simple_string s1(arr1);
  simple_string s2(arr2);
  simple_string s3(arr3);

  REQUIRE(s1.find('H') == 0);
  REQUIRE(s1.find('l') == 2);
  REQUIRE(s1.find('h') == simple_string::npos);

  REQUIRE(s2.find(' ') == 3);
  REQUIRE(s2.find('G') == 4);

  REQUIRE(s3.find('e') == 1);
  REQUIRE(s3.find('y') == simple_string::npos);
  REQUIRE(s3.find('s') == simple_string::npos);

  simple_string s4("");
  REQUIRE(s4.find('\0') == simple_string::npos);
  REQUIRE(s4.find('a') == simple_string::npos);

  REQUIRE(simple_string::npos == -1);

  char* cs1 = strdup("Seamus");
  char* cs2 = strdup("Echoes");
  char* cs3 = strdup("Chumbawamba");
  char* cs4 = strdup(
      "What shalllll    we use          to   fill       the            "
      "emptyyyy           spaces");
  char* cs5 = strdup("");
  char* cs6 = strdup("CIT 5950: Computer Systems Programming");

  simple_string s5(cs1);
  simple_string s6(cs2);
  simple_string s7(cs3);
  simple_string s8(cs4);
  simple_string s9(cs5);
  simple_string s10(cs6);

  REQUIRE(s5.find('S') == 0);
  REQUIRE(s5.find('s') == 5);
  REQUIRE(s5.find('\0') == simple_string::npos);

  REQUIRE(s6.find('c') == 1);
  REQUIRE(s6.find('.') == simple_string::npos);
  REQUIRE(s6.find('e') == 4);

  REQUIRE(s7.find('m') == 3);
  REQUIRE(s7.find('a') == 5);
  REQUIRE(s7.find('4') == simple_string::npos);

  REQUIRE(s8.find(' ') == 4);
  REQUIRE(s8.find('y') == 68);
  REQUIRE(s8.find('R') == simple_string::npos);
  REQUIRE(s8.find('\0') == simple_string::npos);

  REQUIRE(s9.find(' ') == simple_string::npos);
  REQUIRE(s9.find('h') == simple_string::npos);
  REQUIRE(s9.find('5') == simple_string::npos);
  REQUIRE(s9.find('\0') == simple_string::npos);

  REQUIRE(s10.find('C') == 0);
  REQUIRE(s10.find('5') == 4);
  REQUIRE(s10.find(':') == 8);
  REQUIRE(s10.find('z') == simple_string::npos);
  REQUIRE(s10.find('6') == simple_string::npos);

  free(cs1);
  free(cs2);
  free(cs3);
  free(cs4);
  free(cs5);
  free(cs6);

  char uninitialized[5];
  uninitialized[0] = 'H';
  uninitialized[1] = 'i';
  uninitialized[2] = '\0';

  simple_string uninit(uninitialized);

  REQUIRE(uninit.find('H') == 0);
  REQUIRE(uninit.find('a') == simple_string::npos);
  REQUIRE(uninit.find('\0') == simple_string::npos);
}

TEST_CASE("operator==", "[simple_string]") {
  simple_string s1("");
  simple_string s2("");

  REQUIRE(s1 == s2);
  REQUIRE(s2 == s1);
  REQUIRE(s1 == s1);
  REQUIRE(s2 == s2);

  simple_string s3("hello");
  simple_string s4("hello");

  REQUIRE(s3 == s4);
  REQUIRE(s4 == s3);
  REQUIRE(s3 == s3);
  REQUIRE(s4 == s4);

  simple_string s5("hella");
  simple_string s6("hellb");
  simple_string s7("hellc");

  REQUIRE_FALSE(s5 == s6);
  REQUIRE_FALSE(s5 == s7);
  REQUIRE_FALSE(s6 == s7);

  simple_string s8("hello");
  simple_string s9("helloa");

  REQUIRE_FALSE(s8 == s9);
  REQUIRE_FALSE(s9 == s8);
  REQUIRE(s8 == s8);
  REQUIRE(s9 == s9);

  simple_string s10("abc");
  simple_string s11("xbc");
  simple_string s12("axc");
  simple_string s13("abx");

  REQUIRE_FALSE(s10 == s11);
  REQUIRE_FALSE(s11 == s12);
  REQUIRE_FALSE(s10 == s13);
  REQUIRE_FALSE(s11 == s13);
  REQUIRE_FALSE(s11 == s12);
  REQUIRE_FALSE(s12 == s13);

  simple_string s14("hello!");
  simple_string s15("hello?");
  REQUIRE_FALSE(s15 == s14);
  REQUIRE_FALSE(s14 == s15);

  simple_string s16("");
  simple_string s17("nonempty");
  REQUIRE_FALSE(s17 == s16);
  REQUIRE_FALSE(s16 == s17);

  simple_string s18("Hello");
  simple_string s19("hello");
  simple_string s20("HELLO");
  REQUIRE_FALSE(s18 == s19);
  REQUIRE_FALSE(s18 == s20);
  REQUIRE_FALSE(s19 == s20);

  simple_string s21("CIT 5950");
  simple_string s22("CIT 5950");
  simple_string s23("CIT5950");
  simple_string s24("cit5950");
  simple_string s25("cit5960");
  REQUIRE(s21 == s22);
  REQUIRE_FALSE(s21 == s23);
  REQUIRE_FALSE(s21 == s24);
  REQUIRE_FALSE(s21 == s25);
  REQUIRE_FALSE(s23 == s24);
  REQUIRE_FALSE(s24 == s25);

  simple_string s26("CIT 5950: Computer Systems Programming");
  simple_string s27("CIT 5950: Computer Systems Programming");
  REQUIRE(s26 == s27);
}

TEST_CASE("substr", "[simple_string]") {
  char* s1 = strdup("Seamus");
  char* s2 = strdup("Echoes");
  char* s3 = strdup("Chumbawamba");

  simple_string ss1(s1);

  simple_string sub11 = ss1.substr(0, 6);
  REQUIRE(sub11.len() == 6U);
  REQUIRE(strcmp(sub11.data(), "Seamus") == 0);

  simple_string sub12 = ss1.substr(1, 5);
  REQUIRE(sub12.len() == 5U);
  REQUIRE(strcmp(sub12.data(), "eamus") == 0);

  simple_string sub13 = ss1.substr(2, 4);
  REQUIRE(sub13.len() == 4U);
  REQUIRE(strcmp(sub13.data(), "amus") == 0);

  simple_string sub14 = ss1.substr(3, 3);
  REQUIRE(sub14.len() == 3U);
  REQUIRE(strcmp(sub14.data(), "mus") == 0);

  simple_string sub15 = ss1.substr(4, 2);
  REQUIRE(sub15.len() == 2U);
  REQUIRE(strcmp(sub15.data(), "us") == 0);

  simple_string sub16 = ss1.substr(5, 1);
  REQUIRE(sub16.len() == 1U);
  REQUIRE(strcmp(sub16.data(), "s") == 0);

  simple_string sub17 = ss1.substr(0, 3);
  REQUIRE(sub17.len() == 3U);
  REQUIRE(strcmp(sub17.data(), "Sea") == 0);

  simple_string sub18 = ss1.substr(1, 2);
  REQUIRE(sub18.len() == 2U);
  REQUIRE(strcmp(sub18.data(), "ea") == 0);

  simple_string sub19 = ss1.substr(4, 3);
  REQUIRE(sub19.len() == 2U);
  REQUIRE(strcmp(sub19.data(), "us") == 0);

  REQUIRE_THROWS_AS(ss1.substr(6, 1), out_of_range);

  REQUIRE(ss1.len() == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss1.data(), s1) == 0);

  simple_string ss2(s2);

  simple_string sub21 = ss2.substr(1, 0);
  REQUIRE(sub21.len() == 0U);
  REQUIRE(strcmp(sub21.data(), "") == 0);

  simple_string sub22 = ss2.substr(1, 100);
  REQUIRE(sub22.len() == 5U);
  REQUIRE(strcmp(sub22.data(), "choes") == 0);

  simple_string sub23 = ss2.substr(4, simple_string::npos);
  REQUIRE(sub23.len() == 2U);
  REQUIRE(strcmp(sub23.data(), "es") == 0);

  simple_string sub24 = ss2.substr(3, 43125132);
  REQUIRE(sub24.len() == 3U);
  REQUIRE(strcmp(sub24.data(), "oes") == 0);

  simple_string sub25 = ss2.substr(0, simple_string::npos);
  REQUIRE(sub25.len() == 6U);
  REQUIRE(strcmp(sub25.data(), "Echoes") == 0);

  simple_string sub26 = ss2.substr(0, 7);
  REQUIRE(sub25.len() == 6U);
  REQUIRE(strcmp(sub26.data(), "Echoes") == 0);

  REQUIRE(ss2.len() == 6U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss2.data(), s2) == 0);

  simple_string ss3(s3);

  simple_string sub31 = ss3.substr(4, 43125132);
  REQUIRE(sub31.len() == 7U);
  REQUIRE(strcmp(sub31.data(), "bawamba") == 0);

  simple_string sub32 = ss3.substr(1, 10);
  REQUIRE(sub32.len() == 10U);
  REQUIRE(strcmp(sub32.data(), "humbawamba") == 0);

  simple_string sub33 = ss3.substr(0, simple_string::npos);
  REQUIRE(sub33.len() == 11U);
  REQUIRE(strcmp(sub33.data(), "Chumbawamba") == 0);

  simple_string sub34 = ss3.substr(10, 1);
  REQUIRE(sub34.len() == 1U);
  REQUIRE(strcmp(sub34.data(), "a") == 0);

  simple_string sub35 = ss3.substr(7, 2);
  REQUIRE(sub35.len() == 2U);
  REQUIRE(strcmp(sub35.data(), "am") == 0);

  REQUIRE(ss3.len() == 11U);
  REQUIRE(simple_string::npos == -1);
  REQUIRE(strcmp(ss3.data(), s3) == 0);

  REQUIRE(strcmp("Seamus", s1) == 0);
  REQUIRE(strcmp("Echoes", s2) == 0);
  REQUIRE(strcmp("Chumbawamba", s3) == 0);

  free(s1);
  free(s2);
  free(s3);
}

TEST_CASE("zero_arg_constructor", "[zero_arg_constructor]") {
  simple_string ss = simple_string();
  char* s1 = strdup("");
  REQUIRE(ss.len() == 0);
  REQUIRE(ss.data() != nullptr);
  REQUIRE(ss.data()[0] == '\0');
  REQUIRE(strcmp(ss.data(), s1) == 0);
  REQUIRE(simple_string::npos == -1);
  REQUIRE_THROWS_AS(ss.at(0), out_of_range);
  REQUIRE_THROWS_AS(ss.at(1), out_of_range);
  REQUIRE_THROWS_AS(ss.at(5), out_of_range);

  free(s1);
}
