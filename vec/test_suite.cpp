#define CATCH_CONFIG_MAIN
#include "./catch.hpp"


#include <cstring>  // Does not do our cstring.h this includes C's <string.h>
#include <cstdlib>  // include's C's <stdlib.h>
#include <iostream> // for C++ printing

#include "./vec.hpp"

#include <stdexcept>

using namespace std;

TEST_CASE("basic insert", "[insert]") {
  vec v;
  size_t index;
  
  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);

  index = v.insert(0, "Egg sandwich./n");

  REQUIRE(index == 0);
  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 1);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "Egg sandwich./n");

  index = v.insert(1, "bad squirrel");

  REQUIRE(index == 1);
  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 2);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "Egg sandwich./n");
  REQUIRE(v.data()[1] == "bad squirrel");

  index = v.insert(0, "okay");

  REQUIRE(index == 0);
  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 3);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "okay");
  REQUIRE(v.data()[1] == "Egg sandwich./n");
  REQUIRE(v.data()[2] == "bad squirrel");

  vec v2;
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 0);
  REQUIRE(v2.data() != nullptr);

  v2.push_back("chestnut");
  v2.push_back("walnut");
  v2.push_back("peanut");
  v2.push_back("applenut");

  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 4);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "chestnut");
  REQUIRE(v2.data()[1] == "walnut");
  REQUIRE(v2.data()[2] == "peanut");
  REQUIRE(v2.data()[3] == "applenut");

  index = v2.insert(3, "hazelnut");
  REQUIRE(index == 3);
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 5);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "chestnut");
  REQUIRE(v2.data()[1] == "walnut");
  REQUIRE(v2.data()[2] == "peanut");
  REQUIRE(v2.data()[3] == "hazelnut");
  REQUIRE(v2.data()[4] == "applenut");

  index = v2.insert(3, "candlenut");
  REQUIRE(index == 3);
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 6);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "chestnut");
  REQUIRE(v2.data()[1] == "walnut");
  REQUIRE(v2.data()[2] == "peanut");
  REQUIRE(v2.data()[3] == "candlenut");
  REQUIRE(v2.data()[4] == "hazelnut");
  REQUIRE(v2.data()[5] == "applenut");

  index = v2.insert(0, "coconut");
  REQUIRE(index == 0);
  index = v2.insert(0, "donut");
  REQUIRE(index == 0);
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 8);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "donut");
  REQUIRE(v2.data()[1] == "coconut");
  REQUIRE(v2.data()[2] == "chestnut");
  REQUIRE(v2.data()[3] == "walnut");
  REQUIRE(v2.data()[4] == "peanut");
  REQUIRE(v2.data()[5] == "candlenut");
  REQUIRE(v2.data()[6] == "hazelnut");
  REQUIRE(v2.data()[7] == "applenut");
  

  index = v2.insert(8, "doughnut");
  REQUIRE(index == 8);
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 9);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "donut");
  REQUIRE(v2.data()[1] == "coconut");
  REQUIRE(v2.data()[2] == "chestnut");
  REQUIRE(v2.data()[3] == "walnut");
  REQUIRE(v2.data()[4] == "peanut");
  REQUIRE(v2.data()[5] == "candlenut");
  REQUIRE(v2.data()[6] == "hazelnut");
  REQUIRE(v2.data()[7] == "applenut");
  REQUIRE(v2.data()[8] == "doughnut");

  vec v3;
  REQUIRE(v3.capacity() == 10);
  REQUIRE(v3.length() == 0);
  REQUIRE(v3.data() != nullptr);

  v3.reserve(5);
  REQUIRE(v3.capacity() == 10);
  REQUIRE(v3.length() == 0);

  v3.reserve(32);
  REQUIRE(v3.capacity() == 32);
  REQUIRE(v3.length() == 0);

  v3.reserve(65);
  REQUIRE(v3.capacity() == 65);
  REQUIRE(v3.length() == 0);

  v3.reserve(1044);
  REQUIRE(v3.capacity() == 1044);
  REQUIRE(v3.length() == 0);

  v3.reserve(3000);
  REQUIRE(v3.capacity() == 3000);
  REQUIRE(v3.length() == 0);

  for (int i = 0; i < 3001; ++i) {
    v3.push_back("thing " + std::to_string(i));
  }
  REQUIRE(v3.capacity() == 6000);
  REQUIRE(v3.length() == 3001);
  REQUIRE(v3.data() != nullptr);
  for (int i = 0; i < 3001; ++i) {
    REQUIRE(v3.data()[i] == "thing " + std::to_string(i));
  }

}


TEST_CASE("complex insert", "[insert]") {
  vec v;
  size_t index;

  for (int i = 0; i < 9; ++i) {
    v.push_back("count " + std::to_string(i));
  }

  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 9);
  REQUIRE(v.data() != nullptr);
  for (int i = 0; i < 9; ++i) {
    REQUIRE(v.data()[i] == "count " + std::to_string(i));
  }

  index = v.insert(9, "count 9");
  REQUIRE(index == 9);
  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 10);
  REQUIRE(v.data() != nullptr);
  for (int i = 0; i < 10; ++i) {
    REQUIRE(v.data()[i] == "count " + std::to_string(i));
  }
  
  index = v.insert(10, "count 10");
  REQUIRE(index == 10);
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 11);
  REQUIRE(v.data() != nullptr);
  for (int i = 0; i < 11; ++i) {
    REQUIRE(v.data()[i] =="count " + std::to_string(i));
  }

  REQUIRE_THROWS_AS(v.insert(12, "12 out of bounds"), out_of_range);
  REQUIRE_THROWS_AS(v.insert(21, "21 out of bounds"), out_of_range);
  REQUIRE_THROWS_AS(v.insert(24, "24 out of bounds"), out_of_range);
  REQUIRE_THROWS_AS(v.insert(2824, "2824 out of bounds"), out_of_range);

  
  vec v2(1);
  REQUIRE(v2.capacity() == 1);
  REQUIRE(v2.length() == 0);
  REQUIRE(v2.data() != nullptr);

  for (int i = 0; i < 1000; ++i) {
    index = v2.insert(i, "boom boom " + std::to_string(i));
    REQUIRE(index == i);
  }

  REQUIRE(v2.capacity() == 1024);
  REQUIRE(v2.length() == 1000);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "boom boom 0");
  REQUIRE(v2.data()[12] == "boom boom 12");
  REQUIRE(v2.data()[419] == "boom boom 419");
  REQUIRE(v2.data()[732] == "boom boom 732");
  REQUIRE(v2.data()[999] == "boom boom 999");


  vec v3(13);
  REQUIRE(v3.capacity() == 13);
  REQUIRE(v3.length() == 0);
  REQUIRE(v3.data() != nullptr);

  for (int i = 0; i < 13; ++i) {
    index = v3.insert(0, "choom choom " + std::to_string(i));
    REQUIRE(index == 0); 
  } 
  REQUIRE(v3.capacity() == 13);
  REQUIRE(v3.length() == 13);

  v3.insert(0, "choom choom 13");
  REQUIRE(v3.capacity() == 26);
  REQUIRE(v3.length() == 14);

  for (int i = 14; i < 571; ++i) {
    index = v3.insert(0, "choom choom " + std::to_string(i));
    REQUIRE(index == 0); 
  }
  REQUIRE(v3.capacity() == 832);
  REQUIRE(v3.length() == 571);
  REQUIRE(v3.data() != nullptr);

  int val = 570;
  for (int i = 0; i < 571; ++i) {
    REQUIRE(v3.data()[i] == "choom choom " + std::to_string(val));
    val--;
  }

  vec v4(1);
  REQUIRE(v4.capacity() == 1);
  REQUIRE(v4.length() == 0);
  REQUIRE(v4.data() != nullptr);

  index = v4.insert(0, "elem 0");
  REQUIRE(v4.capacity() == 1);
  REQUIRE(v4.length() == 1);

  index = v4.insert(1, "elem 1");
  REQUIRE(v4.capacity() == 2);
  REQUIRE(v4.length() == 2);

  v4.insert(2, "elem 2");
  REQUIRE(v4.capacity() == 4);
  REQUIRE(v4.length() == 3);

  v4.insert(3, "elem 3");
  v4.insert(4, "elem 4");
  REQUIRE(v4.capacity() == 8);
  REQUIRE(v4.length() == 5);
  
  v4.insert(5, "elem 5");
  v4.insert(6, "elem 6");
  v4.insert(7, "elem 7");
  v4.insert(8, "elem 8");
  REQUIRE(v4.capacity() == 16);
  REQUIRE(v4.length() == 9);
  for (int i = 0; i < 9; ++i) {
    REQUIRE(v4.data()[i] == "elem " + std::to_string(i));
  }
  REQUIRE_THROWS_AS(v4.insert(10, "10 out of bounds"), out_of_range);
  REQUIRE_THROWS_AS(v4.insert(14, "14 out of bounds"), out_of_range);

  
  vec v5(5);
  REQUIRE(v5.insert(0, "num 0") == 0);
  REQUIRE(v5.insert(1, "num 1") == 1);
  REQUIRE(v5.insert(2, "num 3") == 2); 
  REQUIRE(v5.insert(3, "num 5") == 3);
  REQUIRE(v5.insert(4, "num 8") == 4);

  REQUIRE(v5.capacity() == 5);
  REQUIRE(v5.length() == 5);

  REQUIRE(v5.insert(3, "num 4") == 3);
  REQUIRE(v5.insert(5, "num 7") == 5);
  REQUIRE(v5.insert(5, "num 6") == 5); 
  REQUIRE(v5.insert(2, "num 2") == 2);

  REQUIRE(v5.capacity() == 10);
  REQUIRE(v5.length() == 9);
  for (int i = 0; i < 9; ++i) {
    REQUIRE(v5.data()[i] == "num " + std::to_string(i));
  }

  REQUIRE(v5.insert(9, "num 12") == 9);
  REQUIRE(v5.insert(9, "num 9") == 9);
  REQUIRE(v5.insert(10, "num 11") == 10); 
  REQUIRE(v5.insert(10, "num 10") == 10);
  REQUIRE(v5.insert(0, "num -2") == 0); 
  REQUIRE(v5.insert(1, "num -1") == 1);

  REQUIRE(v5.capacity() == 20);
  REQUIRE(v5.length() == 15);

  for (int i = 0; i < 15; ++i) {
    REQUIRE(v5.data()[i] == "num " + std::to_string(i - 2));
  }
}
TEST_CASE("basic ctor", "[ctor]") {
  vec v;

  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);
  
  vec v2(5);
  REQUIRE(v2.capacity() == 5);
  REQUIRE(v2.length() == 0);
  REQUIRE(v2.data() != nullptr);
}

TEST_CASE("basic push", "[push_back]") {
  vec v;

  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);

  v.push_back("testing");
  v.push_back("omelas");
  v.push_back("cookie");

  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 3);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "testing");
  REQUIRE(v.data()[1] == "omelas");
  REQUIRE(v.data()[2] == "cookie");
}

TEST_CASE("complex push", "[push_back]") {
  // basic push:
  vec v;

  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);

  v.push_back("testing");
  v.push_back("omelas");
  v.push_back("cookie");

  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 3);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "testing");
  REQUIRE(v.data()[1] == "omelas");
  REQUIRE(v.data()[2] == "cookie");

  // additional push tests:
  v.push_back("hello");
  v.push_back("world");
  v.push_back("CIT");
  v.push_back("5950");
  v.push_back("Computer");
  v.push_back("Systems");
  v.push_back("Programming");

  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 10);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "testing");
  REQUIRE(v.data()[1] == "omelas");
  REQUIRE(v.data()[2] == "cookie");
  REQUIRE(v.data()[3] == "hello");
  REQUIRE(v.data()[4] == "world");
  REQUIRE(v.data()[5] == "CIT");
  REQUIRE(v.data()[6] == "5950");
  REQUIRE(v.data()[7] == "Computer");
  REQUIRE(v.data()[8] == "Systems");
  REQUIRE(v.data()[9] == "Programming");

  v.push_back("testing");

  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 11);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "testing");
  REQUIRE(v.data()[1] == "omelas");
  REQUIRE(v.data()[2] == "cookie");
  REQUIRE(v.data()[3] == "hello");
  REQUIRE(v.data()[4] == "world");
  REQUIRE(v.data()[5] == "CIT");
  REQUIRE(v.data()[6] == "5950");
  REQUIRE(v.data()[7] == "Computer");
  REQUIRE(v.data()[8] == "Systems");
  REQUIRE(v.data()[9] == "Programming");
  REQUIRE(v.data()[10] == "testing");

  v.push_back("testing");
  v.push_back("adding");
  v.push_back("elements");
  v.push_back("to");
  v.push_back("the");
  v.push_back("end");
  v.push_back("of");
  v.push_back("a");
  v.push_back("vector");

  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 20);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "testing");
  REQUIRE(v.data()[1] == "omelas");
  REQUIRE(v.data()[2] == "cookie");
  REQUIRE(v.data()[3] == "hello");
  REQUIRE(v.data()[4] == "world");
  REQUIRE(v.data()[5] == "CIT");
  REQUIRE(v.data()[6] == "5950");
  REQUIRE(v.data()[7] == "Computer");
  REQUIRE(v.data()[8] == "Systems");
  REQUIRE(v.data()[9] == "Programming");
  REQUIRE(v.data()[10] == "testing");
  REQUIRE(v.data()[11] == "testing");
  REQUIRE(v.data()[12] == "adding");
  REQUIRE(v.data()[13] == "elements");
  REQUIRE(v.data()[14] == "to");
  REQUIRE(v.data()[15] == "the");
  REQUIRE(v.data()[16] == "end");
  REQUIRE(v.data()[17] == "of");
  REQUIRE(v.data()[18] == "a");
  REQUIRE(v.data()[19] == "vector");

  v.push_back("object");

  REQUIRE(v.capacity() == 40);
  REQUIRE(v.length() == 21);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "testing");
  REQUIRE(v.data()[1] == "omelas");
  REQUIRE(v.data()[2] == "cookie");
  REQUIRE(v.data()[3] == "hello");
  REQUIRE(v.data()[4] == "world");
  REQUIRE(v.data()[5] == "CIT");
  REQUIRE(v.data()[6] == "5950");
  REQUIRE(v.data()[7] == "Computer");
  REQUIRE(v.data()[8] == "Systems");
  REQUIRE(v.data()[9] == "Programming");
  REQUIRE(v.data()[10] == "testing");
  REQUIRE(v.data()[11] == "testing");
  REQUIRE(v.data()[12] == "adding");
  REQUIRE(v.data()[13] == "elements");
  REQUIRE(v.data()[14] == "to");
  REQUIRE(v.data()[15] == "the");
  REQUIRE(v.data()[16] == "end");
  REQUIRE(v.data()[17] == "of");
  REQUIRE(v.data()[18] == "a");
  REQUIRE(v.data()[19] == "vector");
  REQUIRE(v.data()[20] == "object");

  vec v2(5);
  REQUIRE(v2.capacity() == 5);
  REQUIRE(v2.length() == 0);
  REQUIRE(v2.data() != nullptr);

  v2.push_back("testing");
  v2.push_back("omelas");
  v2.push_back("cookie");

  REQUIRE(v2.capacity() == 5);
  REQUIRE(v2.length() == 3);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "testing");
  REQUIRE(v2.data()[1] == "omelas");
  REQUIRE(v2.data()[2] == "cookie");

  v2.push_back("hello");
  v2.push_back("world");
  REQUIRE(v2.capacity() == 5);
  REQUIRE(v2.length() == 5);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "testing");
  REQUIRE(v2.data()[1] == "omelas");
  REQUIRE(v2.data()[2] == "cookie");
  REQUIRE(v2.data()[3] == "hello");
  REQUIRE(v2.data()[4] == "world");

  v2.push_back("CIT");
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 6);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "testing");
  REQUIRE(v2.data()[1] == "omelas");
  REQUIRE(v2.data()[2] == "cookie");
  REQUIRE(v2.data()[3] == "hello");
  REQUIRE(v2.data()[4] == "world");
  REQUIRE(v2.data()[5] == "CIT");

  v2.push_back("5950");
  v2.push_back("Computer");
  v2.push_back("Systems");
  v2.push_back("Programming");
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 10);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "testing");
  REQUIRE(v2.data()[1] == "omelas");
  REQUIRE(v2.data()[2] == "cookie");
  REQUIRE(v2.data()[3] == "hello");
  REQUIRE(v2.data()[4] == "world");
  REQUIRE(v2.data()[5] == "CIT");
  REQUIRE(v2.data()[6] == "5950");
  REQUIRE(v2.data()[7] == "Computer");
  REQUIRE(v2.data()[8] == "Systems");
  REQUIRE(v2.data()[9] == "Programming");

  v2.push_back("testing");
  REQUIRE(v2.capacity() == 20);
  REQUIRE(v2.length() == 11);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "testing");
  REQUIRE(v2.data()[1] == "omelas");
  REQUIRE(v2.data()[2] == "cookie");
  REQUIRE(v2.data()[3] == "hello");
  REQUIRE(v2.data()[4] == "world");
  REQUIRE(v2.data()[5] == "CIT");
  REQUIRE(v2.data()[6] == "5950");
  REQUIRE(v2.data()[7] == "Computer");
  REQUIRE(v2.data()[8] == "Systems");
  REQUIRE(v2.data()[9] == "Programming");
  REQUIRE(v2.data()[10] == "testing");

  v2.push_back("testing");
  v2.push_back("adding");
  v2.push_back("elements");
  v2.push_back("to");
  v2.push_back("the");
  v2.push_back("end");
  v2.push_back("of");
  v2.push_back("a");
  v2.push_back("vector");

  REQUIRE(v2.capacity() == 20);
  REQUIRE(v2.length() == 20);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "testing");
  REQUIRE(v2.data()[1] == "omelas");
  REQUIRE(v2.data()[2] == "cookie");
  REQUIRE(v2.data()[3] == "hello");
  REQUIRE(v2.data()[4] == "world");
  REQUIRE(v2.data()[5] == "CIT");
  REQUIRE(v2.data()[6] == "5950");
  REQUIRE(v2.data()[7] == "Computer");
  REQUIRE(v2.data()[8] == "Systems");
  REQUIRE(v2.data()[9] == "Programming");
  REQUIRE(v2.data()[10] == "testing");
  REQUIRE(v2.data()[11] == "testing");
  REQUIRE(v2.data()[12] == "adding");
  REQUIRE(v2.data()[13] == "elements");
  REQUIRE(v2.data()[14] == "to");
  REQUIRE(v2.data()[15] == "the");
  REQUIRE(v2.data()[16] == "end");
  REQUIRE(v2.data()[17] == "of");
  REQUIRE(v2.data()[18] == "a");
  REQUIRE(v2.data()[19] == "vector");

  v2.push_back("object");

  REQUIRE(v2.capacity() == 40);
  REQUIRE(v2.length() == 21);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "testing");
  REQUIRE(v2.data()[1] == "omelas");
  REQUIRE(v2.data()[2] == "cookie");
  REQUIRE(v2.data()[3] == "hello");
  REQUIRE(v2.data()[4] == "world");
  REQUIRE(v2.data()[5] == "CIT");
  REQUIRE(v2.data()[6] == "5950");
  REQUIRE(v2.data()[7] == "Computer");
  REQUIRE(v2.data()[8] == "Systems");
  REQUIRE(v2.data()[9] == "Programming");
  REQUIRE(v2.data()[10] == "testing");
  REQUIRE(v2.data()[11] == "testing");
  REQUIRE(v2.data()[12] == "adding");
  REQUIRE(v2.data()[13] == "elements");
  REQUIRE(v2.data()[14] == "to");
  REQUIRE(v2.data()[15] == "the");
  REQUIRE(v2.data()[16] == "end");
  REQUIRE(v2.data()[17] == "of");
  REQUIRE(v2.data()[18] == "a");
  REQUIRE(v2.data()[19] == "vector");
  REQUIRE(v2.data()[20] == "object");

  vec v3(3);
  REQUIRE(v3.capacity() == 3);
  REQUIRE(v3.length() == 0);
  REQUIRE(v3.data() != nullptr);

  v3.push_back("testing");
  v3.push_back("omelas");
  v3.push_back("cookie");

  REQUIRE(v3.capacity() == 3);
  REQUIRE(v3.length() == 3);
  REQUIRE(v3.data() != nullptr);
  REQUIRE(v3.data()[0] == "testing");
  REQUIRE(v3.data()[1] == "omelas");
  REQUIRE(v3.data()[2] == "cookie");

  v3.push_back("hello");
  REQUIRE(v3.capacity() == 6);
  REQUIRE(v3.length() == 4);
  REQUIRE(v3.data() != nullptr);
  REQUIRE(v3.data()[0] == "testing");
  REQUIRE(v3.data()[1] == "omelas");
  REQUIRE(v3.data()[2] == "cookie");
  REQUIRE(v3.data()[3] == "hello");

  v3.push_back("world");
  REQUIRE(v3.capacity() == 6);
  REQUIRE(v3.length() == 5);
  REQUIRE(v3.data() != nullptr);
  REQUIRE(v3.data()[0] == "testing");
  REQUIRE(v3.data()[1] == "omelas");
  REQUIRE(v3.data()[2] == "cookie");
  REQUIRE(v3.data()[3] == "hello");
  REQUIRE(v3.data()[4] == "world");

  v3.push_back("CIT");
  REQUIRE(v3.capacity() == 6);
  REQUIRE(v3.length() == 6);
  REQUIRE(v3.data() != nullptr);
  REQUIRE(v3.data()[0] == "testing");
  REQUIRE(v3.data()[1] == "omelas");
  REQUIRE(v3.data()[2] == "cookie");
  REQUIRE(v3.data()[3] == "hello");
  REQUIRE(v3.data()[4] == "world");
  REQUIRE(v3.data()[5] == "CIT");

  v3.push_back("5950");
  v3.push_back("Computer");
  v3.push_back("Systems");
  v3.push_back("Programming");
  REQUIRE(v3.capacity() == 12);
  REQUIRE(v3.length() == 10);
  REQUIRE(v3.data() != nullptr);
  REQUIRE(v3.data()[0] == "testing");
  REQUIRE(v3.data()[1] == "omelas");
  REQUIRE(v3.data()[2] == "cookie");
  REQUIRE(v3.data()[3] == "hello");
  REQUIRE(v3.data()[4] == "world");
  REQUIRE(v3.data()[5] == "CIT");
  REQUIRE(v3.data()[6] == "5950");
  REQUIRE(v3.data()[7] == "Computer");
  REQUIRE(v3.data()[8] == "Systems");
  REQUIRE(v3.data()[9] == "Programming");

  v3.push_back("testing");
  v3.push_back("testing");
  v3.push_back("adding");
  v3.push_back("elements");
  v3.push_back("to");
  v3.push_back("the");
  v3.push_back("end");
  v3.push_back("of");
  v3.push_back("a");
  v3.push_back("vector");
  v3.push_back("object");

  REQUIRE(v3.capacity() == 24);
  REQUIRE(v3.length() == 21);
  REQUIRE(v3.data() != nullptr);
  REQUIRE(v3.data()[0] == "testing");
  REQUIRE(v3.data()[1] == "omelas");
  REQUIRE(v3.data()[2] == "cookie");
  REQUIRE(v3.data()[3] == "hello");
  REQUIRE(v3.data()[4] == "world");
  REQUIRE(v3.data()[5] == "CIT");
  REQUIRE(v3.data()[6] == "5950");
  REQUIRE(v3.data()[7] == "Computer");
  REQUIRE(v3.data()[8] == "Systems");
  REQUIRE(v3.data()[9] == "Programming");
  REQUIRE(v3.data()[10] == "testing");
  REQUIRE(v3.data()[11] == "testing");
  REQUIRE(v3.data()[12] == "adding");
  REQUIRE(v3.data()[13] == "elements");
  REQUIRE(v3.data()[14] == "to");
  REQUIRE(v3.data()[15] == "the");
  REQUIRE(v3.data()[16] == "end");
  REQUIRE(v3.data()[17] == "of");
  REQUIRE(v3.data()[18] == "a");
  REQUIRE(v3.data()[19] == "vector");
  REQUIRE(v3.data()[20] == "object");

  for (int i = 21; i < 100; i++) {
    std::string test_string = "test";
    test_string += i;
    v3.push_back(test_string);
  }
  REQUIRE(v3.capacity() == 192);
  REQUIRE(v3.length() == 100);
  REQUIRE(v3.data() != nullptr);
  REQUIRE(v3.data()[0] == "testing");
  REQUIRE(v3.data()[1] == "omelas");
  REQUIRE(v3.data()[2] == "cookie");
  REQUIRE(v3.data()[3] == "hello");
  REQUIRE(v3.data()[4] == "world");
  REQUIRE(v3.data()[5] == "CIT");
  REQUIRE(v3.data()[6] == "5950");
  REQUIRE(v3.data()[7] == "Computer");
  REQUIRE(v3.data()[8] == "Systems");
  REQUIRE(v3.data()[9] == "Programming");
  REQUIRE(v3.data()[10] == "testing");
  REQUIRE(v3.data()[11] == "testing");
  REQUIRE(v3.data()[12] == "adding");
  REQUIRE(v3.data()[13] == "elements");
  REQUIRE(v3.data()[14] == "to");
  REQUIRE(v3.data()[15] == "the");
  REQUIRE(v3.data()[16] == "end");
  REQUIRE(v3.data()[17] == "of");
  REQUIRE(v3.data()[18] == "a");
  REQUIRE(v3.data()[19] == "vector");
  REQUIRE(v3.data()[20] == "object");
  for (int i = 21; i < 100; i++) {
    std::string test_string = "test";
    test_string += i;
    REQUIRE(v3.data()[i] == test_string);
  }
}

TEST_CASE("basic pop", "[pop_back]") {
  vec v;

  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);

  v.push_back("testing");
  v.push_back("omelas");
  v.push_back("cookie");
  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 3);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "testing");
  REQUIRE(v.data()[1] == "omelas");
  REQUIRE(v.data()[2] == "cookie");

  std::optional<string> just_popped = v.pop_back();
  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 2);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "testing");
  REQUIRE(v.data()[1] == "omelas");
  REQUIRE(just_popped == "cookie");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 1);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "testing");
  REQUIRE(just_popped == "omelas");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);
  REQUIRE(just_popped == "testing");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);
  REQUIRE(just_popped == std::nullopt);
}

TEST_CASE("complex pop", "[pop_back]") {
  vec v(5);

  REQUIRE(v.capacity() == 5);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);

  std::optional<string> just_popped = v.pop_back();
  REQUIRE(v.capacity() == 5);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);
  REQUIRE(just_popped == std::nullopt);

  v.push_back("testing");
  REQUIRE(v.capacity() == 5);
  REQUIRE(v.length() == 1);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "testing");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 5);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);
  REQUIRE(just_popped == "testing");

  v.push_back("omelas");
  v.push_back("cookie");
  REQUIRE(v.capacity() == 5);
  REQUIRE(v.length() == 2);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "cookie");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 5);
  REQUIRE(v.length() == 1);
  REQUIRE(v.data() != nullptr);
  REQUIRE(just_popped == "cookie");

  v.push_back("hello");
  v.push_back("world");
  v.push_back("CIT");
  v.push_back("5950");
  REQUIRE(v.capacity() == 5);
  REQUIRE(v.length() == 5);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(v.data()[4] == "5950");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 5);
  REQUIRE(v.length() == 4);
  REQUIRE(v.data() != nullptr);
  REQUIRE(just_popped == "5950");

  v.push_back("Computer");
  v.push_back("Systems");
  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 6);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(v.data()[4] == "Computer");
  REQUIRE(v.data()[5] == "Systems");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 5);
  REQUIRE(v.data() != nullptr);
  REQUIRE(just_popped == "Systems");

  v.push_back("Programming");
  v.push_back("testing");
  v.push_back("testing");
  v.push_back("adding");
  v.push_back("elements");
  v.push_back("to");
  v.push_back("the");
  v.push_back("end");
  v.push_back("of");
  v.push_back("a");
  v.push_back("vector");
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 16);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(v.data()[4] == "Computer");
  REQUIRE(v.data()[5] == "Programming");
  REQUIRE(v.data()[6] == "testing");
  REQUIRE(v.data()[7] == "testing");
  REQUIRE(v.data()[8] == "adding");
  REQUIRE(v.data()[9] == "elements");
  REQUIRE(v.data()[10] == "to");
  REQUIRE(v.data()[11] == "the");
  REQUIRE(v.data()[12] == "end");
  REQUIRE(v.data()[13] == "of");
  REQUIRE(v.data()[14] == "a");
  REQUIRE(v.data()[15] == "vector");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 15);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(v.data()[4] == "Computer");
  REQUIRE(v.data()[5] == "Programming");
  REQUIRE(v.data()[6] == "testing");
  REQUIRE(v.data()[7] == "testing");
  REQUIRE(v.data()[8] == "adding");
  REQUIRE(v.data()[9] == "elements");
  REQUIRE(v.data()[10] == "to");
  REQUIRE(v.data()[11] == "the");
  REQUIRE(v.data()[12] == "end");
  REQUIRE(v.data()[13] == "of");
  REQUIRE(v.data()[14] == "a");
  REQUIRE(just_popped == "vector");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 14);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(v.data()[4] == "Computer");
  REQUIRE(v.data()[5] == "Programming");
  REQUIRE(v.data()[6] == "testing");
  REQUIRE(v.data()[7] == "testing");
  REQUIRE(v.data()[8] == "adding");
  REQUIRE(v.data()[9] == "elements");
  REQUIRE(v.data()[10] == "to");
  REQUIRE(v.data()[11] == "the");
  REQUIRE(v.data()[12] == "end");
  REQUIRE(v.data()[13] == "of");
  REQUIRE(just_popped == "a");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 13);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(v.data()[4] == "Computer");
  REQUIRE(v.data()[5] == "Programming");
  REQUIRE(v.data()[6] == "testing");
  REQUIRE(v.data()[7] == "testing");
  REQUIRE(v.data()[8] == "adding");
  REQUIRE(v.data()[9] == "elements");
  REQUIRE(v.data()[10] == "to");
  REQUIRE(v.data()[11] == "the");
  REQUIRE(v.data()[12] == "end");
  REQUIRE(just_popped == "of");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 12);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(v.data()[4] == "Computer");
  REQUIRE(v.data()[5] == "Programming");
  REQUIRE(v.data()[6] == "testing");
  REQUIRE(v.data()[7] == "testing");
  REQUIRE(v.data()[8] == "adding");
  REQUIRE(v.data()[9] == "elements");
  REQUIRE(v.data()[10] == "to");
  REQUIRE(v.data()[11] == "the");
  REQUIRE(just_popped == "end");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 11);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(v.data()[4] == "Computer");
  REQUIRE(v.data()[5] == "Programming");
  REQUIRE(v.data()[6] == "testing");
  REQUIRE(v.data()[7] == "testing");
  REQUIRE(v.data()[8] == "adding");
  REQUIRE(v.data()[9] == "elements");
  REQUIRE(v.data()[10] == "to");
  REQUIRE(just_popped == "the");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 10);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(v.data()[4] == "Computer");
  REQUIRE(v.data()[5] == "Programming");
  REQUIRE(v.data()[6] == "testing");
  REQUIRE(v.data()[7] == "testing");
  REQUIRE(v.data()[8] == "adding");
  REQUIRE(v.data()[9] == "elements");
  REQUIRE(just_popped == "to");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 9);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(v.data()[4] == "Computer");
  REQUIRE(v.data()[5] == "Programming");
  REQUIRE(v.data()[6] == "testing");
  REQUIRE(v.data()[7] == "testing");
  REQUIRE(v.data()[8] == "adding");
  REQUIRE(just_popped == "elements");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 8);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(v.data()[4] == "Computer");
  REQUIRE(v.data()[5] == "Programming");
  REQUIRE(v.data()[6] == "testing");
  REQUIRE(v.data()[7] == "testing");
  REQUIRE(just_popped == "adding");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 7);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(v.data()[4] == "Computer");
  REQUIRE(v.data()[5] == "Programming");
  REQUIRE(v.data()[6] == "testing");
  REQUIRE(just_popped == "testing");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 6);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(v.data()[4] == "Computer");
  REQUIRE(v.data()[5] == "Programming");
  REQUIRE(just_popped == "testing");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 5);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(v.data()[4] == "Computer");
  REQUIRE(just_popped == "Programming");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 4);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(v.data()[3] == "CIT");
  REQUIRE(just_popped == "Computer");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 3);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(v.data()[2] == "world");
  REQUIRE(just_popped == "CIT");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 2);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(v.data()[1] == "hello");
  REQUIRE(just_popped == "world");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 1);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.data()[0] == "omelas");
  REQUIRE(just_popped == "hello");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);
  REQUIRE(just_popped == "omelas");

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);
  REQUIRE(just_popped == std::nullopt);

  just_popped = v.pop_back();
  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);
  REQUIRE(just_popped == std::nullopt);

  vec v2(5);

  REQUIRE(v2.capacity() == 5);
  REQUIRE(v2.length() == 0);
  REQUIRE(v2.data() != nullptr);

  v2.push_back("testing");
  v2.push_back("omelas");
  REQUIRE(v2.capacity() == 5);
  REQUIRE(v2.length() == 2);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "testing");
  REQUIRE(v2.data()[1] == "omelas");

  just_popped = v2.pop_back();
  REQUIRE(v2.capacity() == 5);
  REQUIRE(v2.length() == 1);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(just_popped == "omelas");

  just_popped = v2.pop_back();
  REQUIRE(v2.capacity() == 5);
  REQUIRE(v2.length() == 0);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(just_popped == "testing");

  just_popped = v2.pop_back();
  REQUIRE(v2.capacity() == 5);
  REQUIRE(v2.length() == 0);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(just_popped == std::nullopt);

  v2.push_back("cookie");
  v2.push_back("hello");
  v2.push_back("world");
  v2.push_back("CIT");
  v2.push_back("5950");
  v2.push_back("Computer");
  v2.push_back("Systems");
  v2.push_back("Programming");
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 8);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "cookie");
  REQUIRE(v2.data()[1] == "hello");
  REQUIRE(v2.data()[2] == "world");
  REQUIRE(v2.data()[3] == "CIT");
  REQUIRE(v2.data()[4] == "5950");
  REQUIRE(v2.data()[5] == "Computer");
  REQUIRE(v2.data()[6] == "Systems");
  REQUIRE(v2.data()[7] == "Programming");

  just_popped = v2.pop_back();
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 7);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(just_popped == "Programming");

  just_popped = v2.pop_back();
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 6);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(just_popped == "Systems");

  just_popped = v2.pop_back();
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 5);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(just_popped == "Computer");

  just_popped = v2.pop_back();
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 4);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(just_popped == "5950");

  just_popped = v2.pop_back();
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 3);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(just_popped == "CIT");
  REQUIRE(v2.data()[0] == "cookie");
  REQUIRE(v2.data()[1] == "hello");
  REQUIRE(v2.data()[2] == "world");

  v2.push_back("testing");
  v2.push_back("testing");
  v2.push_back("adding");
  v2.push_back("elements");
  v2.push_back("to");
  v2.push_back("the");
  v2.push_back("end");
  v2.push_back("of");
  v2.push_back("a");
  v2.push_back("vector");
  v2.push_back("object");
  REQUIRE(v2.capacity() == 20);
  REQUIRE(v2.length() == 14);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.data()[0] == "cookie");
  REQUIRE(v2.data()[1] == "hello");
  REQUIRE(v2.data()[2] == "world");
  REQUIRE(v2.data()[3] == "testing");
  REQUIRE(v2.data()[4] == "testing");
  REQUIRE(v2.data()[5] == "adding");
  REQUIRE(v2.data()[6] == "elements");
  REQUIRE(v2.data()[7] == "to");
  REQUIRE(v2.data()[8] == "the");
  REQUIRE(v2.data()[9] == "end");
  REQUIRE(v2.data()[10] == "of");
  REQUIRE(v2.data()[11] == "a");
  REQUIRE(v2.data()[12] == "vector");
  REQUIRE(v2.data()[13] == "object");

  just_popped = v2.pop_back();
  REQUIRE(v2.capacity() == 20);
  REQUIRE(v2.length() == 13);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(just_popped == "object");

  just_popped = v2.pop_back();
  REQUIRE(v2.capacity() == 20);
  REQUIRE(v2.length() == 12);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(just_popped == "vector");

  just_popped = v2.pop_back();
  REQUIRE(v2.capacity() == 20);
  REQUIRE(v2.length() == 11);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(just_popped == "a");
  REQUIRE(v2.data()[0] == "cookie");
  REQUIRE(v2.data()[1] == "hello");
  REQUIRE(v2.data()[2] == "world");
  REQUIRE(v2.data()[3] == "testing");
  REQUIRE(v2.data()[4] == "testing");
  REQUIRE(v2.data()[5] == "adding");
  REQUIRE(v2.data()[6] == "elements");
  REQUIRE(v2.data()[7] == "to");
  REQUIRE(v2.data()[8] == "the");
  REQUIRE(v2.data()[9] == "end");
  REQUIRE(v2.data()[10] == "of");

  vec v3(3);
  REQUIRE(v3.capacity() == 3);
  REQUIRE(v3.length() == 0);
  REQUIRE(v3.data() != nullptr);

  v3.push_back("testing");
  v3.push_back("omelas");
  v3.push_back("cookie");
  v3.push_back("hello");
  v3.push_back("world");
  v3.push_back("CIT");
  v3.push_back("5950");
  v3.push_back("Computer");
  v3.push_back("Systems");
  v3.push_back("Programming");
  v3.push_back("testing");
  v3.push_back("testing");
  v3.push_back("adding");
  v3.push_back("elements");
  v3.push_back("to");
  v3.push_back("the");
  v3.push_back("end");
  v3.push_back("of");
  v3.push_back("a");
  v3.push_back("vector");
  v3.push_back("object");
  REQUIRE(v3.capacity() == 24);
  REQUIRE(v3.length() == 21);
  REQUIRE(v3.data() != nullptr);
  REQUIRE(v3.data()[0] == "testing");
  REQUIRE(v3.data()[1] == "omelas");
  REQUIRE(v3.data()[2] == "cookie");
  REQUIRE(v3.data()[3] == "hello");
  REQUIRE(v3.data()[4] == "world");
  REQUIRE(v3.data()[5] == "CIT");
  REQUIRE(v3.data()[6] == "5950");
  REQUIRE(v3.data()[7] == "Computer");
  REQUIRE(v3.data()[8] == "Systems");
  REQUIRE(v3.data()[9] == "Programming");
  REQUIRE(v3.data()[10] == "testing");
  REQUIRE(v3.data()[11] == "testing");
  REQUIRE(v3.data()[12] == "adding");
  REQUIRE(v3.data()[13] == "elements");
  REQUIRE(v3.data()[14] == "to");
  REQUIRE(v3.data()[15] == "the");
  REQUIRE(v3.data()[16] == "end");
  REQUIRE(v3.data()[17] == "of");
  REQUIRE(v3.data()[18] == "a");
  REQUIRE(v3.data()[19] == "vector");
  REQUIRE(v3.data()[20] == "object");

  just_popped = v3.pop_back();
  REQUIRE(v3.capacity() == 24);
  REQUIRE(v3.length() == 20);
  REQUIRE(v3.data() != nullptr);
  REQUIRE(v3.data()[0] == "testing");
  REQUIRE(v3.data()[1] == "omelas");
  REQUIRE(v3.data()[2] == "cookie");
  REQUIRE(v3.data()[3] == "hello");
  REQUIRE(v3.data()[4] == "world");
  REQUIRE(v3.data()[5] == "CIT");
  REQUIRE(v3.data()[6] == "5950");
  REQUIRE(v3.data()[7] == "Computer");
  REQUIRE(v3.data()[8] == "Systems");
  REQUIRE(v3.data()[9] == "Programming");
  REQUIRE(v3.data()[10] == "testing");
  REQUIRE(v3.data()[11] == "testing");
  REQUIRE(v3.data()[12] == "adding");
  REQUIRE(v3.data()[13] == "elements");
  REQUIRE(v3.data()[14] == "to");
  REQUIRE(v3.data()[15] == "the");
  REQUIRE(v3.data()[16] == "end");
  REQUIRE(v3.data()[17] == "of");
  REQUIRE(v3.data()[18] == "a");
  REQUIRE(v3.data()[19] == "vector");
  REQUIRE(just_popped == "object");

  for (int i = 20; i < 100; i++) {
    std::string test_string = "test";
    test_string += i;
    v3.push_back(test_string);
  }
  REQUIRE(v3.capacity() == 192);
  REQUIRE(v3.length() == 100);
  REQUIRE(v3.data() != nullptr);
  REQUIRE(v3.data()[0] == "testing");
  REQUIRE(v3.data()[1] == "omelas");
  REQUIRE(v3.data()[2] == "cookie");
  REQUIRE(v3.data()[3] == "hello");
  REQUIRE(v3.data()[4] == "world");
  REQUIRE(v3.data()[5] == "CIT");
  REQUIRE(v3.data()[6] == "5950");
  REQUIRE(v3.data()[7] == "Computer");
  REQUIRE(v3.data()[8] == "Systems");
  REQUIRE(v3.data()[9] == "Programming");
  REQUIRE(v3.data()[10] == "testing");
  REQUIRE(v3.data()[11] == "testing");
  REQUIRE(v3.data()[12] == "adding");
  REQUIRE(v3.data()[13] == "elements");
  REQUIRE(v3.data()[14] == "to");
  REQUIRE(v3.data()[15] == "the");
  REQUIRE(v3.data()[16] == "end");
  REQUIRE(v3.data()[17] == "of");
  REQUIRE(v3.data()[18] == "a");
  REQUIRE(v3.data()[19] == "vector");
  for (int i = 20; i < 100; i++) {
    std::string test_string = "test";
    test_string += i;
    REQUIRE(v3.data()[i] == test_string);
  }

  for (int i = 99; i >= 20; i--) {
    just_popped = v3.pop_back();
    std::string test_string = "test";
    test_string += i;
    REQUIRE(just_popped == test_string);
  }
  REQUIRE(v3.capacity() == 192);
  REQUIRE(v3.length() == 20);
  REQUIRE(v3.data() != nullptr);
  REQUIRE(v3.data()[0] == "testing");
  REQUIRE(v3.data()[1] == "omelas");
  REQUIRE(v3.data()[2] == "cookie");
  REQUIRE(v3.data()[3] == "hello");
  REQUIRE(v3.data()[4] == "world");
  REQUIRE(v3.data()[5] == "CIT");
  REQUIRE(v3.data()[6] == "5950");
  REQUIRE(v3.data()[7] == "Computer");
  REQUIRE(v3.data()[8] == "Systems");
  REQUIRE(v3.data()[9] == "Programming");
  REQUIRE(v3.data()[10] == "testing");
  REQUIRE(v3.data()[11] == "testing");
  REQUIRE(v3.data()[12] == "adding");
  REQUIRE(v3.data()[13] == "elements");
  REQUIRE(v3.data()[14] == "to");
  REQUIRE(v3.data()[15] == "the");
  REQUIRE(v3.data()[16] == "end");
  REQUIRE(v3.data()[17] == "of");
  REQUIRE(v3.data()[18] == "a");
  REQUIRE(v3.data()[19] == "vector");

  for (int i = 19; i > 0; i--) {
    just_popped = v3.pop_back();
  }
  REQUIRE(v3.capacity() == 192);
  REQUIRE(v3.length() == 1);
  REQUIRE(v3.data() != nullptr);
  REQUIRE(v3.data()[0] == "testing");
  REQUIRE(just_popped == "omelas");

  just_popped = v3.pop_back();
  REQUIRE(v3.capacity() == 192);
  REQUIRE(v3.length() == 0);
  REQUIRE(v3.data() != nullptr);
  REQUIRE(just_popped == "testing");

  just_popped = v3.pop_back();
  REQUIRE(v3.capacity() == 192);
  REQUIRE(v3.length() == 0);
  REQUIRE(v3.data() != nullptr);
  REQUIRE(just_popped == std::nullopt);
}

TEST_CASE("basic operator[]", "[operator]") {
  vec v;

  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);

  v.push_back("testing");
  v.push_back("omelas");
  v.push_back("cookie");

  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 3);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v[0] == "testing");
  REQUIRE(v[1] == "omelas");
  REQUIRE(v[2] == "cookie");

  REQUIRE_THROWS_AS(v.at(4), out_of_range);
  REQUIRE_THROWS_AS(v.at(100), out_of_range);
  REQUIRE_THROWS_AS(v.at(-1), out_of_range);
}

TEST_CASE("complex operator[]", "[operator]") {
  vec v;

  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);
  REQUIRE_THROWS_AS(v[0], out_of_range);
  REQUIRE_THROWS_AS(v[100], out_of_range);
  REQUIRE_THROWS_AS(v[-1], out_of_range);

  v.push_back("testing");
  v.push_back("omelas");
  v.push_back("cookie");
  v.push_back("hello");
  v.push_back("world");
  v.push_back("CIT");
  v.push_back("5950");
  v.push_back("Computer");
  v.push_back("Systems");
  v.push_back("Programming");
  v.push_back("testing");
  v.push_back("testing");
  v.push_back("adding");
  v.push_back("elements");
  v.push_back("to");
  v.push_back("the");
  v.push_back("end");
  v.push_back("of");
  v.push_back("a");
  v.push_back("vector");

  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 20);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v[0] == "testing");
  REQUIRE(v[1] == "omelas");
  REQUIRE(v[2] == "cookie");
  REQUIRE(v[3] == "hello");
  REQUIRE(v[4] == "world");
  REQUIRE(v[5] == "CIT");
  REQUIRE(v[6] == "5950");
  REQUIRE(v[7] == "Computer");
  REQUIRE(v[8] == "Systems");
  REQUIRE(v[9] == "Programming");
  REQUIRE(v[10] == "testing");
  REQUIRE(v[11] == "testing");
  REQUIRE(v[12] == "adding");
  REQUIRE(v[13] == "elements");
  REQUIRE(v[14] == "to");
  REQUIRE(v[15] == "the");
  REQUIRE(v[16] == "end");
  REQUIRE(v[17] == "of");
  REQUIRE(v[18] == "a");
  REQUIRE(v[19] == "vector");
  REQUIRE_THROWS_AS(v[20], out_of_range);
  REQUIRE_THROWS_AS(v[100], out_of_range);
  REQUIRE_THROWS_AS(v[-1], out_of_range);

  v.pop_back();
  REQUIRE_THROWS_AS(v[19], out_of_range);

  vec v2(5);

  REQUIRE(v2.capacity() == 5);
  REQUIRE(v2.length() == 0);
  REQUIRE(v2.data() != nullptr);

  v2.push_back("testing");
  v2.push_back("omelas");
  v2.push_back("cookie");
  v2.push_back("hello");
  v2.push_back("world");
  REQUIRE(v2.capacity() == 5);
  REQUIRE(v2.length() == 5);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2[0] == "testing");
  REQUIRE(v2[1] == "omelas");
  REQUIRE(v2[2] == "cookie");
  REQUIRE(v2[3] == "hello");
  REQUIRE(v2[4] == "world");
  REQUIRE_THROWS_AS(v2[5], out_of_range);
  REQUIRE_THROWS_AS(v2[100], out_of_range);
  REQUIRE_THROWS_AS(v2[-1], out_of_range);

  v2.push_back("CIT");
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 6);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2[0] == "testing");
  REQUIRE(v2[1] == "omelas");
  REQUIRE(v2[2] == "cookie");
  REQUIRE(v2[3] == "hello");
  REQUIRE(v2[4] == "world");
  REQUIRE(v2[5] == "CIT");

  v2.push_back("5950");
  v2.push_back("Computer");
  v2.push_back("Systems");
  v2.push_back("Programming");
  v2.push_back("testing");
  REQUIRE(v2.capacity() == 20);
  REQUIRE(v2.length() == 11);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2[0] == "testing");
  REQUIRE(v2[1] == "omelas");
  REQUIRE(v2[2] == "cookie");
  REQUIRE(v2[3] == "hello");
  REQUIRE(v2[4] == "world");
  REQUIRE(v2[5] == "CIT");
  REQUIRE(v2[6] == "5950");
  REQUIRE(v2[7] == "Computer");
  REQUIRE(v2[8] == "Systems");
  REQUIRE(v2[9] == "Programming");
  REQUIRE(v2[10] == "testing");
  REQUIRE_THROWS_AS(v2[11], out_of_range);
  REQUIRE_THROWS_AS(v2[100], out_of_range);
  REQUIRE_THROWS_AS(v2[-1], out_of_range);

  for (int i = 0; i < 10; i++) {
    v2.pop_back();
  }
  REQUIRE(v2.capacity() == 20);
  REQUIRE(v2.length() == 1);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2[0] == "testing");
  REQUIRE_THROWS_AS(v2[1], out_of_range);
  REQUIRE_THROWS_AS(v2[100], out_of_range);
  REQUIRE_THROWS_AS(v2[-1], out_of_range);

  v2.pop_back();
  REQUIRE(v2.capacity() == 20);
  REQUIRE(v2.length() == 0);
  REQUIRE(v2.data() != nullptr);
  REQUIRE_THROWS_AS(v2[0], out_of_range);
  REQUIRE_THROWS_AS(v[100], out_of_range);
  REQUIRE_THROWS_AS(v[-1], out_of_range);
}

TEST_CASE("basic at", "[at]") {
  vec v;

  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);

  v.push_back("testing");
  v.push_back("omelas");
  v.push_back("cookie");

  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 3);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.at(0) == "testing");
  REQUIRE(v.at(1) == "omelas");
  REQUIRE(v.at(2) == "cookie");

  REQUIRE_THROWS_AS(v.at(4), out_of_range);
  REQUIRE_THROWS_AS(v.at(100), out_of_range);
  REQUIRE_THROWS_AS(v.at(-1), out_of_range);
}

TEST_CASE("complex at", "[at]") {
  vec v;

  REQUIRE(v.capacity() == 10);
  REQUIRE(v.length() == 0);
  REQUIRE(v.data() != nullptr);
  REQUIRE_THROWS_AS(v.at(0), out_of_range);
  REQUIRE_THROWS_AS(v.at(100), out_of_range);
  REQUIRE_THROWS_AS(v.at(-1), out_of_range);

  v.push_back("testing");
  v.push_back("omelas");
  v.push_back("cookie");
  v.push_back("hello");
  v.push_back("world");
  v.push_back("CIT");
  v.push_back("5950");
  v.push_back("Computer");
  v.push_back("Systems");
  v.push_back("Programming");
  v.push_back("testing");
  v.push_back("testing");
  v.push_back("adding");
  v.push_back("elements");
  v.push_back("to");
  v.push_back("the");
  v.push_back("end");
  v.push_back("of");
  v.push_back("a");
  v.push_back("vector");

  REQUIRE(v.capacity() == 20);
  REQUIRE(v.length() == 20);
  REQUIRE(v.data() != nullptr);
  REQUIRE(v.at(0) == "testing");
  REQUIRE(v.at(1) == "omelas");
  REQUIRE(v.at(2) == "cookie");
  REQUIRE(v.at(3) == "hello");
  REQUIRE(v.at(4) == "world");
  REQUIRE(v.at(5) == "CIT");
  REQUIRE(v.at(6) == "5950");
  REQUIRE(v.at(7) == "Computer");
  REQUIRE(v.at(8) == "Systems");
  REQUIRE(v.at(9) == "Programming");
  REQUIRE(v.at(10) == "testing");
  REQUIRE(v.at(11) == "testing");
  REQUIRE(v.at(12) == "adding");
  REQUIRE(v.at(13) == "elements");
  REQUIRE(v.at(14) == "to");
  REQUIRE(v.at(15) == "the");
  REQUIRE(v.at(16) == "end");
  REQUIRE(v.at(17) == "of");
  REQUIRE(v.at(18) == "a");
  REQUIRE(v.at(19) == "vector");
  REQUIRE_THROWS_AS(v.at(20), out_of_range);
  REQUIRE_THROWS_AS(v.at(100), out_of_range);
  REQUIRE_THROWS_AS(v.at(-1), out_of_range);

  v.pop_back();
  REQUIRE_THROWS_AS(v.at(19), out_of_range);

  vec v2(5);

  REQUIRE(v2.capacity() == 5);
  REQUIRE(v2.length() == 0);
  REQUIRE(v2.data() != nullptr);

  v2.push_back("testing");
  v2.push_back("omelas");
  v2.push_back("cookie");
  v2.push_back("hello");
  v2.push_back("world");
  REQUIRE(v2.capacity() == 5);
  REQUIRE(v2.length() == 5);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.at(0) == "testing");
  REQUIRE(v2.at(1) == "omelas");
  REQUIRE(v2.at(2) == "cookie");
  REQUIRE(v2.at(3) == "hello");
  REQUIRE(v2.at(4) == "world");
  REQUIRE_THROWS_AS(v2.at(5), out_of_range);
  REQUIRE_THROWS_AS(v2.at(100), out_of_range);
  REQUIRE_THROWS_AS(v2.at(-1), out_of_range);

  v2.push_back("CIT");
  REQUIRE(v2.capacity() == 10);
  REQUIRE(v2.length() == 6);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.at(0) == "testing");
  REQUIRE(v2.at(1) == "omelas");
  REQUIRE(v2.at(2) == "cookie");
  REQUIRE(v2.at(3) == "hello");
  REQUIRE(v2.at(4) == "world");
  REQUIRE(v2.at(5) == "CIT");

  v2.push_back("5950");
  v2.push_back("Computer");
  v2.push_back("Systems");
  v2.push_back("Programming");
  v2.push_back("testing");
  REQUIRE(v2.capacity() == 20);
  REQUIRE(v2.length() == 11);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.at(0) == "testing");
  REQUIRE(v2.at(1) == "omelas");
  REQUIRE(v2.at(2) == "cookie");
  REQUIRE(v2.at(3) == "hello");
  REQUIRE(v2.at(4) == "world");
  REQUIRE(v2.at(5) == "CIT");
  REQUIRE(v2.at(6) == "5950");
  REQUIRE(v2.at(7) == "Computer");
  REQUIRE(v2.at(8) == "Systems");
  REQUIRE(v2.at(9) == "Programming");
  REQUIRE(v2.at(10) == "testing");
  REQUIRE_THROWS_AS(v2.at(11), out_of_range);
  REQUIRE_THROWS_AS(v2.at(100), out_of_range);
  REQUIRE_THROWS_AS(v2.at(-1), out_of_range);

  for (int i = 0; i < 10; i++) {
    v2.pop_back();
  }
  REQUIRE(v2.capacity() == 20);
  REQUIRE(v2.length() == 1);
  REQUIRE(v2.data() != nullptr);
  REQUIRE(v2.at(0) == "testing");
  REQUIRE_THROWS_AS(v2.at(1), out_of_range);
  REQUIRE_THROWS_AS(v2.at(100), out_of_range);
  REQUIRE_THROWS_AS(v2.at(-1), out_of_range);

  v2.pop_back();
  REQUIRE(v2.capacity() == 20);
  REQUIRE(v2.length() == 0);
  REQUIRE(v2.data() != nullptr);
  REQUIRE_THROWS_AS(v2.at(0), out_of_range);
  REQUIRE_THROWS_AS(v.at(100), out_of_range);
  REQUIRE_THROWS_AS(v.at(-1), out_of_range);
}

TEST_CASE("basic copy constructor", "[copy_ctor]") {
  vec original;

  // Empty copy
  vec empty_copy(original);
  REQUIRE(empty_copy.length() == original.length());
  REQUIRE(empty_copy.capacity() == original.capacity());

  original.push_back("alpha");
  original.push_back("beta");
  original.push_back("gamma");

  // Copy with values
  vec copy_of_original(original);
  REQUIRE(copy_of_original.length() == original.length());
  REQUIRE(copy_of_original.capacity() >= original.length());
  REQUIRE(copy_of_original.at(0) == original.at(0));
  REQUIRE(copy_of_original.at(1) == original.at(1));
  REQUIRE(copy_of_original.at(2) == original.at(2));

  // Memory independence
  REQUIRE(original.data() != copy_of_original.data());

  // Deep copy
  original.push_back("delta");
  REQUIRE(original.length() == 4);
  REQUIRE(copy_of_original.length() == 3);
  original.pop_back();
  original.pop_back();
  REQUIRE(copy_of_original.at(2) == "gamma");
  REQUIRE(copy_of_original.length() == 3);
  copy_of_original.at(0) = "changed";
  REQUIRE(copy_of_original.at(0) != original.at(0));
  REQUIRE(copy_of_original.at(0) == "changed");
  REQUIRE(original.at(0) == "alpha");

  // Copy vector with initial capacity
  vec preallocated(20);
  preallocated.push_back("hello");
  preallocated.push_back("world");
  REQUIRE(preallocated.length() == 2);
  REQUIRE(preallocated.capacity() == 20);
  vec copy_of_preallocated(preallocated);
  REQUIRE(copy_of_preallocated.length() == 2);
  REQUIRE(copy_of_preallocated.capacity() == 20);
  REQUIRE(copy_of_preallocated.at(0) == "hello");
  REQUIRE(copy_of_preallocated.at(1) == "world");
  REQUIRE(copy_of_preallocated.data() != preallocated.data());

  // Test reserve with copy
  original.reserve(10);
  vec reserved_copy(original);
  REQUIRE(reserved_copy.capacity() >= 10);
  REQUIRE(reserved_copy.length() == original.length());

  // Larger vector copy
  vec large_vec;
  for (size_t i = 0; i < 50; ++i) {
    large_vec.push_back("Item" + std::to_string(i));
  }
  vec large_copy(large_vec);
  REQUIRE(large_copy.length() == 50);
  bool identical = true;
  for (size_t i = 0; i < 50; ++i) {
    if (large_copy.at(i) != large_vec.at(i)) {
      identical = false;
      break;
    }
  }
  REQUIRE(identical == true);
}

TEST_CASE("complex copy constructor", "[copy_ctor]") {
  
  vec original;
  for (size_t i = 0; i < 100; ++i) {
    original.push_back("Item" + std::to_string(i));
  }

  // Create three chained copies
  vec copy1(original);
  vec copy2(copy1);
  vec copy3(copy2);

  // Modify original
  original.push_back("gamma");
  REQUIRE(original.length() == 101);
  REQUIRE(copy1.length() == 100);
  REQUIRE(copy2.length() == 100);
  REQUIRE(copy3.length() == 100);

  // Modify copy1
  copy1.push_back("delta");
  REQUIRE(copy1.length() == 101);
  REQUIRE(copy1.at(100) == "delta");
  REQUIRE(copy2.length() == 100);
  REQUIRE(copy3.length() == 100);
  REQUIRE(original.length() == 101);

  // Modify copy2
  copy2.at(0) = "changed";
  REQUIRE(copy2.at(0) == "changed");
  REQUIRE(copy1.at(0) == "Item0");
  REQUIRE(copy3.at(0) == "Item0");
  REQUIRE(original.at(0) == "Item0");

  // Modify copy3
  copy3.push_back("epsilon");
  REQUIRE(copy3.length() == 101);
  REQUIRE(copy3.at(100) == "epsilon");
  REQUIRE(copy2.length() == 100);
  REQUIRE(copy1.length() == 101);
  REQUIRE(original.length() == 101);

  // Verify original and copies remain independent
  REQUIRE(copy1.data() != original.data());
  REQUIRE(copy2.data() != copy1.data());
  REQUIRE(copy3.data() != copy2.data());
  REQUIRE(copy3.data() != original.data());

  // Complex Strings
  vec large_vec;
  for (size_t i = 0; i < 100; ++i) {
    std::string large_string(1000, 'A' + (i % 26));
    large_vec.push_back(large_string);
  }
  vec large_copy(large_vec);
  REQUIRE(large_copy.data() != large_vec.data());
  REQUIRE(large_copy.length() == large_vec.length());
  
  bool identical = true;
  for (size_t i = 0; i < large_vec.length(); ++i) {
    if (large_copy.at(i) != large_vec.at(i)) {
      identical = false;
      break;
    }
  }
  REQUIRE(identical == true);

  // Copy a large vector with initial capacity
  vec large_preallocated(200);
  for (size_t i = 0; i < 150; ++i) {
    large_preallocated.push_back("BigItem" + std::to_string(i));
  }

  REQUIRE(large_preallocated.length() == 150);
  REQUIRE(large_preallocated.capacity() == 200);
  vec large_preallocated_copy(large_preallocated);
  REQUIRE(large_preallocated_copy.length() == 150);
  REQUIRE(large_preallocated_copy.capacity() == 200);
  identical = true;
  for (size_t i = 0; i < 150; ++i) {
    if (large_preallocated_copy.at(i) != large_preallocated.at(i)){
      identical = false;
    }
  }
  REQUIRE(identical == true);
  REQUIRE(large_preallocated_copy.data() != large_preallocated.data());
}
