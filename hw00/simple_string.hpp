// TODO: add header guards
// look at what ctypes.hpp does and do something similar.
// Just remeber that this file is called simple_string.hpp
#ifndef SIMPLE_STRING_HPP_
#define SIMPLE_STRING_HPP_
#include <cstddef>  // for size_t
#include <ostream>  // for ostream

// no other #includes are allowed

/*!
 * Represents a string "object"
 */
struct simple_string {
  char* data; /*! A dynamically allocated null-terminated character array */
  size_t len; /*! The length of the string */

/*!
 * Since this is a static variable, it is not tied to any specific
 * instance of a simple_string. It can always be referred to via
 * doing `simple_string::npos`
 */
  static const size_t npos = -1;
};

/*!
 * "constructs" a new `simple_string` from a given C-style string.
 *
 * @param: cstring - the null-terminated C-style string this is based off of
 * @return: a new simple_string "object" this object is based off of the
 *          passed in cstring. However, after construction the simple_string
 *          is independent from the passed in `cstring` by making its own copy
 *          of the `cstring`'s contents.
 */
simple_string from_cstring(const char* cstring);

/*!
 * "constructs" a copy of a `simple_string`
 *
 * @param: to_copy - the simple_string we are copying
 * @return: a new simple_string "object" this object is based off of the
 *          passed in simple_string. However, the new simple_string
 *          is independent from the passed in simple_string by making its own
 *          copy of the `simple_string`'s contents.
 *
 */
simple_string duplicate(const simple_string to_copy);

/*!
 * takes the substring of a given string.
 * Note: unlike some substring functions in other languages, in C++
 * the substring is usually defined by being given the starting index into the
 * string and the __length__ of the substring.
 *
 * For example:
 *   simple_string s = from_cstring("Hello!");
 *   simple_string sub = substr(s, 3, 3);
 *   cout << sub << endl;  // prints "lo!"
 *
 * @param: self - the string we are taking the substring of
 * @param: start_index - the index we are starting our substring at
 * @param: length - the length of the substring we want to make.
 *         If length is equal to `simple_string::npos` or if
 *         `length + start_index >= self.len` then the substring
 *         just goes from the specified start index and goes to the
 *         end of the input string.
 * @pre you can assume start_index is a valid index into the string.
 * @return: the specified substring
 */
simple_string substr(const simple_string self, size_t start_index, size_t length);

/*!
 * Finds the location of a character in the string starting
 * from index 0.
 *
 * @param: self - the string we are searching
 * @param: target - the char we are lookking for
 * @return: the lowest index that the character is found at
 *          or simple_string::npos if it is not found.
 */
size_t find(const simple_string self, char target);

/*!
 * Checks to see if two simple_string's are equal.
 *
 * Two simple_strings are equal iff (if and only if) they have
 * the same length and the same character at the same indexes.
 *
 * @param: lhs - the first string we want to compare
 * @param: rhs - the second string we want to compare
 * @returns: true iff the strings are equal
 * 
 * Note that we call the parameters `lhs` and `rhs`.
 * This is short for Left Hand Side and Right Hand Side.
 * This is because C++ allows us to define functions that look like
 * normal operators and this is an exmaple of this. We can invoke this
 * function is like so:
 * 
 * simple_string s1 = from_cstring("Howdy!");
 * simple_string s2 = from_cstring("howdy!");
 * bool equal = s1 == s2;  // <-- we call the function here with ==
 *                         // evaluates to false since the strings are differ
 * 
 * as you can see, one simple_string is on the left side of the `==` operator
 * and one of the simple_strings is on the right side :)
 */
bool operator==(const simple_string lhs, const simple_string rhs);

/*!
 * Gets the character at the specified index in the simple_string
 *
 * @param: self - the string we want to get a character from
 * @param: index - the index we want to get the character from
 * @returns: the character at the specified index
 * @pre: assumes that the index is a valid index into the string
 *
 * example:
 * simple_string s1 = from_cstring("Howdy!");
 * cout << at(s1, 0) << endl;  // prints "H"
 */
char at(const simple_string self, size_t index);

/*!
 * Cleans up a simple_string object.
 * 
 * @param: to_destroy the simple_string whose resources we want to clean up
 * @pre: assumes we are given a valid simple_string
 * @post: using the simple_string passed in for ANYTHING after calling this
 * function is erronous behaviour.
 */
void destruct(simple_string to_destroy);

// this function is implemented for you in the assignment specification.
// It allows us to print using cout like:
// simple_string h = from_cstring("Hello!");
// cout << h << endl;
std::ostream& operator<<(std::ostream& os, const simple_string to_print);
#endif 
