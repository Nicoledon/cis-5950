#ifndef VEC_HPP
#define VEC_HPP
#include <cstddef>
#include <string>
#include<optional>
// vec is a class that represents
// an ordered list of strings as a dynamically resizable array.
class vec {
 public:
  // constructors:

  // 0-arg constructor.
  // Constructs an vec with initial capacity 10
  // The initial length will be 0 and the vector contains no initial elements
  vec();

  // 1-arg construcor
  // Constructs an vec with the specified initial capacity
  // The initial size will be 0 and the vector contains no initial elements
  vec(size_t initial_capacity);

  // copy constructor
  // Takes in another vector by const-reference
  // contructs a new vector as an independent copy of the
  // passed in vector
  vec(const vec& other);

  // Destructor
  // Deallocates any allocated resources
  ~vec();

  // Returns the current number of elements in the vector
  size_t length();

  // returns access to the underlying dynamically allocated array
  // that the vec object manages.
  std::string* data();

  // Returns the current capacity of the vector
  size_t capacity();

  // Requests that the vector capacity be at least enough to contain `new_cap`
  // elements.
  //
  // If `new_cap` is greater than the current vector capacity, the function causes the
  // container to reallocate its storage increasing its capacity to n (or greater).
  // 
  // In all other cases, the function call does not cause a reallocation and the
  // vector capacity is not affected. This function has no effect on the vector size
  // and cannot alter its elements.
  void reserve(size_t new_cap);

  // Adds a new elemnt to the end of the vec, after the current last element.
  // This means that the number of elements goes up by 1, but the capacity may stay
  // the same. If the new size would be greater than the capacity, then the capcity
  // should be increased and the space used for storage should be re-allocated
  // to support the new capacity and values in the oringal storage are copied
  // to the new storage.
  //
  // For our implmentation, you should double the capacity when you reallocate.
  void push_back(const std::string& element);

  // removes the value from the end of the vec and returns it.
  // If there are no elemennts in the vector, then it returns std::nullopt
  std::optional<std::string> pop_back();

  // Returns a reference to the value at the specified index
  // If the index is out of the bounds of the array, then it
  // throws an `out_of_range` exception.
  //
  // Indexes start at 0, just as they do in a normal array.
  std::string& operator[](size_t index);

  // Returns a reference to the value at the specified index
  // If the index is out of the bounds of the array, then it
  // throws an `out_of_range` exception.
  //
  // Indexes start at 0, just as they do in a normal array.
  std::string& at(size_t index);

  // Given an index and a value, inserts the specified value at the specified
  // index in the vec. This extends the existing vec to insert the
  // new element. Any values at or after the specified index before insertion
  // will be shifted "up" an index for the insertion.
  // For example, if we inserted value 3 at index 2, on the vector {1, 5, 2, 8}
  // then the vector should look like {1, 5, 3, 2, 8} after insertion.
  //
  // If the new size would be greater than the capacity, then the capcity
  // should be increased and the space used for storage should be re-allocated
  // to support the new capacity and values in the oringal storage are copied
  // to the new storage.
  //
  // For our implmentation, you should double the capacity when you reallocate.
  //
  // The index can be in the range of [0, length] (inclusive of both).
  // This means that when index == length, we are inserting the element to the
  // back of the vector.
  //
  // If the index is out of the bounds of the vector (except if index == length)
  // then it throws an `out_of_range` exception.
  //
  // Returns the index of the newly inserted element
  size_t insert(size_t index, const std::string& element);

  // Given an index, erases the value at the specified index
  // This means the length of the vector decreases and any elements past
  // the erased element are "shifted down". Capacity remains unchanged.
  //
  // If the index is out of the bounds of the vector, then it
  // throws an `out_of_range` exception.
  //
  // The function should return the index we erased at or if we erased the
  // end of the vector, then the length - 1 is returned
  size_t erase(size_t index);

 private:
  std::string* data_;
  size_t length_;
  size_t capacity_;
  
  static constexpr size_t default_capacity = 10;
};
#endif  // VEC_HPP 
