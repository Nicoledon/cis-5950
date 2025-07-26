#include "./hash_table.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

// provided for you
// just hashes the string to get a hash code and then
// modulo's by the number of buckets to decide which bucket
// it should go into
size_t hash_table::key_to_bucket_num(const string &key) {
  return hash<string>{}(key) % (this->buckets_.size());
}

/////////////////////////////////////////////////////////////////////////////
// Part 1a: Basic Accessor functions
/////////////////////////////////////////////////////////////////////////////

string &hash_table::operator[](const string &key) {
  // you should keep this code.
  // Once you implement load_factor() and rehash()
  // this resizes the hash table when tehre are too many elements
  if (this->load_factor() >= hash_table::MAX_LOAD_FACTOR) {
    this->rehash(this->buckets_.size() * 2);
  }

  // calculate which bucket this key belongs to
  size_t bucket_num = key_to_bucket_num(key);
  // get the corresponding bucket
  auto &bucket = this->buckets_.at(bucket_num);
  for (auto &item : bucket) {
    if (key == item.first) {
      return item.second;
    }
  }
  std::string ch = "";
  bucket.emplace_back(key, ch);
  this->size_ += 1;
  return bucket.back().second;
  // TODO find the key if it exists
  // and either return the corresponding value
  // or perform the insertion
}

size_t hash_table::size() const { return this->size_; }
std::string &hash_table::at(const std::string &key) {
  if (this->load_factor() >= hash_table::MAX_LOAD_FACTOR) {
    this->rehash(this->buckets_.size() * 2);
  }

  // calculate which bucket this key belongs to
  size_t bucket_num = key_to_bucket_num(key);
  // get the corresponding bucket
  auto &bucket = this->buckets_.at(bucket_num);
  for (auto &item : bucket) {
    if (key == item.first) {
      return item.second;
    }
  }
  throw out_of_range{"out of range"};
}
bool hash_table::contains(const std::string &key) {
  size_t bucket_num = key_to_bucket_num(key);
  auto &bucket = this->buckets_.at(bucket_num);
  for (auto item : bucket) {
    if (key == item.first) {
      return true;
    }
  }
  return false;
}
bool hash_table::erase(std::string &key) {
  if (!contains(key)) {
    return false;
  }
  size_t bucket_num = key_to_bucket_num(key);
  auto &bucket = this->buckets_.at(bucket_num);
  int count = 0;
  for (const auto &elem : bucket) {
    if (elem.first == key) {
      break;
    }
    count += 1;
  }
  auto it = bucket.begin();
  std::advance(it, count);
  bucket.erase(it);
  this->size_ -= 1;

  return true;
}
// TODO: implement the remaining hash table functions

/////////////////////////////////////////////////////////////////////////////
// Part 1b: Resising/rehashing functions
/////////////////////////////////////////////////////////////////////////////

float hash_table::load_factor() {
  // you may find static_cast<float> useful to convert size_t
  // or other types to float. For example:
  //
  // size_t x = 5;
  // float y = static_cast<float>(x);
  size_t size = this->size();
  size_t bu = this->buckets_.size();
  float x = static_cast<float>(size);
  float y = static_cast<float>(bu);
  return x / y;
}
void hash_table::rehash(size_t count) {
  if (count > this->buckets_.size()) {
    std::vector<std::list<kv_pair>> new_hash_table(count);
    for (const auto &bucket : this->buckets_) {
      for (const auto &item : bucket) {
        auto index = hash<string>{}(item.first) % (count);
        new_hash_table[index].push_back({item.first, item.second});
      }
    }
    this->buckets_ = new_hash_table;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Part 2: Implementing the iterator.
// Only need to implement:
// - iterator::operator++()  "next"
// - iterator::opeartor*()   "get"
/////////////////////////////////////////////////////////////////////////////

hash_table::iterator &hash_table::iterator::operator++() {
  auto it = ht_.buckets_[bucket_num_].end();

  if (++list_iter_ == it) {
    for (auto i = bucket_num_ + 1; i < ht_.buckets_.size(); i++) {
      if (!ht_.buckets_[i].empty()) {
        list_iter_ = ht_.buckets_[i].begin();
        bucket_num_ = i;
        break;
      }
      if (i == ht_.buckets_.size() - 1) {
        bucket_num_ = ht_.buckets_.size() - 1;
        list_iter_ = ht_.buckets_[bucket_num_].end();
        return *this;
      }
    }
  }
  return *this;
}

kv_pair &hash_table::iterator::operator*() {
  auto it = ht_.buckets_[ht_.buckets_.size() - 1].end();
  if (it == this->list_iter_) {
    throw out_of_range{"out of range"};
  }
  auto &elem = *(this->list_iter_);
  return elem;
}

// provded constructor
hash_table::iterator::iterator(hash_table &table) : ht_(table) {
  if (ht_.buckets_.empty()) {
    throw invalid_argument(
        "Trying to construct an iterator from an invalid hash table");
  }

  bucket_num_ = 0;
  for (auto &bucket : ht_.buckets_) {
    if (!bucket.empty()) {
      list_iter_ = bucket.begin();
      break;
    }
    bucket_num_ += 1;
  }

  if (bucket_num_ == ht_.buckets_.size()) {
    bucket_num_ -= 1;
    list_iter_ = ht_.buckets_.back().begin();
  }
}

// provded function
hash_table::iterator hash_table::begin() { return {*this}; }

// provded function
hash_table::iterator hash_table::end() {
  hash_table::iterator res(*this);

  res.bucket_num_ = this->buckets_.size() - 1;

  res.list_iter_ = this->buckets_.at(res.bucket_num_).end();
  return res;
}

// provded function
bool operator==(const hash_table::iterator &lhs,
                const hash_table::iterator &rhs) {
  if (&lhs.ht_ != &rhs.ht_) {
    throw invalid_argument(
        "Cannot compare iterators to two separate hash tables");
  }

  if (lhs.bucket_num_ != rhs.bucket_num_) {
    return false;
  }

  return lhs.list_iter_ == rhs.list_iter_;
}
