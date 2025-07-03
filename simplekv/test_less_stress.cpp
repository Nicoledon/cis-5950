#include "catch.hpp"
#include "SimpleKV.hpp"

#include <string>

using namespace simplekv;
using namespace std;

TEST_CASE("100 stress test", "[stress]") {
  SimpleKV kv = {};
  vector<string> expected = {};  // will be reinitialized everytime before it is used
  vector<string> actual = {};  // will be reinitialized everytime before it is used
  optional<string> str_opt = nullopt;  // will be reinitialized everytime before it is used
  optional<vector<string>> vec_opt = nullopt;  // will be reinitialized everytime before it is used

  // initial population of the kv
  kv.sset("Kevin", "Jeremie", "Lang");
  kv.sset("Sean", "Harry", "Serena");
  kv.sset("Kevin", "Tom", "Kevin");
  REQUIRE(kv.rpush("Yuna", "Serena", "Tom") == 1);
  REQUIRE(kv.rpush("Sean", "Tom", "Heyi") == 1);
  kv.sset("Tom", "Travis", "Eric");
  REQUIRE(kv.rpush("Travis", "Serena", "Travis") == 1);
  REQUIRE(kv.rpush("Tom", "Serena", "Serena") == 1);
  kv.sset("Serena", "Sean", "Sean");
  REQUIRE(kv.rpush("Lang", "Tom", "Kevin") == 1);
  REQUIRE(kv.rpush("Sean", "Tom", "Arvind") == 1);
  REQUIRE(kv.rpush("Heyi", "Serena", "Harry") == 1);
  kv.sset("Heyi", "Kyrie", "Arvind");
  REQUIRE(kv.rpush("Lang", "CV", "Kyrie") == 1);
  REQUIRE(kv.lpush("Lang", "CV", "Lang") == 1);
  kv.sset("Harry", "CV", "Eric");
  REQUIRE(kv.lpush("Travis", "Serena", "Heyi") == 1);
  REQUIRE(kv.rpush("Sean", "Harry", "Jeremie") == 0);
  REQUIRE(kv.rpush("Heyi", "Kyrie", "Kevin") == 0);
  REQUIRE(kv.rpush("Kevin", "Jeremie", "Tom") == 0);
  REQUIRE(kv.rpush("Sean", "Serena", "Heyi") == 1);
  REQUIRE(kv.rpush("Harry", "CV", "Felix") == 0);
  REQUIRE(kv.rpush("Serena", "Sean", "Kevin") == 0);
  REQUIRE(kv.rpush("Tom", "Travis", "Travis") == 0);
  REQUIRE(kv.rpush("Yuna", "Serena", "Felix") == 1);
  REQUIRE(kv.rpush("Sean", "Eric", "Tom") == 1);
  REQUIRE(kv.rpush("Lang", "Tom", "Kyrie") == 1);
  kv.sset("Serena", "Sean", "Lang");
  REQUIRE(kv.rpush("Harry", "Kevin", "Eric") == 1);
  kv.sset("Tom", "Travis", "Jeremie");

  // check the initial population of the kv

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 9);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Harry"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom", "Felix"};
      vec_opt = kv.lmembers("Yuna", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Yuna", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Yuna", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Serena"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Tom"));
    REQUIRE(kv.type("Kevin", "Tom") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Tom");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Kevin");
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }



  REQUIRE(kv.lset("Tom", "Serena", 0, "CV") == 1);


































































































































































































































  REQUIRE(kv.lpop("Yuna", "Serena") == "Tom");

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 9);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Harry"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Felix"};
      vec_opt = kv.lmembers("Yuna", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Yuna", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Yuna", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Tom"));
    REQUIRE(kv.type("Kevin", "Tom") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Tom");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Kevin");
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.del("Felix", "Yuna") == 0);


































































































































































































































  REQUIRE(kv.rpush("Yuna", "Serena", "Sean") == 1);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 9);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Harry"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Felix", "Sean"};
      vec_opt = kv.lmembers("Yuna", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Yuna", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Yuna", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Tom"));
    REQUIRE(kv.type("Kevin", "Tom") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Tom");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Kevin");
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.rpush("Lang", "Tom", "Felix") == 1);


































































































































































































































  REQUIRE(kv.lpush("Heyi", "Serena", "Kevin") == 1);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 9);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Harry"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Felix", "Sean"};
      vec_opt = kv.lmembers("Yuna", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Yuna", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Yuna", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Tom"));
    REQUIRE(kv.type("Kevin", "Tom") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Tom");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Kevin");
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.del("Kevin", "Tom") == 1);




























































































































































































































  REQUIRE(kv.rpush("Lang", "Tom", "Harry") == 1);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 9);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Harry"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Felix", "Sean"};
      vec_opt = kv.lmembers("Yuna", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Yuna", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Yuna", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.lset("Kevin", "Jeremie", 0U, "Arvind") == false);




























































































































































































































  REQUIRE(kv.ns_exists("Yuna") == 1);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 9);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Harry"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Felix", "Sean"};
      vec_opt = kv.lmembers("Yuna", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Yuna", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Yuna", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.rpush("Heyi", "Serena", "Tom") == 1);




























































































































































































































  REQUIRE(kv.ns_exists("Travis") == 1);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 9);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Felix", "Sean"};
      vec_opt = kv.lmembers("Yuna", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Yuna", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Yuna", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.del("Eric", "CV") == 0);




























































































































































































































  REQUIRE(kv.rpush("Felix", "Serena", "Tom") == 1);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Felix") != namespaces.end());
    REQUIRE(kv.ns_exists("Felix"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Felix");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Felix", "Serena"));
    REQUIRE(kv.type("Felix", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom"};
      vec_opt = kv.lmembers("Felix", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Felix", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Felix", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Felix", "Sean"};
      vec_opt = kv.lmembers("Yuna", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Yuna", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Yuna", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.rpush("Lang", "CV", "Serena") == 1);














































































































































































































































  vec_opt = kv.lunion("Yuna", "Serena", "Harry", "Serena");
  REQUIRE(vec_opt.has_value());
  actual = vec_opt.value();
  expected = {"Sean", "Felix"};
  REQUIRE(actual.size() == expected.size());
  for (const auto& str : expected) {
    REQUIRE(std::find(actual.begin(), actual.end(), str) != actual.end());
  }

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Felix") != namespaces.end());
    REQUIRE(kv.ns_exists("Felix"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Felix");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Felix", "Serena"));
    REQUIRE(kv.type("Felix", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom"};
      vec_opt = kv.lmembers("Felix", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Felix", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Felix", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie", "Serena"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Felix", "Sean"};
      vec_opt = kv.lmembers("Yuna", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Yuna", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Yuna", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.lset("Heyi", "Serena", 0, "Jeremie") == 1);














































































































































































































































  REQUIRE(kv.rpop("Felix", "Serena") == "Tom");

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 9);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie", "Serena"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Felix", "Sean"};
      vec_opt = kv.lmembers("Yuna", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Yuna", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Yuna", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.lpush("Heyi", "Serena", "Arvind") == 1);




























































































































































































































  REQUIRE(kv.lset("Jeremie", "Lang", 0U, "CV") == false);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 9);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie", "Serena"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Felix", "Sean"};
      vec_opt = kv.lmembers("Yuna", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Yuna", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Yuna", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.lpush("Kyrie", "Harry", "Kyrie") == 1);














































































































































































































































  vec_opt = kv.ldiff("Lang", "Tom", "Kyrie", "Tom");
  REQUIRE(vec_opt.has_value());
  actual = vec_opt.value();
  expected = {"Harry", "Felix", "Kyrie", "Kevin"};
  REQUIRE(actual.size() == expected.size());
  for (const auto& str : expected) {
    REQUIRE(std::find(actual.begin(), actual.end(), str) != actual.end());
  }

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie", "Serena"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Felix", "Sean"};
      vec_opt = kv.lmembers("Yuna", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Yuna", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Yuna", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Tom"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.lset("Sean", "Eric", 0, "Lang") == 1);














































































































































































































































  REQUIRE(kv.del("Heyi", "Harry") == 0);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie", "Serena"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Felix", "Sean"};
      vec_opt = kv.lmembers("Yuna", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Yuna", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Yuna", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.lpush("CV", "Travis", "Sean") == 1);
































































































































































































































































  REQUIRE(kv.del("Arvind", "Harry") == 0);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 11);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "CV") != namespaces.end());
    REQUIRE(kv.ns_exists("CV"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("CV");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("CV", "Travis"));
    REQUIRE(kv.type("CV", "Travis") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Sean"};
      vec_opt = kv.lmembers("CV", "Travis");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("CV", "Travis")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("CV", "Travis", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie", "Serena"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Felix", "Sean"};
      vec_opt = kv.lmembers("Yuna", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Yuna", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Yuna", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.lset("Kevin", "Jeremie", 0U, "CV") == false);
































































































































































































































































  kv.sset("Yuna", "Serena", "Felix");

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 11);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "CV") != namespaces.end());
    REQUIRE(kv.ns_exists("CV"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("CV");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("CV", "Travis"));
    REQUIRE(kv.type("CV", "Travis") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Sean"};
      vec_opt = kv.lmembers("CV", "Travis");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("CV", "Travis")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("CV", "Travis", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie", "Serena"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.rpop("Travis", "Serena") == "Travis");
























































































































































































































































  REQUIRE(kv.ns_exists("Serena") == 1);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 11);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "CV") != namespaces.end());
    REQUIRE(kv.ns_exists("CV"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("CV");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("CV", "Travis"));
    REQUIRE(kv.type("CV", "Travis") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Sean"};
      vec_opt = kv.lmembers("CV", "Travis");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("CV", "Travis")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("CV", "Travis", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie", "Serena"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.lpop("Serena", "Sean") == nullopt);
























































































































































































































































  REQUIRE(kv.rpop("CV", "Travis") == "Sean");

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie", "Serena"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.rpush("Sean", "Tom", "Lang") == 1);






































































































































































































































  REQUIRE(kv.rpush("Tom", "Serena", "Kevin") == 1);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie", "Serena"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV", "Kevin"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.rpush("Travis", "Serena", "Kevin") == 1);






































































































































































































































  REQUIRE(kv.rpush("Lang", "CV", "Sean") == 1);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie", "Serena", "Sean"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"CV", "Kevin"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.lset("Tom", "Serena", 2, "Serena") == false);
  REQUIRE(kv.lset("Tom", "Serena", 0, "Serena") == 1);






































































































































































































































  vec_opt = kv.ldiff("Sean", "Tom", "Kyrie", "Harry");
  REQUIRE(vec_opt.has_value());
  actual = vec_opt.value();
  expected = {"Lang", "Arvind", "Heyi"};
  REQUIRE(actual.size() == expected.size());
  for (const auto& str : expected) {
    REQUIRE(std::find(actual.begin(), actual.end(), str) != actual.end());
  }

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie", "Serena", "Sean"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Serena", "Kevin"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.lunion("Serena", "Sean", "Tom", "Sean") == nullopt);






































































































































































































































  REQUIRE(kv.ns_exists("Serena") == 1);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Kyrie", "Serena", "Sean"};
      vec_opt = kv.lmembers("Lang", "CV");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "CV")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "CV", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Serena", "Kevin"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  kv.sset("Lang", "CV", "Tom");






























































































































































































































  REQUIRE(kv.rpop("Harry", "Travis") == nullopt);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Serena", "Kevin"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.lindex("Serena", "Sean", 0U) == nullopt);






























































































































































































































  REQUIRE(kv.rpush("Lang", "Tom", "Kyrie") == 1);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Serena", "Kevin"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.rpush("Yuna", "Serena", "Arvind") == 0);






























































































































































































































  REQUIRE(kv.lindex("Sean", "Serena", 0) == "Heyi");

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Serena", "Kevin"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.rpush("Sean", "Eric", "Jeremie") == 1);






























































































































































































































  vec_opt = kv.ldiff("Travis", "Serena", "Serena", "Serena");
  REQUIRE(vec_opt.has_value());
  actual = vec_opt.value();
  expected = {"Kevin", "Heyi"};
  REQUIRE(actual.size() == expected.size());
  for (const auto& str : expected) {
    REQUIRE(std::find(actual.begin(), actual.end(), str) != actual.end());
  }

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Serena", "Kevin"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Jeremie"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.lpush("Serena", "Sean", "Jeremie") == 0);






























































































































































































































  vec_opt = kv.linter("Travis", "Serena", "Tom", "Serena");
  REQUIRE(vec_opt.has_value());
  actual = vec_opt.value();
  expected = {"Kevin"};
  REQUIRE(actual.size() == expected.size());
  for (const auto& str : expected) {
    REQUIRE(std::find(actual.begin(), actual.end(), str) != actual.end());
  }

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kevin") != keys.end());
    REQUIRE(kv.key_exists("Harry", "Kevin"));
    REQUIRE(kv.type("Harry", "Kevin") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Eric"};
      vec_opt = kv.lmembers("Harry", "Kevin");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Harry", "Kevin")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Harry", "Kevin", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Serena", "Kevin"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Jeremie"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.key_exists("Harry", "CV") == 1);






























































































































































































































  REQUIRE(kv.rpop("Harry", "Kevin") == "Eric");

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Serena", "Kevin"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Jeremie"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.lmembers("Kevin", "Jeremie") == nullopt);
















































































































































































































  REQUIRE(kv.key_exists("Lang", "CV") == 1);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Serena", "Kevin"};
      vec_opt = kv.lmembers("Tom", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Tom", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Tom", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Jeremie"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  kv.sset("Tom", "Serena", "Heyi");








































































































































































































  REQUIRE(kv.rpush("Tom", "Serena", "Yuna") == 0);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::string);
    str_opt = kv.sget("Tom", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Heyi");
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Jeremie"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.ns_exists("Eric") == 0);








































































































































































































  REQUIRE(kv.rpush("Kevin", "Jeremie", "Travis") == 0);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::string);
    str_opt = kv.sget("Tom", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Heyi");
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Jeremie"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.lpop("Serena", "Sean") == nullopt);








































































































































































































  kv.sset("Tom", "Serena", "Kevin");

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::string);
    str_opt = kv.sget("Tom", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Kevin");
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Jeremie"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.type("Lang", "CV") == value_type_info::string);








































































































































































































  REQUIRE(kv.lset("Lang", "CV", 0U, "Lang") == false);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::string);
    str_opt = kv.sget("Tom", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Kevin");
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Jeremie"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.ldiff("Harry", "CV", "Heyi", "CV") == nullopt);








































































































































































































  REQUIRE(kv.rpush("Yuna", "Serena", "Jeremie") == 0);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::string);
    str_opt = kv.sget("Tom", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Kevin");
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Jeremie"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.rpush("Travis", "Serena", "Travis") == 1);








































































































































































































  REQUIRE(kv.rpush("Serena", "Arvind", "Sean") == 1);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Arvind") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Arvind"));
    REQUIRE(kv.type("Serena", "Arvind") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Sean"};
      vec_opt = kv.lmembers("Serena", "Arvind");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Serena", "Arvind")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Serena", "Arvind", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::string);
    str_opt = kv.sget("Tom", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Kevin");
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Jeremie"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);






















































































































































































































  vec_opt = kv.ldiff("Kyrie", "Harry", "Travis", "Harry");
  REQUIRE(vec_opt.has_value());
  actual = vec_opt.value();
  expected = {"Kyrie"};
  REQUIRE(actual.size() == expected.size());
  for (const auto& str : expected) {
    REQUIRE(std::find(actual.begin(), actual.end(), str) != actual.end());
  }

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Arvind") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Arvind"));
    REQUIRE(kv.type("Serena", "Arvind") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Sean"};
      vec_opt = kv.lmembers("Serena", "Arvind");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Serena", "Arvind")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Serena", "Arvind", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::string);
    str_opt = kv.sget("Tom", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Kevin");
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Jeremie"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

  REQUIRE(kv.rpush("Kyrie", "Harry", "Sean") == 1);






















































































































































































































  REQUIRE(kv.lset("Sean", "Serena", 0, "Felix") == 1);

  // check the full state of the kv
  {
    const vector<string> namespaces = kv.namespaces();
    vector<string> keys = {};
    REQUIRE(namespaces.size() == 10);
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kyrie") != namespaces.end());
    REQUIRE(kv.ns_exists("Kyrie"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Harry") != namespaces.end());
    REQUIRE(kv.ns_exists("Harry"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Heyi") != namespaces.end());
    REQUIRE(kv.ns_exists("Heyi"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Travis") != namespaces.end());
    REQUIRE(kv.ns_exists("Travis"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Lang") != namespaces.end());
    REQUIRE(kv.ns_exists("Lang"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Serena") != namespaces.end());
    REQUIRE(kv.ns_exists("Serena"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Yuna") != namespaces.end());
    REQUIRE(kv.ns_exists("Yuna"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Tom") != namespaces.end());
    REQUIRE(kv.ns_exists("Tom"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Sean") != namespaces.end());
    REQUIRE(kv.ns_exists("Sean"));
    REQUIRE(std::find(namespaces.begin(), namespaces.end(), "Kevin") != namespaces.end());
    REQUIRE(kv.ns_exists("Kevin"));
    keys = kv.keys("Kyrie");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Kyrie", "Harry"));
    REQUIRE(kv.type("Kyrie", "Harry") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kyrie", "Sean"};
      vec_opt = kv.lmembers("Kyrie", "Harry");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Kyrie", "Harry")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Kyrie", "Harry", i) == expected.at(i));
      }
    }
    keys = kv.keys("Harry");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Harry", "CV"));
    REQUIRE(kv.type("Harry", "CV") == value_type_info::string);
    str_opt = kv.sget("Harry", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Eric");
    keys = kv.keys("Heyi");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Kyrie") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Kyrie"));
    REQUIRE(kv.type("Heyi", "Kyrie") == value_type_info::string);
    str_opt = kv.sget("Heyi", "Kyrie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Arvind");
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Heyi", "Serena"));
    REQUIRE(kv.type("Heyi", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Arvind", "Jeremie", "Harry", "Tom"};
      vec_opt = kv.lmembers("Heyi", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Heyi", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Heyi", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Travis");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Travis", "Serena"));
    REQUIRE(kv.type("Travis", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Kevin", "Travis"};
      vec_opt = kv.lmembers("Travis", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Travis", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Travis", "Serena", i) == expected.at(i));
      }
    }
    keys = kv.keys("Lang");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "CV") != keys.end());
    REQUIRE(kv.key_exists("Lang", "CV"));
    REQUIRE(kv.type("Lang", "CV") == value_type_info::string);
    str_opt = kv.sget("Lang", "CV");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Tom");
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Lang", "Tom"));
    REQUIRE(kv.type("Lang", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Kevin", "Kyrie", "Felix", "Harry", "Kyrie"};
      vec_opt = kv.lmembers("Lang", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Lang", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Lang", "Tom", i) == expected.at(i));
      }
    }
    keys = kv.keys("Serena");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Arvind") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Arvind"));
    REQUIRE(kv.type("Serena", "Arvind") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Sean"};
      vec_opt = kv.lmembers("Serena", "Arvind");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Serena", "Arvind")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Serena", "Arvind", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Sean") != keys.end());
    REQUIRE(kv.key_exists("Serena", "Sean"));
    REQUIRE(kv.type("Serena", "Sean") == value_type_info::string);
    str_opt = kv.sget("Serena", "Sean");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
    keys = kv.keys("Yuna");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Yuna", "Serena"));
    REQUIRE(kv.type("Yuna", "Serena") == value_type_info::string);
    str_opt = kv.sget("Yuna", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Felix");
    keys = kv.keys("Tom");
    REQUIRE(keys.size() == 2);
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Serena"));
    REQUIRE(kv.type("Tom", "Serena") == value_type_info::string);
    str_opt = kv.sget("Tom", "Serena");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Kevin");
    REQUIRE(std::find(keys.begin(), keys.end(), "Travis") != keys.end());
    REQUIRE(kv.key_exists("Tom", "Travis"));
    REQUIRE(kv.type("Tom", "Travis") == value_type_info::string);
    str_opt = kv.sget("Tom", "Travis");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Jeremie");
    keys = kv.keys("Sean");
    REQUIRE(keys.size() == 4);
    REQUIRE(std::find(keys.begin(), keys.end(), "Eric") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Eric"));
    REQUIRE(kv.type("Sean", "Eric") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Lang", "Jeremie"};
      vec_opt = kv.lmembers("Sean", "Eric");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Eric")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Eric", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Serena") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Serena"));
    REQUIRE(kv.type("Sean", "Serena") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Felix"};
      vec_opt = kv.lmembers("Sean", "Serena");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Serena")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Serena", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Tom") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Tom"));
    REQUIRE(kv.type("Sean", "Tom") == value_type_info::list);
    // check the vector
    {
      const vector<string> expected = {"Heyi", "Arvind", "Lang"};
      vec_opt = kv.lmembers("Sean", "Tom");
      REQUIRE(vec_opt.has_value());
      REQUIRE(vec_opt.value() == expected);
      REQUIRE(static_cast<size_t>(kv.llen("Sean", "Tom")) == expected.size());
      for (size_t i = 0U; i < expected.size(); i++) {
        REQUIRE(kv.lindex("Sean", "Tom", i) == expected.at(i));
      }
    }
    REQUIRE(std::find(keys.begin(), keys.end(), "Harry") != keys.end());
    REQUIRE(kv.key_exists("Sean", "Harry"));
    REQUIRE(kv.type("Sean", "Harry") == value_type_info::string);
    str_opt = kv.sget("Sean", "Harry");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Serena");
    keys = kv.keys("Kevin");
    REQUIRE(keys.size() == 1);
    REQUIRE(std::find(keys.begin(), keys.end(), "Jeremie") != keys.end());
    REQUIRE(kv.key_exists("Kevin", "Jeremie"));
    REQUIRE(kv.type("Kevin", "Jeremie") == value_type_info::string);
    str_opt = kv.sget("Kevin", "Jeremie");
    REQUIRE(str_opt.has_value());
    REQUIRE(str_opt.value() == "Lang");
  }

}
