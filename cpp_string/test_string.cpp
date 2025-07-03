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

TEST_CASE("basic find_first_of", "[find_first_of]") {
  char* s1 = strdup("attack!!");

  simple_string ss = simple_string(s1);
  REQUIRE(ss.len() == 8);
  REQUIRE(ss.data() != nullptr);
  REQUIRE(strcmp(ss.data(), s1) == 0);
  REQUIRE(ss.data()[8] == '\0');
  REQUIRE(simple_string::npos == -1);

  char* toFind1 = strdup("a");
  char* toFind2 = strdup("t");
  char* toFind3 = strdup("ck");
  char* toFind4 = strdup("!");
  char* toFind5 = strdup("!!kcatta");

  REQUIRE(ss.find_first_of(simple_string(toFind1), 0) == 0);
  REQUIRE(ss.find_first_of(simple_string(toFind1), 1) == 3);
  REQUIRE(ss.find_first_of(simple_string(toFind1), 5) == simple_string::npos);

  REQUIRE(ss.find_first_of(simple_string(toFind2), 0) == 1);
  REQUIRE(ss.find_first_of(simple_string(toFind2), 1) == 1);
  REQUIRE(ss.find_first_of(simple_string(toFind2), 2) == 2);
  REQUIRE(ss.find_first_of(simple_string(toFind2), 2) == 2);
  REQUIRE(ss.find_first_of(simple_string(toFind2), 3) == simple_string::npos);

  REQUIRE(ss.find_first_of(simple_string(toFind3), 0) == 4);
  REQUIRE(ss.find_first_of(simple_string(toFind3), 4) == 4);
  REQUIRE(ss.find_first_of(simple_string(toFind3), 5) == 5);
  REQUIRE(ss.find_first_of(simple_string(toFind3), 6) == simple_string::npos);
  REQUIRE(ss.find_first_of(simple_string(toFind3), 7) == simple_string::npos);

  REQUIRE(ss.find_first_of(simple_string(toFind4), 1) == 6);
  REQUIRE(ss.find_first_of(simple_string(toFind4), 3) == 6);
  REQUIRE(ss.find_first_of(simple_string(toFind4), 6) == 6);
  REQUIRE(ss.find_first_of(simple_string(toFind4), 7) == 7);
  REQUIRE_THROWS_AS(ss.find_first_of(simple_string(toFind4), 8), out_of_range);

  REQUIRE(ss.find_first_of(simple_string(toFind5), 0) == 0);
  REQUIRE(ss.find_first_of(simple_string(toFind5), 1) == 1);
  REQUIRE(ss.find_first_of(simple_string(toFind5), 3) == 3);
  REQUIRE(ss.find_first_of(simple_string(toFind5), 7) == 7);
  REQUIRE_THROWS_AS(ss.find_first_of(simple_string(toFind5), 8), out_of_range);

  free(s1);
  free(toFind1);
  free(toFind2);
  free(toFind3);
  free(toFind4);
  free(toFind5);
}

TEST_CASE("complex find_first_of", "[find_first_of]") {
  simple_string ss1 = simple_string();
  REQUIRE(ss1.len() == 0);
  REQUIRE(ss1.data() != nullptr);
  REQUIRE(strcmp(ss1.data(), "") == 0);
  REQUIRE(ss1.data()[0] == '\0');
  REQUIRE(simple_string::npos == -1);

  char* toFind1 = strdup("");
  char* toFind2 = strdup("abc");

  REQUIRE_THROWS_AS(ss1.find_first_of(simple_string(toFind1), 0), out_of_range);
  REQUIRE_THROWS_AS(ss1.find_first_of(simple_string(toFind1), 1), out_of_range);

  REQUIRE_THROWS_AS(ss1.find_first_of(simple_string(toFind2), 0), out_of_range);
  REQUIRE_THROWS_AS(ss1.find_first_of(simple_string(toFind2), 3), out_of_range);

  char* s1 = strdup("e@rth'$ w0rm$");
  simple_string ss2 = simple_string(s1);
  REQUIRE(ss2.len() == 13);
  REQUIRE(ss2.data() != nullptr);
  REQUIRE(strcmp(ss2.data(), s1) == 0);
  REQUIRE(simple_string::npos == -1);

  char* toFind3 = strdup(" ");
  char* toFind4 = strdup("$@");
  char* toFind5 = strdup("*%!-_=+");
  char* toFind6 = strdup("1234567890");

  REQUIRE(ss2.find_first_of(simple_string(toFind3), 0) == 7);
  REQUIRE(ss2.find_first_of(simple_string(toFind3), 5) == 7);
  REQUIRE(ss2.find_first_of(simple_string(toFind3), 7) == 7);
  REQUIRE(ss2.find_first_of(simple_string(toFind3), 10) == simple_string::npos);

  REQUIRE(ss2.find_first_of(simple_string(toFind4), 0) == 1);
  REQUIRE(ss2.find_first_of(simple_string(toFind4), 1) == 1);
  REQUIRE(ss2.find_first_of(simple_string(toFind4), 2) == 6);
  REQUIRE(ss2.find_first_of(simple_string(toFind4), 6) == 6);
  REQUIRE(ss2.find_first_of(simple_string(toFind4), 7) == 12);
  REQUIRE(ss2.find_first_of(simple_string(toFind4), 9) == 12);
  REQUIRE(ss2.find_first_of(simple_string(toFind4), 12) == 12);
  REQUIRE_THROWS_AS(ss2.find_first_of(simple_string(toFind4), 14),
                    out_of_range);

  REQUIRE(ss2.find_first_of(simple_string(toFind5), 0) == simple_string::npos);
  REQUIRE(ss2.find_first_of(simple_string(toFind5), 3) == simple_string::npos);
  REQUIRE(ss2.find_first_of(simple_string(toFind5), 6) == simple_string::npos);
  REQUIRE(ss2.find_first_of(simple_string(toFind5), 9) == simple_string::npos);

  REQUIRE(ss2.find_first_of(simple_string(toFind6), 0) == 9);
  REQUIRE(ss2.find_first_of(simple_string(toFind6), 4) == 9);
  REQUIRE(ss2.find_first_of(simple_string(toFind6), 7) == 9);
  REQUIRE(ss2.find_first_of(simple_string(toFind6), 9) == 9);
  REQUIRE(ss2.find_first_of(simple_string(toFind6), 12) == simple_string::npos);

  char* s2 = strdup("aaaaaaahhhhhhhhhh ah... ??  aha");
  simple_string ss3 = simple_string(s2);
  REQUIRE(ss3.len() == 31);
  REQUIRE(ss3.data() != nullptr);
  REQUIRE(strcmp(ss3.data(), s2) == 0);
  REQUIRE(simple_string::npos == -1);

  char* toFind7 = strdup("aaaaaaaaa");
  char* toFind8 = strdup("h");
  char* toFind9 = strdup("b onk");
  char* toFind10 = strdup("");

  REQUIRE(ss3.find_first_of(simple_string(toFind7), 0) == 0);
  REQUIRE(ss3.find_first_of(simple_string(toFind7), 5) == 5);
  REQUIRE(ss3.find_first_of(simple_string(toFind7), 6) == 6);
  REQUIRE(ss3.find_first_of(simple_string(toFind7), 12) == 18);
  REQUIRE(ss3.find_first_of(simple_string(toFind7), 18) == 18);
  REQUIRE(ss3.find_first_of(simple_string(toFind7), 19) == 28);
  REQUIRE(ss3.find_first_of(simple_string(toFind7), 26) == 28);
  REQUIRE(ss3.find_first_of(simple_string(toFind7), 28) == 28);
  REQUIRE(ss3.find_first_of(simple_string(toFind7), 29) == 30);
  REQUIRE(ss3.find_first_of(simple_string(toFind7), 30) == 30);
  REQUIRE_THROWS_AS(ss3.find_first_of(simple_string(toFind7), 31),
                    out_of_range);

  REQUIRE(ss3.find_first_of(simple_string(toFind8), 0) == 7);
  REQUIRE(ss3.find_first_of(simple_string(toFind8), 3) == 7);
  REQUIRE(ss3.find_first_of(simple_string(toFind8), 7) == 7);
  REQUIRE(ss3.find_first_of(simple_string(toFind8), 8) == 8);
  REQUIRE(ss3.find_first_of(simple_string(toFind8), 11) == 11);
  REQUIRE(ss3.find_first_of(simple_string(toFind8), 14) == 14);
  REQUIRE(ss3.find_first_of(simple_string(toFind8), 18) == 19);
  REQUIRE(ss3.find_first_of(simple_string(toFind8), 19) == 19);
  REQUIRE(ss3.find_first_of(simple_string(toFind8), 25) == 29);
  REQUIRE(ss3.find_first_of(simple_string(toFind8), 27) == 29);
  REQUIRE(ss3.find_first_of(simple_string(toFind8), 29) == 29);
  REQUIRE(ss3.find_first_of(simple_string(toFind8), 30) == simple_string::npos);

  REQUIRE(ss3.find_first_of(simple_string(toFind9), 0) == 17);
  REQUIRE(ss3.find_first_of(simple_string(toFind9), 17) == 17);
  REQUIRE(ss3.find_first_of(simple_string(toFind9), 19) == 23);
  REQUIRE(ss3.find_first_of(simple_string(toFind9), 23) == 23);
  REQUIRE(ss3.find_first_of(simple_string(toFind9), 25) == 26);
  REQUIRE(ss3.find_first_of(simple_string(toFind9), 26) == 26);
  REQUIRE(ss3.find_first_of(simple_string(toFind9), 27) == 27);
  REQUIRE(ss3.find_first_of(simple_string(toFind9), 28) == simple_string::npos);

  REQUIRE(ss3.find_first_of(simple_string(toFind10), 0) == simple_string::npos);
  REQUIRE(ss3.find_first_of(simple_string(toFind10), 25) ==
          simple_string::npos);
  REQUIRE_THROWS_AS(ss3.find_first_of(simple_string(toFind10), 31),
                    out_of_range);

  char* s3 = strdup("tabbies\tand spaces and\n new lines \t\t!\t!\n");
  simple_string ss4 = simple_string(s3);
  REQUIRE(ss4.len() == 40);
  REQUIRE(ss4.data() != nullptr);
  REQUIRE(strcmp(ss4.data(), s3) == 0);
  REQUIRE(simple_string::npos == -1);

  char* toFind11 = strdup(" \t\n");

  REQUIRE(ss4.find_first_of(simple_string(toFind11), 0) == 7);
  REQUIRE(ss4.find_first_of(simple_string(toFind11), 7) == 7);
  REQUIRE(ss4.find_first_of(simple_string(toFind11), 9) == 11);
  REQUIRE(ss4.find_first_of(simple_string(toFind11), 11) == 11);
  REQUIRE(ss4.find_first_of(simple_string(toFind11), 15) == 18);
  REQUIRE(ss4.find_first_of(simple_string(toFind11), 18) == 18);
  REQUIRE(ss4.find_first_of(simple_string(toFind11), 20) == 22);
  REQUIRE(ss4.find_first_of(simple_string(toFind11), 22) == 22);
  REQUIRE(ss4.find_first_of(simple_string(toFind11), 32) == 33);
  REQUIRE(ss4.find_first_of(simple_string(toFind11), 33) == 33);
  REQUIRE(ss4.find_first_of(simple_string(toFind11), 34) == 34);
  REQUIRE(ss4.find_first_of(simple_string(toFind11), 35) == 35);
  REQUIRE(ss4.find_first_of(simple_string(toFind11), 36) == 37);
  REQUIRE(ss4.find_first_of(simple_string(toFind11), 38) == 39);
  REQUIRE(ss4.find_first_of(simple_string(toFind11), 39) == 39);
  REQUIRE_THROWS_AS(ss4.find_first_of(simple_string(toFind10), 40),
                    out_of_range);

  char* s4 = strdup("CATERPILLAR");
  simple_string ss5 = simple_string(s4);
  REQUIRE(ss5.len() == 11);
  REQUIRE(ss5.data() != nullptr);
  REQUIRE(strcmp(ss5.data(), s4) == 0);
  REQUIRE(simple_string::npos == -1);

  char* toFind12 = strdup("caterpillar");
  char* toFind13 = strdup("RALLIPRETAC");
  char* toFind14 = strdup("cAt");

  REQUIRE(ss5.find_first_of(simple_string(toFind12), 0) == simple_string::npos);
  REQUIRE(ss5.find_first_of(simple_string(toFind12), 6) == simple_string::npos);
  REQUIRE(ss5.find_first_of(simple_string(toFind12), 10) ==
          simple_string::npos);

  REQUIRE(ss5.find_first_of(simple_string(toFind13), 0) == 0);
  REQUIRE(ss5.find_first_of(simple_string(toFind13), 1) == 1);
  REQUIRE(ss5.find_first_of(simple_string(toFind13), 3) == 3);
  REQUIRE(ss5.find_first_of(simple_string(toFind13), 5) == 5);
  REQUIRE(ss5.find_first_of(simple_string(toFind13), 8) == 8);
  REQUIRE(ss5.find_first_of(simple_string(toFind13), 9) == 9);
  REQUIRE(ss5.find_first_of(simple_string(toFind13), 10) == 10);

  REQUIRE(ss5.find_first_of(simple_string(toFind14), 0) == 1);
  REQUIRE(ss5.find_first_of(simple_string(toFind14), 1) == 1);
  REQUIRE(ss5.find_first_of(simple_string(toFind14), 2) == 9);
  REQUIRE(ss5.find_first_of(simple_string(toFind14), 4) == 9);
  REQUIRE(ss5.find_first_of(simple_string(toFind14), 6) == 9);
  REQUIRE(ss5.find_first_of(simple_string(toFind14), 9) == 9);
  REQUIRE(ss5.find_first_of(simple_string(toFind14), 10) ==
          simple_string::npos);

  free(s1);
  free(s2);
  free(s3);
  free(s4);
  free(toFind1);
  free(toFind2);
  free(toFind3);
  free(toFind4);
  free(toFind5);
  free(toFind6);
  free(toFind7);
  free(toFind8);
  free(toFind9);
  free(toFind10);
  free(toFind11);
  free(toFind12);
  free(toFind13);
  free(toFind14);
}

TEST_CASE("Basic split", "[split]") {
  
  simple_string str1("apple,banana,grape");
  simple_string delim1(",");
  auto result1 = str1.split(delim1);
  REQUIRE(result1.size() == 3);
  REQUIRE(result1[0] == simple_string("apple"));
  REQUIRE(result1[1] == simple_string("banana"));
  REQUIRE(result1[2] == simple_string("grape"));

  simple_string str2(",apple,banana,grape");
  simple_string delim2(",");
  auto result2 = str2.split(delim2);
  REQUIRE(result2.size() == 3);
  REQUIRE(result2[0] == simple_string("apple"));
  REQUIRE(result2[1] == simple_string("banana"));
  REQUIRE(result2[2] == simple_string("grape"));

  simple_string str3("apple,banana,grape,");
  simple_string delim3(",");
  auto result3 = str3.split(delim3);
  REQUIRE(result3.size() == 3);
  REQUIRE(result3[0] == simple_string("apple"));
  REQUIRE(result3[1] == simple_string("banana"));
  REQUIRE(result3[2] == simple_string("grape"));

  simple_string str4("apple");
  simple_string delim4(",");
  auto result4 = str4.split(delim4);
  REQUIRE(result4.size() == 1);
  REQUIRE(result4[0] == simple_string("apple"));

  simple_string str5("");
  simple_string delim5(",");
  auto result5 = str5.split(delim5);
  REQUIRE(result5.empty());

  simple_string str6("apple,,banana,,,grape");
  simple_string delim6(",");
  auto result6 = str6.split(delim6);
  REQUIRE(result6.size() == 3);
  REQUIRE(result6[0] == simple_string("apple"));
  REQUIRE(result6[1] == simple_string("banana"));
  REQUIRE(result6[2] == simple_string("grape"));

  simple_string str7(",");
  simple_string delim7(",");
  auto result7 = str7.split(delim7);
  REQUIRE(result7.empty());

  simple_string str8(",,,");
  simple_string delim8(",");
  auto result8 = str8.split(delim8);
  REQUIRE(result8.empty());

  simple_string str9(",apple,banana,grape,");
  simple_string delim9(",");
  auto result9 = str9.split(delim9);
  REQUIRE(result9.size() == 3);
  REQUIRE(result9[0] == simple_string("apple"));
  REQUIRE(result9[1] == simple_string("banana"));
  REQUIRE(result9[2] == simple_string("grape"));

  simple_string str10("singleword");
  simple_string delim10(",");
  auto result10 = str10.split(delim10);
  REQUIRE(result10.size() == 1);
  REQUIRE(result10[0] == simple_string("singleword"));

  simple_string str11("apple , banana , grape");
  simple_string delim11(",");
  auto result11 = str11.split(delim11);
  REQUIRE(result11.size() == 3);
  REQUIRE(result11[0] == simple_string("apple "));
  REQUIRE(result11[1] == simple_string(" banana "));
  REQUIRE(result11[2] == simple_string(" grape"));

  simple_string str12("   apple,banana,grape   ");
  simple_string delim12(",");
  auto result12 = str12.split(delim12);
  REQUIRE(result12.size() == 3);
  REQUIRE(result12[0] == simple_string("   apple"));
  REQUIRE(result12[1] == simple_string("banana"));
  REQUIRE(result12[2] == simple_string("grape   "));

  simple_string str13("apple,  banana,   grape");
  simple_string delim13(",");
  auto result13 = str13.split(delim13);
  REQUIRE(result13.size() == 3);
  REQUIRE(result13[0] == simple_string("apple"));
  REQUIRE(result13[1] == simple_string("  banana"));
  REQUIRE(result13[2] == simple_string("   grape"));

  std::string long_input(10000, 'A');
  simple_string str14(long_input.c_str());
  simple_string delim14(",");
  auto result14 = str14.split(delim14);
  REQUIRE(result14.size() == 1);
  REQUIRE(result14[0] == simple_string(long_input.c_str()));


  std::string long_input_mod(10000, 'A');
  for (size_t i = 999; i < 10000; i += 1000) {
      long_input_mod[i] = ',';
  }
  simple_string str15(long_input_mod.c_str());
  simple_string delim15(",");
  auto result15 = str15.split(delim15);
  REQUIRE(result15.size() == 10);
  for (size_t i = 0; i < 10; ++i) {
      REQUIRE(result15[i].len() == 999);
      REQUIRE(result15[i] == simple_string(std::string(999, 'A').c_str()));
  }
}

TEST_CASE("Complex split", "[split]") {

  simple_string str_empty_delim("abcdef");
  simple_string empty_delim("");
  auto result_empty_delim = str_empty_delim.split(empty_delim);
  REQUIRE(result_empty_delim.size() == 1);
  REQUIRE(result_empty_delim[0] == simple_string("abcdef"));

  simple_string str_repeated_delim("apple::banana:grape::orange");
  simple_string repeated_delim("::");
  auto result_repeated_delim = str_repeated_delim.split(repeated_delim);
  REQUIRE(result_repeated_delim.size() == 4);
  REQUIRE(result_repeated_delim[0] == simple_string("apple"));
  REQUIRE(result_repeated_delim[1] == simple_string("banana"));
  REQUIRE(result_repeated_delim[2] == simple_string("grape"));
  REQUIRE(result_repeated_delim[3] == simple_string("orange"));

  simple_string str_overlap("abcabc");
  simple_string delim_overlap("abc");
  auto result_overlap = str_overlap.split(delim_overlap);
  REQUIRE(result_overlap.size() == 0);

  simple_string str_all_delim(",,,");
  simple_string delim_all(",,,");
  auto result_all_delim = str_all_delim.split(delim_all);
  REQUIRE(result_all_delim.empty());

  simple_string str_single_char("a");
  simple_string delim_single_char("b");
  auto result_single_char = str_single_char.split(delim_single_char);
  REQUIRE(result_single_char.size() == 1);
  REQUIRE(result_single_char[0] == simple_string("a"));

  simple_string str_multi_basic("apple,banana|grape orange");
  simple_string multi_delims(",| ");
  auto result_multi_basic = str_multi_basic.split(multi_delims);
  REQUIRE(result_multi_basic.size() == 4);
  REQUIRE(result_multi_basic[0] == simple_string("apple"));
  REQUIRE(result_multi_basic[1] == simple_string("banana"));
  REQUIRE(result_multi_basic[2] == simple_string("grape"));
  REQUIRE(result_multi_basic[3] == simple_string("orange"));

  simple_string str_multi_extra("  apple, banana | grape , orange | peach  ");
  auto result_multi_extra = str_multi_extra.split(multi_delims);
  REQUIRE(result_multi_extra.size() == 5);
  REQUIRE(result_multi_extra[0] == simple_string("apple"));
  REQUIRE(result_multi_extra[1] == simple_string("banana"));
  REQUIRE(result_multi_extra[2] == simple_string("grape"));
  REQUIRE(result_multi_extra[3] == simple_string("orange"));
  REQUIRE(result_multi_extra[4] == simple_string("peach"));

  simple_string str_multi_front_end("| A , B | C ,| D |");
  auto result_multi_front_end = str_multi_front_end.split(multi_delims);
  REQUIRE(result_multi_front_end.size() == 4);
  REQUIRE(result_multi_front_end[0] == simple_string("A"));
  REQUIRE(result_multi_front_end[1] == simple_string("B"));
  REQUIRE(result_multi_front_end[2] == simple_string("C"));
  REQUIRE(result_multi_front_end[3] == simple_string("D"));

  simple_string str_multi_spaces("apple  ,  banana |  grape , orange  |  peach");
  // Note that here we explicitly pass ",|" as the delimiter string
  auto result_multi_spaces = str_multi_spaces.split(",|");
  REQUIRE(result_multi_spaces.size() == 5);
  REQUIRE(result_multi_spaces[0] == simple_string("apple  "));
  REQUIRE(result_multi_spaces[1] == simple_string("  banana "));
  REQUIRE(result_multi_spaces[2] == simple_string("  grape "));
  REQUIRE(result_multi_spaces[3] == simple_string(" orange  "));
  REQUIRE(result_multi_spaces[4] == simple_string("  peach"));

  simple_string str_every_char("|,| |,|| , ");
  auto result_every_char = str_every_char.split(multi_delims);
  REQUIRE(result_every_char.empty());

  std::string long_input_multi(5000, 'A');
  long_input_multi[1000] = ',';
  long_input_multi[2000] = '|';
  long_input_multi[3000] = ' ';
  long_input_multi[4000] = ',';
  simple_string str_multi_intervals(long_input_multi.c_str());
  auto result_multi_intervals = str_multi_intervals.split(multi_delims);
  REQUIRE(result_multi_intervals.size() == 5);
  REQUIRE(result_multi_intervals[0].len() == 1000);
  REQUIRE(result_multi_intervals[1].len() == 999);
  REQUIRE(result_multi_intervals[2].len() == 999);
  REQUIRE(result_multi_intervals[3].len() == 999);
  REQUIRE(result_multi_intervals[4].len() == 999);

  simple_string str_extreme1(",,,|||   ,,,apple,,,||  banana  ,,,,|grape,, ||| orange   ");
  auto result_extreme1 = str_extreme1.split(multi_delims);
  REQUIRE(result_extreme1.size() == 4);
  REQUIRE(result_extreme1[0] == simple_string("apple"));
  REQUIRE(result_extreme1[1] == simple_string("banana"));
  REQUIRE(result_extreme1[2] == simple_string("grape"));
  REQUIRE(result_extreme1[3] == simple_string("orange"));

  simple_string str_extreme2(",a|b,c d,e f|g,h | i | j , k |");
  auto result_extreme2 = str_extreme2.split(multi_delims);
  REQUIRE(result_extreme2.size() == 11);
  REQUIRE(result_extreme2[0] == simple_string("a"));
  REQUIRE(result_extreme2[1] == simple_string("b"));
  REQUIRE(result_extreme2[2] == simple_string("c"));
  REQUIRE(result_extreme2[3] == simple_string("d"));
  REQUIRE(result_extreme2[4] == simple_string("e"));
  REQUIRE(result_extreme2[5] == simple_string("f"));
  REQUIRE(result_extreme2[6] == simple_string("g"));
  REQUIRE(result_extreme2[7] == simple_string("h"));
  REQUIRE(result_extreme2[8] == simple_string("i"));
  REQUIRE(result_extreme2[9] == simple_string("j"));
  REQUIRE(result_extreme2[10] == simple_string("k"));

  std::string crazy_long(10000, 'A');
  for (size_t i = 2; i < 10000; i += 3) {
    crazy_long[i] = (i % 2 == 0) ? ',' : '|';
  }
  simple_string str_extreme3(crazy_long.c_str());
  auto result_extreme3 = str_extreme3.split(multi_delims);
  REQUIRE(result_extreme3[0].len() == 2);
  REQUIRE(result_extreme3[1].len() == 2);
  REQUIRE(result_extreme3[100].len() == 2);
  REQUIRE(result_extreme3.back().len() == 1);

  simple_string str_extreme4(",,,,,,,||||||       ||||||,,,,,,||||||");
  auto result_extreme4 = str_extreme4.split(multi_delims);
  REQUIRE(result_extreme4.empty());

  simple_string str_extreme5(",| |,|");
  auto result_extreme5 = str_extreme5.split(multi_delims);
  REQUIRE(result_extreme5.empty());

  simple_string str_spaces_only("apple     banana            grape            orange            ");
  simple_string space_delim(" ");
  auto result_spaces_only = str_spaces_only.split(space_delim);
  REQUIRE(result_spaces_only.size() == 4);
  REQUIRE(result_spaces_only[0] == simple_string("apple"));
  REQUIRE(result_spaces_only[1] == simple_string("banana"));
  REQUIRE(result_spaces_only[2] == simple_string("grape"));
  REQUIRE(result_spaces_only[3] == simple_string("orange"));

  simple_string str_tabs_newlines("\t  apple\t\tbanana\n\n\n  grape\t\t orange \n\npear\t\n\tmelon\n");
  simple_string tab_newline_delim("\t\n");
  auto result_tabs_newlines = str_tabs_newlines.split(tab_newline_delim);
  REQUIRE(result_tabs_newlines.size() == 6);
  REQUIRE(result_tabs_newlines[0] == simple_string("  apple"));
  REQUIRE(result_tabs_newlines[1] == simple_string("banana"));
  REQUIRE(result_tabs_newlines[2] == simple_string("  grape"));
  REQUIRE(result_tabs_newlines[3] == simple_string(" orange "));
  REQUIRE(result_tabs_newlines[4] == simple_string("pear"));
  REQUIRE(result_tabs_newlines[5] == simple_string("melon"));

  simple_string str_alt_letters("A,B|C D,E|F G,H|I");
  auto result_alt_letters = str_alt_letters.split(multi_delims);
  REQUIRE(result_alt_letters.size() == 9);
  REQUIRE(result_alt_letters[0] == simple_string("A"));
  REQUIRE(result_alt_letters[1] == simple_string("B"));
  REQUIRE(result_alt_letters[2] == simple_string("C"));
  REQUIRE(result_alt_letters[3] == simple_string("D"));
  REQUIRE(result_alt_letters[4] == simple_string("E"));
  REQUIRE(result_alt_letters[5] == simple_string("F"));
  REQUIRE(result_alt_letters[6] == simple_string("G"));
  REQUIRE(result_alt_letters[7] == simple_string("H"));
  REQUIRE(result_alt_letters[8] == simple_string("I"));

  simple_string str_long_multi("apple,,,|banana|||grape,,,||orange,|peach");
  auto result_long_multi = str_long_multi.split(multi_delims);
  REQUIRE(result_long_multi.size() == 5);
  REQUIRE(result_long_multi[0] == simple_string("apple"));
  REQUIRE(result_long_multi[1] == simple_string("banana"));
  REQUIRE(result_long_multi[2] == simple_string("grape"));
  REQUIRE(result_long_multi[3] == simple_string("orange"));
  REQUIRE(result_long_multi[4] == simple_string("peach"));

  simple_string str_crazy("apple  ,| banana\t\t,| grape  ,\n orange | peach");
  auto result_crazy = str_crazy.split(multi_delims);
  REQUIRE(result_crazy.size() == 6);
  REQUIRE(result_crazy[0] == simple_string("apple"));
  REQUIRE(result_crazy[1] == simple_string("banana\t\t"));
  REQUIRE(result_crazy[2] == simple_string("grape"));
  REQUIRE(result_crazy[3] == simple_string("\n"));
  REQUIRE(result_crazy[4] == simple_string("orange"));
  REQUIRE(result_crazy[5] == simple_string("peach"));

  std::string large_random(5000, 'B');
  for (size_t i = 500; i < 5000; i += 500) {
    large_random[i] = ',';
  }
  simple_string str_rand(large_random.c_str());
  auto result_rand = str_rand.split(multi_delims);
  REQUIRE(result_rand.size() == 10);
  REQUIRE(result_rand[0].len() == 500);
  REQUIRE(result_rand[1].len() == 499);
  REQUIRE(result_rand.back().len() == 499);

  std::string large_pattern;
  char current_char = 'A';
  for (size_t i = 0; i < 10000; ++i) {
    if (i % 250 == 0 && i != 0) {
      char delimiter = (i % 4 == 0) ? ',' : (i % 4 == 1) ? '|' 
                       : (i % 4 == 2) ? ' ' : '\n';
      large_pattern += delimiter;
      current_char = 'A' + ((current_char - 'A' + 1) % 26);
    } else {
      large_pattern += current_char;
    }
  }
  simple_string large_str(large_pattern.c_str());
  auto result_large_str = large_str.split(multi_delims);
  REQUIRE(result_large_str.size() == 40);
  for (size_t i = 0; i < result_large_str.size(); ++i) {
    if (i == 0) {
      REQUIRE(result_large_str[i].len() == 250);
    } else {
      REQUIRE(result_large_str[i].len() == 249);
    }
    REQUIRE(result_large_str[i].data()[0] == char('A' + (i % 26)));
  }
}

TEST_CASE("Bounds Checking", "[at][substr]") {

  simple_string str(simple_string("hello"));
  REQUIRE_THROWS_AS(str.at(5), std::out_of_range); 
  REQUIRE_THROWS_AS(str.at(10), std::out_of_range);
  REQUIRE_THROWS_AS(str.at(-1), std::out_of_range);
  REQUIRE_THROWS_AS(str[5], std::out_of_range);
  REQUIRE_THROWS_AS(str[10], std::out_of_range);
  REQUIRE_THROWS_AS(str[-1], std::out_of_range);
  REQUIRE_THROWS_AS(str.substr(5, 2), std::out_of_range);
  REQUIRE_THROWS_AS(str.substr(-1, 2), std::out_of_range);
  REQUIRE_THROWS_AS(str.substr(100, 5), std::out_of_range);
  REQUIRE(str.at(0) == 'h');
  REQUIRE(str.at(1) == 'e');
  REQUIRE(str.at(2) == 'l');
  REQUIRE(str.at(3) == 'l');
  REQUIRE(str.at(4) == 'o');
  REQUIRE(str[0] == 'h');
  REQUIRE(str[1] == 'e');
  REQUIRE(str[2] == 'l');
  REQUIRE(str[3] == 'l');
  REQUIRE(str[4] == 'o');
  REQUIRE(str.substr(0, 2) == simple_string("he")); 
  REQUIRE(str.substr(1, 3) == simple_string("ell"));
  REQUIRE(str.substr(3, 2) == simple_string("lo")); 
  REQUIRE(str.substr(4, 1) == simple_string("o")); 
  REQUIRE(str.substr(0, simple_string::npos) == simple_string("hello"));

  simple_string empty_str("");
  REQUIRE_THROWS_AS(empty_str.at(0), std::out_of_range);
  REQUIRE_THROWS_AS(empty_str[0], std::out_of_range);
  REQUIRE_THROWS_AS(empty_str.substr(0, 1), std::out_of_range);
  REQUIRE_THROWS_AS(empty_str.at(2), std::out_of_range);
  REQUIRE_THROWS_AS(empty_str[2], std::out_of_range);
  REQUIRE_THROWS_AS(empty_str.substr(2, 1), std::out_of_range);
  REQUIRE_THROWS_AS(empty_str.at(-1), std::out_of_range);
  REQUIRE_THROWS_AS(empty_str[-1], std::out_of_range);
  REQUIRE_THROWS_AS(empty_str.substr(-1, 1), std::out_of_range);

  simple_string empty_str_zero_arg;
  REQUIRE_THROWS_AS(empty_str_zero_arg.at(0), std::out_of_range);
  REQUIRE_THROWS_AS(empty_str_zero_arg[0], std::out_of_range);
  REQUIRE_THROWS_AS(empty_str_zero_arg.substr(0, 1), std::out_of_range);
  REQUIRE_THROWS_AS(empty_str_zero_arg.at(2), std::out_of_range);
  REQUIRE_THROWS_AS(empty_str_zero_arg[2], std::out_of_range);
  REQUIRE_THROWS_AS(empty_str_zero_arg.substr(2, 1), std::out_of_range);
  REQUIRE_THROWS_AS(empty_str_zero_arg.at(-1), std::out_of_range);
  REQUIRE_THROWS_AS(empty_str_zero_arg[-1], std::out_of_range);
  REQUIRE_THROWS_AS(empty_str_zero_arg.substr(-1, 1), std::out_of_range);

  simple_string phrase("hello world!");
  REQUIRE(phrase.at(0) == 'h');
  REQUIRE(phrase.at(6) == 'w');
  REQUIRE(phrase[11] == '!');
  REQUIRE(phrase.substr(0, 5) == simple_string("hello"));
  REQUIRE(phrase.substr(6, 5) == simple_string("world"));
  REQUIRE(phrase.substr(6, simple_string::npos) == simple_string("world!"));
  REQUIRE_THROWS_AS(phrase.at(12), std::out_of_range);
  REQUIRE_THROWS_AS(phrase[12], std::out_of_range);
  REQUIRE_THROWS_AS(phrase.substr(12, 1), std::out_of_range);
  REQUIRE_THROWS_AS(phrase.at(100), std::out_of_range);
  REQUIRE_THROWS_AS(phrase[100], std::out_of_range);
  REQUIRE_THROWS_AS(phrase.substr(100, 1), std::out_of_range);
    REQUIRE_THROWS_AS(phrase.at(-2), std::out_of_range);
  REQUIRE_THROWS_AS(phrase[-2], std::out_of_range);
  REQUIRE_THROWS_AS(phrase.substr(-2, 1), std::out_of_range);

  simple_string spaced("  leading and trailing  ");
  REQUIRE(spaced.substr(0, 2) == simple_string("  "));
  REQUIRE(spaced.substr(2, 7) == simple_string("leading"));
  REQUIRE(spaced.substr(10, 3) == simple_string("and"));
  REQUIRE(spaced.substr(16, 8) == simple_string("ailing  "));
  REQUIRE(spaced.substr(19, 2) == simple_string("in"));
  REQUIRE_THROWS_AS(spaced.at(24), std::out_of_range);
  REQUIRE_THROWS_AS(spaced[24], std::out_of_range);
  REQUIRE_THROWS_AS(spaced.substr(24, 1), std::out_of_range);
  REQUIRE_THROWS_AS(spaced.at(-1), std::out_of_range);
  REQUIRE_THROWS_AS(spaced[-1], std::out_of_range);
  REQUIRE_THROWS_AS(spaced.substr(-1, 1), std::out_of_range);
  REQUIRE_THROWS_AS(spaced.at(200), std::out_of_range);
  REQUIRE_THROWS_AS(spaced[200], std::out_of_range);
  REQUIRE_THROWS_AS(spaced.substr(200, 1), std::out_of_range);

  simple_string special_chars("!@#$%^&*()_+");
  REQUIRE(special_chars.at(0) == '!');
  REQUIRE(special_chars.at(5) == '^');
  REQUIRE(special_chars.substr(0, 3) == simple_string("!@#"));
  REQUIRE(special_chars.substr(4, 4) == simple_string("%^&*"));
  REQUIRE_THROWS_AS(special_chars.at(12), std::out_of_range);
  REQUIRE_THROWS_AS(special_chars[12], std::out_of_range);
  REQUIRE_THROWS_AS(special_chars.substr(12, 1), std::out_of_range);
  REQUIRE_THROWS_AS(special_chars.at(-1), std::out_of_range);
  REQUIRE_THROWS_AS(special_chars[-1], std::out_of_range);
  REQUIRE_THROWS_AS(special_chars.substr(-1, 1), std::out_of_range);
  REQUIRE_THROWS_AS(special_chars.at(100), std::out_of_range);
  REQUIRE_THROWS_AS(special_chars[100], std::out_of_range);
  REQUIRE_THROWS_AS(special_chars.substr(100, 1), std::out_of_range);

  simple_string source_str("ABC");
  simple_string copy_str(source_str);
  REQUIRE(copy_str.at(0) == 'A');
  REQUIRE(copy_str.at(1) == 'B');
  REQUIRE(copy_str.at(2) == 'C');
  REQUIRE_THROWS_AS(copy_str.at(3), std::out_of_range);
  REQUIRE_THROWS_AS(copy_str[3], std::out_of_range);
  REQUIRE_THROWS_AS(copy_str.substr(3, 1), std::out_of_range);
  REQUIRE_THROWS_AS(copy_str.at(10), std::out_of_range);
  REQUIRE_THROWS_AS(copy_str[10], std::out_of_range);
  REQUIRE_THROWS_AS(copy_str.substr(10, 1), std::out_of_range);
  REQUIRE_THROWS_AS(copy_str.at(-1), std::out_of_range);
  REQUIRE_THROWS_AS(copy_str[-1], std::out_of_range);
  REQUIRE_THROWS_AS(copy_str.substr(-1, 1), std::out_of_range);
  
  std::string long_string(10000, 'A');
  simple_string long_str(long_string.data());
  REQUIRE(long_str.len() == 10000);
  REQUIRE(long_str.at(0) == 'A');
  REQUIRE(long_str.at(9999) == 'A');
  REQUIRE(long_str[0] == 'A');
  REQUIRE(long_str[9999] == 'A');
  REQUIRE(long_str.substr(0, 5) == simple_string("AAAAA"));  
  REQUIRE(long_str.substr(9995, 5) == simple_string("AAAAA"));
  REQUIRE(long_str.substr(5000, 10) == simple_string("AAAAAAAAAA")); 
  REQUIRE(long_str.substr(0, simple_string::npos) == long_str);
  REQUIRE_THROWS_AS(long_str.at(10000), std::out_of_range);
  REQUIRE_THROWS_AS(long_str[10000], std::out_of_range);
  REQUIRE_THROWS_AS(long_str.substr(10000, 1), std::out_of_range);
  REQUIRE_THROWS_AS(long_str.substr(15000, 2), std::out_of_range);
  REQUIRE_THROWS_AS(long_str.at(-10), std::out_of_range);
  REQUIRE_THROWS_AS(long_str[-10], std::out_of_range);
  REQUIRE_THROWS_AS(long_str.substr(-10, 1), std::out_of_range);
  REQUIRE_THROWS_AS(long_str.substr(-20000, 2), std::out_of_range);
  REQUIRE_THROWS_AS(long_str.at(20000), std::out_of_range);
  REQUIRE_THROWS_AS(long_str[20000], std::out_of_range);
  REQUIRE_THROWS_AS(long_str.substr(20000, 1), std::out_of_range);
  REQUIRE_THROWS_AS(long_str.substr(30000, 2), std::out_of_range);
}

