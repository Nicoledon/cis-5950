#ifndef HASHTABLE_HPP_
#define HASHTABLE_HPP_

#include <string>
#include <vector>
#include <utility> // for std::pair
#include <list>


// Define a "new type" kv_pair to make nagivating the map
// looks less messy
using kv_pair = std::pair<std::string, std::string>;

///////////////////////////////////////////////////////////////////////////////
// A hash_table is a automatically-resizing chained hash table.
//
// We provide the interface; your job is to provide the implementation.
//
// This table stores keys that are strings and values that are strings.
//
// As the load factor approaches 1, linked lists hanging off of each bucket
// will start to grow.  This implementation will dynamically resize the
// hashtable when the load factor exceeds 1.  It will multiply the number
// of buckets in the hashtable by 2, so that post-resize load factor is 1/2.
//
// More details on what a chaining hash table is can be found in the
// assignment specification.
class hash_table {
 public:
  /////////////////////////////////////////////////////////////////////////////
  // Part 1: Given Functions
  /////////////////////////////////////////////////////////////////////////////

  // construct a hash table with 13 empty buckets
  hash_table() : buckets_(13), size_(0) { };

  // construct a hash table with N empty buckets
  hash_table(size_t num_buckets) : buckets_(num_buckets), size_(0) { };

  // default destructor
  ~hash_table() = default;

  // default copy constructor and assignment operator
  hash_table(const hash_table& other) = default;
  hash_table& operator=(const hash_table& other) = default;

  // default move constructor and move assignment operator
  // you can ignore these
  hash_table(hash_table&& other) = default;
  hash_table& operator=(hash_table&& other) = default;

  // Given a key, calculate which bucket it should go into
  size_t key_to_bucket_num(const std::string& key);
  
  /////////////////////////////////////////////////////////////////////////////
  // Part 1a: Basic accessor functions
  /////////////////////////////////////////////////////////////////////////////

  // Returns a reference to the value that is mapped to the given key.
  //
  // If the key does not already exist, then it is inserted into the hash_table
  // with it's value set to be the default value for a string (the empty string).
  // After insertion, the reference to this newly inserted empty string will occur.
  // 
  // args:
  // - key: the key we want to lookup or insert in the hash_table
  // 
  // Returns:
  // - A reference to the exisitng value mapped to the key
  //   or
  // - A reference to the newly inserted empty string value mapped to the key.
  std::string& operator[](const std::string& key);

  // Returns a reference to the value that is mapped to the given key.
  // Unlike operator[], it throws an out_of_range exception when the key does
  // not already exist.
  //
  // args:
  // - key: the key we want to lookup in the hash_table
  //
  // throws:
  // - out_of_range exception when the key is not found
  // 
  // Returns:
  // - A reference to the exisitng value mapped to the key
  std::string& at(const std::string& key);

  // Checks to see if there is a kv pair in the hash_table with the given key.
  //
  /// args:
  // - key: the key we want to lookup in the hash_table
  //
  // Returns:
  // - true if the key already exists in the map, false otherwise.
  bool contains(const std::string& key);

  // Given a key, removes the corresponding kv pair from the map if it exists
  //
  /// args:
  // - key: the key we want to remove form the hash table
  //
  // Returns:
  // - true if the key already existed and thus was removed from the hashtable
  //   false if the key did not exist and thus no removal happened.
  bool erase(std::string& key);

  // Returns the number of key value pairs stored in the map.
  std::size_t size() const;

  /////////////////////////////////////////////////////////////////////////////
  // Part 1b: Resizing/rehashing functions
  /////////////////////////////////////////////////////////////////////////////

  // Returns the current load factor of the table. This is just the ratio
  // of number_of_elements / number_of_buckets.
  float load_factor();

  // Resizes the hashtable to have at least N buckets and redistributes
  // kv pairs across those buckets.
  //
  // If the specified count is less than or equal to the current number of
  // buckets, then this function does nothing.
  //
  // This function is automatically called by operator[] when the load factor
  // is greater than or equal to the MAX_LOAD_FACTOR (1)
  //
  /// args:
  // - key: the new number of buckets to store in the hash table.
  void rehash(std::size_t count);

  /////////////////////////////////////////////////////////////////////////////
  // Part 2: Implementing the iterator.
  // Only need to implement:
  // - iterator::operator++()  "next"
  // - iterator::opeartor*()   "get"
  /////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////
  // HashTable iterator
  //
  // HashTables support the notion of an iterator, similar to Java iterators.
  // You use an iterator to iterate forward through the hash_table. The order
  // in which the iterator goes through the hash_table is undefined, and not
  // necessarily deterministic; all that is promised is that each (key,value)
  // is visited exactly once.  Also, if the user uses a hash_table function
  // to mutate the hash table, any existing iterators become undefined (ie,
  // dangerous to use; arbitrary memory corruption can occur).
  class iterator {
   public:
    // default destructor and copy constructor
    ~iterator() = default;
    iterator(const iterator& other) = default;

    // default other things
    iterator(iterator&& other) = default;
    iterator& operator=(const iterator& other) = delete;
    iterator& operator=(iterator&& other) = delete;

    // Advances the iterator to the next kv_pair in the hash_table.
    // If the iterator is already at the end, then nothing is done and the
    // function simply returns
    //
    // returns:
    // - *this (a reference to the iterator this operator is called on)
    iterator& operator++();

    // Gets the current kv_pair that the iterator is currently at.
    //
    // Returns:
    // - the kv pair that the iterator is currently at.
    //
    // Throw:
    // - out_of_range if the iterator is already at the end of the table.
    kv_pair& operator*();

    // provided function to compare iterator objects
    // HINT: You probably want to use this function 
    //
    // throws an exception if the iterators refer to different hash_tables.
    friend bool operator==(const hash_table::iterator& lhs, const hash_table::iterator& rhs);

    // marke the hash_table as friend so that it
    // can access the constructor
    friend class hash_table;

   private:
    // private constructor: for our implementation users should get an iterator
    // by calling begin() or end() on a hash_table.
    iterator(hash_table& table);

    // reference to the hash table we are iterating over
    hash_table& ht_;
    
    // the index of which bucket we are currently iterating over
    std::size_t bucket_num_;
    
    // the iterator for the bucket we are currently in.
    std::list<kv_pair>::iterator list_iter_;
  };

  // provided function: returns an iterator to the beginning of the table
  // The beginning is "on" the first element it will iterate over.
  // begin() may be the same as end() if the hash_table is emtpy;
  iterator begin();

  // provided function: returns an iterator to the end of the table
  //
  // An end() iterator is after all the elements in the table,
  // Checking to see if the iterator is equal to the end is useful for:
  // - checking to see if we can move on to the next element
  // - similarly, if we should stop iterating over the container
  // - seeing if it is safe to dereference the iterator.
  iterator end();

  // let the iterator access the private fields of a hash_table
  friend class iterator;

  // let a struct that is used by the test_suite get access
  // to the data members of the hash_table.
  friend struct ht_inspector;

 private:
  // Fields: do not change these
  std::vector<std::list<kv_pair>> buckets_;
  std::size_t size_;

  // constant: the maximum load factor before
  // we would rehash the table.
  static constexpr float MAX_LOAD_FACTOR = 1.0F;
};

#endif  // HASHTABLE_HPP_
