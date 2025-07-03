#include "catch.hpp"
#include "SimpleKV.hpp"
#include <vector>
#include <string>

using simplekv::SimpleKV;
using simplekv::value_type_info;
using namespace std;

TEST_CASE("Construction & Destruction", "[basic]") {
  auto kv = new SimpleKV();
  REQUIRE(kv != nullptr);
  delete kv;
}

TEST_CASE("Adding a string value", "[basic]") {
  SimpleKV kv = {};
  kv.sset("sample namespace", "sample key", "sample value");

  vector<string> expected_namespaces {"sample namespace"};
  vector<string> expected_keys {"sample key"};

  REQUIRE(kv.namespaces() == expected_namespaces);
  REQUIRE(kv.keys("sample namespace") == expected_keys);

  optional<string> value = kv.sget("sample namespace", "sample key");
  REQUIRE(value.has_value());
  REQUIRE(value.value() == "sample value");

  REQUIRE(kv.ns_exists("sample namespace"));
  REQUIRE(kv.key_exists("sample namespace", "sample key"));
}

TEST_CASE("String Set", "[basic]") {
  SimpleKV kv = {};

  kv.sset("sample_ns", "k1", "v1");

  std::vector<std::string> expected_ns = {"sample_ns"};
  std::vector<std::string> expected_keys = {"k1"};
  REQUIRE(kv.namespaces() == expected_ns);
  REQUIRE(kv.keys("sample_ns") == expected_keys);

  REQUIRE(kv.ns_exists("sample_ns"));
  REQUIRE(kv.key_exists("sample_ns", "k1"));
  REQUIRE(kv.type("sample_ns", "k1") == simplekv::value_type_info::string);
  REQUIRE(kv.sget("sample_ns", "k1") == "v1");

}


TEST_CASE("Adding and deleteing a string value", "[basic]") {
  SimpleKV kv = {};
  kv.sset("sample namespace", "sample key", "sample value");

  vector<string> expected_namespaces {"sample namespace"};
  vector<string> expected_keys {"sample key"};

  REQUIRE(kv.namespaces() == expected_namespaces);
  REQUIRE(kv.keys("sample namespace") == expected_keys);

  optional<string> get_opt = kv.sget("sample namespace", "sample key");
  REQUIRE(get_opt.has_value());
  REQUIRE(get_opt.value() == "sample value");

  REQUIRE(kv.ns_exists("sample namespace"));
  REQUIRE(kv.key_exists("sample namespace", "sample key"));

  REQUIRE(kv.del("sample namespace", "sample key"));

  expected_namespaces = {};
  expected_keys = {};
  REQUIRE(kv.namespaces() == expected_namespaces);
  REQUIRE(kv.keys("sample namespace") == expected_keys);

  REQUIRE_FALSE(kv.ns_exists("sample namespace"));
  REQUIRE_FALSE(kv.key_exists("sample namespace", "sample key"));

  REQUIRE(kv.namespaces() == expected_namespaces);
  REQUIRE(kv.keys("sample namespace") == expected_keys);

  get_opt = kv.sget("sample namespace", "sample key");
  REQUIRE_FALSE(get_opt.has_value());

  REQUIRE(kv.namespaces() == expected_namespaces);
  REQUIRE(kv.keys("sample namespace") == expected_keys);

  REQUIRE_FALSE(kv.ns_exists("sample namespace"));
  REQUIRE_FALSE(kv.key_exists("sample namespace", "sample key"));

  REQUIRE(kv.namespaces() == expected_namespaces);
  REQUIRE(kv.keys("sample namespace") == expected_keys);
}

TEST_CASE("Basic Stress", "[basic]") {
  SimpleKV kv{};
  vector<string> expected_namespaces {};
  vector<string> expected_keys {};

  for (int i = 0; i < 25; i++) {
    string nspace {"ns"};
    nspace += to_string(i % 5);

    string key {"k"};
    key += to_string(i);

    string value {to_string(i)};
    kv.sset(nspace, key, value);

    string value2 {value + "U"};
    kv.sset(nspace, key, value2);

    REQUIRE(kv.ns_exists(nspace));
    REQUIRE(kv.key_exists(nspace, key));

    auto opt = kv.sget(nspace, key);
    REQUIRE(opt.has_value());
    REQUIRE(opt.value() == value2);
  
    string fake_key = "f" + key; 
    opt = kv.sget(nspace, fake_key);
    REQUIRE_FALSE(opt.has_value());
    REQUIRE_FALSE(kv.del(nspace, fake_key));

    REQUIRE(kv.del(nspace, key));
    auto actual_names = kv.namespaces();
    REQUIRE(actual_names.size() == expected_namespaces.size());
    for (const auto& name : expected_namespaces) {
      REQUIRE(find(actual_names.begin(), actual_names.end(), name) != actual_names.end());
    }


    auto keys = kv.keys(nspace);
    for (int j = i % 5; j < i; j += 5) {
      string expected_key = "k" + to_string(j);
      REQUIRE(find(keys.begin(), keys.end(), expected_key) != keys.end());
    }

    kv.sset(nspace, key, value2);
    kv.sset(nspace, key, value2);
    opt = kv.sget(nspace, key);
    REQUIRE(opt.has_value());
    REQUIRE(opt.value() == value2);

    if (i < 5) {
      expected_namespaces.push_back(nspace);
    }
  }
}

TEST_CASE("Pushing onto a list", "[basic list]") {
  SimpleKV kv = {};
  REQUIRE(kv.lpush("gy!be", "lysflah", "static"));
  REQUIRE(kv.lpush("gy!be", "lysflah", "storm"));
  REQUIRE(kv.rpush("gy!be", "lysflah", "sleep"));
  REQUIRE(kv.rpush("gy!be", "lysflah", "Like Antennas to Heaven"));

  REQUIRE(kv.lpush("gy!be", "Slow Riot for New Zero Kanada", "bbf3"));
  REQUIRE(kv.lpush("gy!be", "Slow Riot for New Zero Kanada", "moya"));

  REQUIRE(kv.lpush("gy!be", "F#A#Infinity", "Providence"));
  REQUIRE(kv.lpush("gy!be", "F#A#Infinity", "East Hastings"));
  REQUIRE(kv.lpush("gy!be", "F#A#Infinity", "Dead Flag Blues"));

  REQUIRE(kv.lpush("DOOM", "madvillainy", "Rhinestone Cowboy"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "All Caps"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "All Caps"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "Fancy Clown"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "Fancy Clown"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "Strange Ways"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "Curls"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "America's Most Blunted"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "Fancy Clown"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "Bistro"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "All Caps"));

  REQUIRE(kv.lpush("DOOM", "Mm..Food", "One Beer"));
  REQUIRE(kv.rpush("DOOM", "Mm..Food", "Rapp Snitch Knishes"));
  REQUIRE(kv.rpush("DOOM", "Mm..Food", "Deep Fried Frenz"));
  REQUIRE(kv.rpush("DOOM", "Mm..Food", "Kookies"));
  REQUIRE(kv.rpush("DOOM", "Mm..Food", "Rapp Snitch Knishes"));
  REQUIRE(kv.lpush("DOOM", "Mm..Food", "One Beer"));

  auto names = kv.namespaces();
  REQUIRE(names.size() == 2);
  REQUIRE(find(names.begin(), names.end(), "gy!be") != names.end());
  REQUIRE(find(names.begin(), names.end(), "DOOM") != names.end());

  auto keys = kv.keys("gy!be");
  REQUIRE(keys.size() == 3);
  REQUIRE(find(keys.begin(), keys.end(), "lysflah") != keys.end());
  REQUIRE(find(keys.begin(), keys.end(), "Slow Riot for New Zero Kanada") != keys.end());
  REQUIRE(find(keys.begin(), keys.end(), "F#A#Infinity") != keys.end());

  vector<string> expected {"storm", "static", "sleep", "Like Antennas to Heaven"};
  auto opt = kv.lmembers("gy!be", "lysflah");
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == expected);
  REQUIRE(kv.llen("gy!be", "lysflah") == 4);

  expected = {"moya", "bbf3"};
  opt = kv.lmembers("gy!be", "Slow Riot for New Zero Kanada");
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == expected);
  REQUIRE(kv.llen("gy!be", "Slow Riot for New Zero Kanada") == 2);

  expected = {"Dead Flag Blues", "East Hastings", "Providence"};
  opt = kv.lmembers("gy!be", "F#A#Infinity");
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == expected);
  REQUIRE(kv.llen("gy!be", "F#A#Infinity") == 3);

  expected = {"All Caps", "Bistro", "Fancy Clown", "America's Most Blunted", "Curls", "Strange Ways", "Fancy Clown", "Fancy Clown", "All Caps", "All Caps", "Rhinestone Cowboy"};
  opt = kv.lmembers("DOOM", "madvillainy");
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == expected);
  REQUIRE(kv.llen("DOOM", "madvillainy") == 11);

  expected = {"One Beer", "One Beer", "Rapp Snitch Knishes", "Deep Fried Frenz", "Kookies", "Rapp Snitch Knishes"};
  opt = kv.lmembers("DOOM", "Mm..Food");
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == expected);
  REQUIRE(kv.llen("DOOM", "Mm..Food") == 6);
}

TEST_CASE("Pushing Popping and Indexing into a list", "[basic list]") {
  SimpleKV kv = {};
  REQUIRE(kv.lpush("gy!be", "lysflath", "static"));
  REQUIRE(kv.lpush("gy!be", "lysflath", "storm"));
  REQUIRE(kv.rpush("gy!be", "lysflath", "sleep"));
  REQUIRE(kv.rpush("gy!be", "lysflath", "Like Antennas to Heaven"));

  REQUIRE(kv.lpush("gy!be", "Slow Riot for New Zero Kanada", "bbf3"));
  REQUIRE(kv.lpush("gy!be", "Slow Riot for New Zero Kanada", "moya"));

  REQUIRE(kv.lpush("gy!be", "F#A#Infinity", "Providence"));
  REQUIRE(kv.lpush("gy!be", "F#A#Infinity", "East Hastings"));
  REQUIRE(kv.lpush("gy!be", "F#A#Infinity", "Dead Flag Blues"));

  REQUIRE(kv.lpush("DOOM", "madvillainy", "Rhinestone Cowboy"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "All Caps"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "All Caps"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "Fancy Clown"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "Fancy Clown"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "Strange Ways"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "Curls"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "America's Most Blunted"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "Fancy Clown"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "Bistro"));
  REQUIRE(kv.lpush("DOOM", "madvillainy", "All Caps"));

  REQUIRE(kv.lpush("DOOM", "Mm..Food", "One Beer"));
  REQUIRE(kv.rpush("DOOM", "Mm..Food", "Rapp Snitch Knishes"));
  REQUIRE(kv.rpush("DOOM", "Mm..Food", "Deep Fried Frenz"));
  REQUIRE(kv.rpush("DOOM", "Mm..Food", "Kookies"));
  REQUIRE(kv.rpush("DOOM", "Mm..Food", "Rapp Snitch Knishes"));
  REQUIRE(kv.lpush("DOOM", "Mm..Food", "One Beer"));

  auto str_opt = kv.lpop("DOOM", "Mm..Food");
  REQUIRE(str_opt.has_value());
  REQUIRE(str_opt.value() == "One Beer");

  str_opt = kv.lpop("DOOM", "Mm..Food");
  REQUIRE(str_opt.has_value());
  REQUIRE(str_opt.value() == "One Beer");

  str_opt = kv.lpop("DOOM", "Mm..Food");
  REQUIRE(str_opt.has_value());
  REQUIRE(str_opt.value() == "Rapp Snitch Knishes");

  str_opt = kv.lpop("gy!be", "Slow Riot for New Zero Kanada");
  REQUIRE(str_opt.has_value());
  REQUIRE(str_opt.value() == "moya");

  str_opt = kv.rpop("gy!be", "Slow Riot for New Zero Kanada");
  REQUIRE(str_opt.has_value());
  REQUIRE(str_opt.value() == "bbf3");

  str_opt = kv.lindex("gy!be", "Slow Riot for New Zero Kanada", 0);
  REQUIRE_FALSE(str_opt.has_value());

  REQUIRE_FALSE(kv.lset("gy!be", "Slow Riot for New Zero Kanada", 0, "moya"));
  REQUIRE_FALSE(kv.lset("gy!be", "lysflath", 4, "moya"));
  REQUIRE_FALSE(kv.lset("gy!be", "lysflath", 10, "bbf3"));

  str_opt = kv.lindex("gy!be", "lysflath", 2);
  REQUIRE(str_opt.has_value()); 
  REQUIRE(str_opt.value() == "sleep");

  str_opt = kv.lindex("DOOM", "madvillainy", 11);
  REQUIRE_FALSE(str_opt.has_value());

  str_opt = kv.lindex("DOOM", "madvillainy", 10);
  REQUIRE(str_opt.has_value());
  REQUIRE(str_opt.value() == "Rhinestone Cowboy");

  str_opt = kv.lindex("DOOM", "lysflath", 11);
  REQUIRE_FALSE(str_opt.has_value());

  str_opt = kv.lindex("I'm sorry", "lysflath", 0);
  REQUIRE_FALSE(str_opt.has_value());

  REQUIRE_FALSE(kv.lset("the basement", "is cold", 2, "a"));
  REQUIRE_FALSE(kv.lset("the glow", "the shirt off", 0, "in the yard"));

  REQUIRE(kv.lset("gy!be", "lysflath", 2, "the moon"));
  REQUIRE(kv.lset("gy!be", "F#A#Infinity", 0, "the glow"));
  REQUIRE(kv.lset("DOOM", "Mm..Food", 2, "Vaudeville Villain"));

  auto names = kv.namespaces();
  REQUIRE(names.size() == 2);
  REQUIRE(find(names.begin(), names.end(), "gy!be") != names.end());
  REQUIRE(find(names.begin(), names.end(), "DOOM") != names.end());

  auto keys = kv.keys("gy!be");
  REQUIRE(keys.size() == 2);
  REQUIRE(find(keys.begin(), keys.end(), "lysflath") != keys.end());
  REQUIRE(find(keys.begin(), keys.end(), "F#A#Infinity") != keys.end());

  vector<string> expected {"storm", "static", "the moon", "Like Antennas to Heaven"};
  auto opt = kv.lmembers("gy!be", "lysflath");
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == expected);
  REQUIRE(kv.llen("gy!be", "lysflath") == 4);

  expected = {"the glow", "East Hastings", "Providence"};
  opt = kv.lmembers("gy!be", "F#A#Infinity");
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == expected);
  REQUIRE(kv.llen("gy!be", "F#A#Infinity") == 3);

  expected = {"All Caps", "Bistro", "Fancy Clown", "America's Most Blunted", "Curls", "Strange Ways", "Fancy Clown", "Fancy Clown", "All Caps", "All Caps", "Rhinestone Cowboy"};
  opt = kv.lmembers("DOOM", "madvillainy");
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == expected);
  REQUIRE(kv.llen("DOOM", "madvillainy") == 11);

  expected = {"Deep Fried Frenz", "Kookies", "Vaudeville Villain"};
  opt = kv.lmembers("DOOM", "Mm..Food");
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == expected);
  REQUIRE(kv.llen("DOOM", "Mm..Food") == 3);
}

// [complex list]
// kv.linter kv.ldiff kv.lunion

TEST_CASE("List union", "[complex list]") {
  SimpleKV kv {};

  string ns1 = "me";
  string ns2 = "./";

  string key1 = "././HK.hke";
  string key2 = "./Odyssey.home";

  string bananas = "bananas";
  string pears = "pears";
  string apples = "apples";
  string grapes = "grapes";

  kv.lpush(ns1, key1, bananas);
  kv.lpush(ns1, key1, bananas);
  kv.lpush(ns1, key1, pears);
  kv.lpush(ns1, key1, apples);
  kv.lpush(ns1, key1, apples);

  kv.rpush(ns1, key2, bananas);
  kv.sset(ns1, key2, bananas);

  kv.rpush(ns2, key1, bananas);
  kv.rpush(ns2, key1, grapes);

  kv.rpush(ns2, key2, apples);
  kv.rpush(ns2, key2, grapes);
  kv.rpush(ns2, key2, grapes);
  kv.rpush(ns2, key2, grapes);
  kv.rpush(ns2, key2, bananas);

  REQUIRE(kv.type("neither", key1) == value_type_info::none);
  REQUIRE(kv.type(ns1, "neither") == value_type_info::none);

  auto opt = kv.lunion(ns1, key1, ns1, key2);
  REQUIRE_FALSE(opt.has_value());
  opt = kv.lunion(ns1, key2, ns1, key2);
  REQUIRE_FALSE(opt.has_value());
  opt = kv.lunion(ns1, key2, ns1, key2);
  REQUIRE_FALSE(opt.has_value());
  opt = kv.lunion(ns2, key1, ns1, key2);
  REQUIRE_FALSE(opt.has_value());
  opt = kv.lunion(ns2, key2, ns1, key2);
  REQUIRE_FALSE(opt.has_value());

  opt = kv.linter(ns1, key1, ns1, key2);
  REQUIRE_FALSE(opt.has_value());
  opt = kv.linter(ns1, key2, ns1, key2);
  REQUIRE_FALSE(opt.has_value());
  opt = kv.linter(ns1, key2, ns1, key2);
  REQUIRE_FALSE(opt.has_value());
  opt = kv.linter(ns2, key1, ns1, key2);
  REQUIRE_FALSE(opt.has_value());
  opt = kv.linter(ns2, key2, ns1, key2);
  REQUIRE_FALSE(opt.has_value());

  opt = kv.ldiff(ns1, key1, ns1, key2);
  REQUIRE_FALSE(opt.has_value());
  opt = kv.ldiff(ns1, key2, ns1, key2);
  REQUIRE_FALSE(opt.has_value());
  opt = kv.ldiff(ns1, key2, ns1, key2);
  REQUIRE_FALSE(opt.has_value());
  opt = kv.ldiff(ns2, key1, ns1, key2);
  REQUIRE_FALSE(opt.has_value());
  opt = kv.ldiff(ns2, key2, ns1, key2);
  REQUIRE_FALSE(opt.has_value());

  // kv.lunion
  vector<string> expected {apples, bananas, grapes, pears};
  opt = kv.lunion(ns1, key1, ns2, key1);
  REQUIRE(opt.has_value());
  auto actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);
  opt = kv.lunion(ns2, key1, ns1, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  opt = kv.lunion(ns1, key1, ns2, key2);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);
  opt = kv.lunion(ns2, key2, ns1, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  expected = {apples, bananas, grapes};
  opt = kv.lunion(ns2, key2, ns2, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);
  opt = kv.lunion(ns2, key1, ns2, key2);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  expected = {apples, bananas, pears};
  opt = kv.lunion(ns1, key1, ns1, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  expected = {bananas, grapes};
  opt = kv.lunion(ns2, key1, ns2, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  expected = {apples, bananas, grapes};
  opt = kv.lunion(ns2, key2, ns2, key2);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // kv.linter
  expected = {bananas};
  opt = kv.linter(ns2, key1, ns1, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);
  opt = kv.linter(ns1, key1, ns2, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  expected = {apples, bananas};
  opt = kv.linter(ns2, key2, ns1, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);
  opt = kv.linter(ns1, key1, ns2, key2);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  expected = {bananas, grapes};
  opt = kv.linter(ns2, key2, ns2, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);
  opt = kv.linter(ns2, key1, ns2, key2);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  expected = {apples, bananas, pears};
  opt = kv.linter(ns1, key1, ns1, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  expected = {bananas, grapes};
  opt = kv.linter(ns2, key1, ns2, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  expected = {apples, bananas, grapes};
  opt = kv.linter(ns2, key2, ns2, key2);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // diff
  expected = {apples, pears};
  opt = kv.ldiff(ns1, key1, ns2, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  expected = {pears};
  opt = kv.ldiff(ns1, key1, ns2, key2);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // ba
  expected = {grapes};
  opt = kv.ldiff(ns2, key1, ns1, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // bc
  expected = {};
  opt = kv.ldiff(ns2, key1, ns2, key2);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // ca
  expected = {grapes};
  opt = kv.ldiff(ns2, key2, ns1, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  // cb
  expected = {apples};
  opt = kv.ldiff(ns2, key2, ns2, key1);
  REQUIRE(opt.has_value());
  actual = opt.value();
  sort(actual.begin(), actual.end());
  REQUIRE(actual == expected);

  auto names = kv.namespaces();
  REQUIRE(names.size() == 2);
  REQUIRE(find(names.begin(), names.end(), ns1) != names.end());
  REQUIRE(find(names.begin(), names.end(), ns2) != names.end());

  auto keys = kv.keys(ns1);
  REQUIRE(keys.size() == 2);
  REQUIRE(find(keys.begin(), keys.end(), key1) != keys.end());
  REQUIRE(find(keys.begin(), keys.end(), key2) != keys.end());

  keys = kv.keys(ns2);
  REQUIRE(keys.size() == 2);
  REQUIRE(find(keys.begin(), keys.end(), key1) != keys.end());
  REQUIRE(find(keys.begin(), keys.end(), key2) != keys.end());

  REQUIRE(kv.type(ns1, key1) == value_type_info::list);
  REQUIRE(kv.type(ns1, key2) == value_type_info::string);
  REQUIRE(kv.type(ns2, key1) == value_type_info::list);
  REQUIRE(kv.type(ns2, key2) == value_type_info::list);

  auto str_opt = kv.sget(ns1, key2);
  REQUIRE(str_opt.has_value());
  REQUIRE(str_opt.value() == bananas);

  expected = {apples, apples, pears, bananas, bananas};
  opt = kv.lmembers(ns1, key1);
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == expected);
  REQUIRE(kv.llen(ns1, key1) == 5);

  expected = {bananas, grapes};
  opt = kv.lmembers(ns2, key1);
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == expected);
  REQUIRE(kv.llen(ns2, key1) == 2);

  expected = {apples, grapes, grapes, grapes, bananas};
  opt = kv.lmembers(ns2, key2);
  REQUIRE(opt.has_value());
  REQUIRE(opt.value() == expected);
  REQUIRE(kv.llen(ns2, key2) == 5);
}
