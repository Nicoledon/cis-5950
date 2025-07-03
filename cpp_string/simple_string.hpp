// TODO: add header guards
#ifndef SIMPLE_STRING_HPP
#define SIMPLE_STRING_HPP
#include <cstddef>  // for size_t
#include <vector>   // for vector

// no other #includes are allowed

/*!
 * Represents a simplified version of the real C++ string object
 * Note that this C++ string object is an abstraction ontop of a
 * C-style string that is null-terminated. Notably though, the
 * null-terminator doesn't contribute to the length and is not considered
 * a character "within" the string from the perspective of the user.
 */
class simple_string {
 public:
  // constructors
  
  /*!
   * Constucts a new string object as if it was constructed
   * to represent the empty string: ""
   */
  simple_string();


  /*!
   * constructs a new `simple_string` from a given C-style string.
   *
   * @param: cstring - the null-terminated C-style string this is based off of
   */
  simple_string(const char* cstring);

  /*! copy constructor
   *  makes an independent copy of the passed in string
   */
  simple_string(const simple_string& other);

  // destructor
  ~simple_string();

  // member functions

  /*!
   * returns a const pointer to the underlying C string
   * that this object encapsulates.
   */
  const char* data() const;

  /*!
   * Returns the current length of the string
   */
  size_t len() const;

  /*!
   * takes the substring of `this`.
   * Note: unlike some substring functions in other languages, in C++
   * the substring is usually defined by being given the starting index into the
   * string and the __length__ of the substring.
   *
   * For example:
   *   simple_string s = from_cstring("Hello!");
   *   simple_string sub = substr(s, 3, 3);
   *   cout << sub << endl;  // prints "lo!"
   *
   * @param: start_index - the index we are starting our substring at
   * @param: length - the length of the substring we want to make.
   *         If length is equal to `simple_string::npos` or if
   *         `length + start_index >= self.len` then the substring
   *         just goes from the specified start index and goes to the
   *         end of the input string.
   * @pre if start_index is out of range, it throws an `out_of_range` exception
   * @return: the specified substring
   */
  simple_string substr(size_t start_index, size_t length) const;

  /*!
   * Checks to see if `this` and another simple_string is equal.
   *
   * Two simple_strings are equal iff (if and only if) they have
   * the same length and the same character at the same indexes.
   *
   * @param: other - the string we are comparing to `this`
   * @returns: true iff the strings are equal
   */
  bool operator==(const simple_string& other) const;

  // return the character at the specified index
  // throws out_of_range iff idnex is out of range.
  char& at(size_t index);

  // return the character at the specified index
  // throws out_of_range iff idnex is out of range.
  char& operator[](size_t index);

  /*!
   * Finds the location of a character in the string starting
   * from index 0.
   *
   * @param: target - the char we are lookking for
   * @return: the lowest index that the character is found at
   *          or simple_string::npos if it is not found.
   */
  size_t find(char target) const;

  /*!
   * Finds the first character equal to one of the characters
   * in the given character sequence. Searches in the range from [pos, len)
   * (from the given position until the end of the string).
   *
   * @param: chars - the set of characters we are searching for in `this`
   * @param: pos - the postion we start looking for a matching character
   *               in `this`.
   * @return: the index of the found matching character or simple_string::npos
   *          if it wasn't found
   * 
   * for example:
   * simple_string s1("halso!");
   * simple_string s2("lo");
   * s1.find_first_of(s2, 0)  should be 2
   * s1.find_first_of(s2, 3)  should be 4
   * s1.find_first_of(s2, 5)  should be simple_string::npos
   */
  size_t find_first_of(const simple_string& chars, size_t pos) const;

  /*!
   * Returns a vector of strings that are the non-empty "words" or "tokens" of
   * `this` that have been split on the characters inside the parameter `delims`
   *
   * @param: delims - the set of characters we use to separater "words" in `this`
   * @return: a vector of strings containing the resulting split of words.
   *
   * Examples:
   * simple_string s1("halso!");
   * simple_string s2("lo");
   * s1.split(s2)  should be ["ha", "s", "!"]
   *
   * simple_string s1("helolollllolo!");
   * simple_string s2("lo");
   * s1.split(s2)  should be ["he", "!"]
   *
   * simple_string s1("");
   * simple_string s2("anything");
   * s1.split(s2)  should be []
   */
  std::vector<simple_string> split(const simple_string& delims) const;

  /*!
   * Since this is a static variable, it is not tied to any specific
   * instance of a simple_string. It can always be referred to via
   * doing `simple_string::npos`
   */
  static const size_t npos = -1;

 private:
  char* data_; /*! A dynamically allocated null-terminated character array */
  size_t len_; /*! The length of the string */
};
#endif
