#include "./catch.hpp"
#include "./hash_table.hpp"

#include <string>
#include <list>
#include <array>
#include <stdexcept>

using std::string;
using std::list;
using std::array;
using std::size_t;
using std::to_string;
using std::stoi;
using std::out_of_range;
using std::vector;
using std::hash;

struct ht_inspector {
  ht_inspector(hash_table& to_inspect) : buckets_(to_inspect.buckets_), size_(to_inspect.size_) { };

  vector<list<kv_pair>>& buckets_;
  std::size_t& size_;
  static constexpr float HT_MLF = hash_table::MAX_LOAD_FACTOR;
};

TEST_CASE("basic ctor", "[Test_HashTable]") {
  hash_table ht;
  ht_inspector hti(ht);
  REQUIRE(hti.size_ == 0);

  REQUIRE(hti.buckets_.size() == 13);
  for (size_t i = 0; i < 13; i++) {
    REQUIRE(hti.buckets_[i].empty());
  }
}


TEST_CASE("InsertFindRemove", "[Test_HashTable]") {
  hash_table ht;
  int i;

  // Allocate and insert a bunch of elements.
  for (i = 0; i < 13; i++) {
    //
    // create an element and do the insert
    //
    string key = to_string(i);
    string payload = to_string(i) + "a";

    // check that key did not exist before insertion
    REQUIRE(ht.size() == i);
    REQUIRE(ht[key] == "");
    REQUIRE(ht.size() == (i + 1));

    // check that insertion worked
    ht[key] = payload;
    REQUIRE(ht.size() == (i + 1));
    REQUIRE(ht[key] == payload);


    //
    // Test the double-insert case, using a different value.
    //
    payload = to_string(i) + "b";
    REQUIRE(ht.size() == (i + 1));

    REQUIRE(ht[key] == (to_string(i) + "a"));
    ht[key] = payload;

    REQUIRE(ht[key] == (to_string(i) + "b"));
    REQUIRE(ht.size() == (i + 1));

    //
    // Lookup the newly-inserted value.
    //
    REQUIRE(ht.at(key) == payload);
    REQUIRE(ht.contains(key));

    //
    // Lookup and remove a value that doesn't exist in the table.
    //
    string fake_key = to_string(i + 1);
    REQUIRE_THROWS_AS(ht.at(fake_key), out_of_range);
    REQUIRE_FALSE(ht.contains(fake_key));
    REQUIRE_FALSE(ht.erase(fake_key));

    // Remove the item we just inserted.
    REQUIRE(ht.erase(key));
    REQUIRE(ht.size() == i);
    REQUIRE_FALSE(ht.contains(key));
    REQUIRE_FALSE(ht.contains(key));
    REQUIRE_THROWS_AS(ht.at(key), out_of_range);

    // Insert it again.
    ht[key];
    REQUIRE(ht.at(key) == "");
    REQUIRE(ht.size() == (i + 1));
    ht[key] = payload;

    REQUIRE(ht.at(key) == payload);
    REQUIRE(ht[key] == payload);
    REQUIRE(ht.size() == (i + 1));
    REQUIRE(ht.contains(key));
  }

  // Delete every other key.
  for (i = 0; i < 13; i += 2) {
    string key = to_string(i);

    REQUIRE(ht.erase(key));
    REQUIRE(ht.size() == (13 - ((i/2) + 1)));
    REQUIRE_FALSE(ht.contains(key));
    REQUIRE_FALSE(ht.contains(key));
    REQUIRE_THROWS_AS(ht.at(key), out_of_range);

    // This second attempt fails, since the element was already removed.
    REQUIRE_FALSE(ht.erase(key));
    REQUIRE(ht.size() == (13 - ((i/2) + 1)));
    REQUIRE_FALSE(ht.contains(key));
    REQUIRE_FALSE(ht.contains(key));
    REQUIRE_THROWS_AS(ht.at(key), out_of_range);
  }
  REQUIRE(ht.size() == 6);

  // Delete the remaining keys.
  for (i = 1; i < 13; i += 2) {
    string key = to_string(i);

    REQUIRE(ht.erase(key));
    REQUIRE(ht.size() == (6 - ((i/2) + 1)));
    REQUIRE_FALSE(ht.contains(key));
    REQUIRE_FALSE(ht.contains(key));
    REQUIRE_THROWS_AS(ht.at(key), out_of_range);

    // As before, this second attempt should fail.
    REQUIRE_FALSE(ht.erase(key));
    REQUIRE(ht.size() == (6 - ((i/2) + 1)));
    REQUIRE_FALSE(ht.contains(key));
    REQUIRE_FALSE(ht.contains(key));
    REQUIRE_THROWS_AS(ht.at(key), out_of_range);
  }
  REQUIRE(ht.size() == 0);

  // One more pass, inserting elements.
  for (i = 0; i < 13; i++) {
    string key = to_string(i);
    string payload = to_string(i) + "a";

    // Do the insert.

    // check that key did not exist before insertion
    REQUIRE(ht.size() == i);
    REQUIRE(ht[key] == "");
    REQUIRE(ht.size() == (i + 1));

    // check that insertion worked
    ht[key] = payload;
    REQUIRE(ht.size() == (i + 1));
    REQUIRE(ht[key] == payload);

    // Ensure it's there.
    REQUIRE(ht.at(key) == payload);
    REQUIRE(ht.contains(key));
  }
  REQUIRE(ht.size() == 13);

  // Delete most of the remaining keys.
  for (i = 0; i < 11; i++) {
    string key = to_string(i);

    REQUIRE(ht.erase(key));
    REQUIRE(ht.size() == 13 - (i+1));
    REQUIRE_FALSE(ht.contains(key));
    REQUIRE_FALSE(ht.contains(key));
    REQUIRE_THROWS_AS(ht.at(key), out_of_range);
  }
  REQUIRE(ht.size() == 2);
}

TEST_CASE("Iterator", "[Test_HashTable]") {
  int i;
  hash_table table(300);

  // Test using an iterator on an empty table.
  hash_table::iterator it = table.begin();
  REQUIRE(it == table.end());

  // trying to get from an iteartor at end should throw an exception
  REQUIRE_THROWS_AS(*it, out_of_range);

  // Allocate and insert a bunch of elements, then create an iterator for
  // the populated table.
  for (i = 0; i < 100; i++) {
    string key = to_string(i);
    string payload = to_string(i + 1);

    // Create an element and do the insert.
    REQUIRE(table.size() == i);
    table[key] = payload;
    REQUIRE(table.size() == i + 1);
  }

  auto it2 = table.begin();
  REQUIRE_FALSE(it2 == table.end());

  // Now iterate through the table, verifying each value is found exactly once.
  array<int, 100> num_times_seen = {};   // array of 100 0's
  for (i = 0; i < 100; i++) {
    kv_pair kv;

    REQUIRE_FALSE(it2 == table.end());
    REQUIRE_NOTHROW(kv = *it2);

    int key;
    REQUIRE_NOTHROW(key = stoi(kv.first));

    // Verify that we've never seen this key before, then increment the
    // number of times we've seen it.
    REQUIRE(num_times_seen.at(key) == 0);
    REQUIRE_NOTHROW(num_times_seen.at(key)++);

    // Verify that this is the value we previously inserted.
    string expected = to_string(key + 1);
    REQUIRE(expected == kv.second);

    // Increment the iterator.
    if (i == 99) {
      REQUIRE_FALSE(it2 == table.end());
      REQUIRE_NOTHROW(++it2);
      REQUIRE(it2 == table.end());
    } else {
      REQUIRE_FALSE(it2 == table.end());
      REQUIRE_NOTHROW(++it2);
      REQUIRE_FALSE(it2 == table.end());
    }
  }
  for (i = 0; i < 100; i++) {
    REQUIRE(num_times_seen[i] == 1);  // verify each was seen exactly once.
  }

  // incrementing an iterator at the end should just give the end
  ++it2;
  REQUIRE(it2 == table.end());

  // remove every third element and resetting all
  // the "was seen" counters.
  for (i = 0; i < 100; i++) {
    num_times_seen[i] = 0;

    if (i % 3 == 0) {
      string key = to_string(i);
      REQUIRE(table.erase(key));
    }
  }

  // Iterate through one last time, making sure we only retain elements whose
  // key is NOT a multiple of 3.
  auto it3 = table.begin();
  REQUIRE_FALSE(it3 == table.end());

  REQUIRE(table.size() == 66);
  for (i = 0; i < 66; i++) {
    kv_pair kv;

    REQUIRE_FALSE(it3 == table.end());
    kv = *it3;

    int key = stoi(kv.first);

    // Verify that we've never seen this key before, then increment the
    // number of times we've seen it.
    REQUIRE(num_times_seen[key] == 0);
    num_times_seen[key]++;

    // Verify that this is the value we previously inserted.
    string expected = to_string(key + 1);
    REQUIRE(expected == kv.second);

    if (i == 65) {
      REQUIRE_FALSE(it3 == table.end());
      ++it3;
      REQUIRE(it3 == table.end());
    } else {
      REQUIRE_FALSE(it3 == table.end());
      ++it3;
      REQUIRE_FALSE(it3 == table.end());
    }
  }

  int total = 0;
  for (i = 0; i < 100; i++) {
    int count = num_times_seen[i];

    // verify each was seen once or not at all
    if (i % 3 == 0) {
      REQUIRE(count == 0);
    } else {
      REQUIRE(count == 1);
    }

    total += count;
  }

  REQUIRE(total == 66);
  REQUIRE(table.size() == 66);
}

TEST_CASE("Resize", "[Test_HashTable]") {
  hash_table table(2); // 2 buckets
  ht_inspector hti(table);

  // Add elements to the Table, expect the table to resize
  for (int i = 0; i < 7; ++i) {
    string key = to_string(i);
    string payload = to_string(i) + "ngnm";
    REQUIRE(table[key] == "");
    table.at(key) = payload;
    REQUIRE(table[key] == payload);
    REQUIRE(table.at(key) == payload);
    REQUIRE(table.contains(key));
  }

  REQUIRE(hti.buckets_.size() == 8);

  array<int, 7> counts = {};

  // check that each string was put in the correct bucket
  for (int bucket_num = 0; bucket_num < hti.buckets_.size(); bucket_num += 1) {
    auto& bucket = hti.buckets_.at(bucket_num);
    for (auto& pair : bucket) {
      int key = stoi(pair.first);
      counts.at(key) += 1;
      size_t hashcode = hash<string>{}(pair.first);
      size_t expected_bucket_num = hashcode % (hti.buckets_.size());
      REQUIRE(expected_bucket_num == bucket_num);
    }
  }

  // check that each key was seen exactly once
  for (int count : counts) {
    REQUIRE(count == 1);
  }

  // Make sure that all of the elements are still inside the
  // HashTable after Resizing, then ensure that num_buckets
  // stays the same.
  REQUIRE(table.size() == 7);

  for (int i = 0; i < 7; ++i) {
    string key = to_string(i);
    string expected = to_string(i) + "ngnm";

    REQUIRE(table.contains(key));
    string payload = table.at(key);
    REQUIRE(payload == expected);

    REQUIRE(table.erase(key));
    REQUIRE(table.size() == 6);

    REQUIRE(table[key] == "");

    table.at(key) = payload;

    REQUIRE(table[key] == payload);

    REQUIRE(table.at(key) == payload);
    REQUIRE(table.contains(key));
  }

  REQUIRE(table.size() == 7);

  // check again that each string was put in the correct bucket

  // Assert that the number of buckets has not changed
  REQUIRE(hti.buckets_.size() == 8);
}
