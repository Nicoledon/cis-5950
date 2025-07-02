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
