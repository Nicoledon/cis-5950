/*
 * Copyright ©2025 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Pennsylvania
 * CIT 5950 for use solely during Spring Semester 2025 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#ifndef BUFFEREDFILEREADER_HPP_
#define BUFFEREDFILEREADER_HPP_

#include <array>
#include <cstdint>
#include <string>
#include <optional>

///////////////////////////////////////////////////////////////////////////////
// A BufferedFileReader is a class for reading files.
//
// This class is a moderately complex wrapper around POSIX file I/O calls
// with more functionality than SimpleFileReader. Reading from the file
// is buffered to increase performance.
///////////////////////////////////////////////////////////////////////////////
class BufferedFileReader {
 public:
  // Constructor for a BufferedFileReader. Should open the
  // file and do whatever is necesary to "set-up" the object.
  // After construction, reading from the file should start
  // at the front of the file.
  // Undefined behaviour if the file name is invalid.
  //
  // Arguments:
  // - fname: The name of the file to be read
  BufferedFileReader(const std::string& fname);

  // Destructor for a BufferedFileReader. Should clean up
  // any allocated resources such as memory or open files.
  //
  // Arguments: None
  ~BufferedFileReader();

  // Move Constructor for the BufferedFileReader.
  // Should setup the newly constructor object to refer to the same file
  // and have the same contents as `other` however `other` is set to
  // be "empty" and where it is safe to destruct without causing any
  // issues. 
  //
  // No operations may be performed on `other` after this function
  // other than it's inevitable destructor. Doing anything other than
  // it's destructor would be undefined behaviour. (This means you
  // do not have to worry about any functions being called on other
  // after this other than its destructor).
  BufferedFileReader(BufferedFileReader&& other);

  // Similar to previous, this is the move assignment operator
  // We are moving the values in `other` into this object, but *this
  // is already constructed.
  //
  // Should do nothing if assigning into ones self.
  //
  // Should return a reference to *this.
  BufferedFileReader& operator=(BufferedFileReader&& other);

  // Sets up the BufferedFileReader to start reading from the
  // front of the specified file. Note that there could or could not
  // already be an open file managed by the BufferedFileReader.
  // This function handles both cases and if the object is already
  // managing a file, that file is closed.
  // Undefined behaviour if the file name is invalid.
  //
  // Arguments:
  // - fname: The name of the file to be opened
  void open_file(const std::string& fname);

  // Closes the file currently managed by the BufferedFileReader.
  // If there is not a file currently open, then nothing should happen.
  //
  // Arguments: None
  void close_file();

  // Gets the next singular character from the file.
  //
  // Arguments: None
  //
  // Returns:
  // - the next char in the file. If at the end of the file,
  //   or if there is no file open currently, then EOF is returned.
  char get_char();

  // The next two functions deal with the reading of "tokens".
  // A token is a sequence of characters whose end is marked by a delimiter
  // character and does not contain any delimiters in it. Note that the
  // end of file can be thought of as a delimiter.
  // Also note that a token can be the empty string.
  //
  // For example: if the file had the contents "hi,there,,aaaaa,!0 fds"
  // and the delimiters for the file was specified to only be the ','
  // character, then the tokens in the file would be:
  // - "hi"
  // - "there"
  // - ""
  // - "aaaaa"
  // - "!0 fds"

  // Reads the next token from the file.
  //
  // See above comments for what a token is.
  //
  // When reading tokens, if a token were to be read, we can mark the
  // delimiter for that token as having been read. For example,
  // if we used the example above and starting from the beginning of the file,
  // after the first call to get_token, the unread part of the file would be
  // "there,,aaaaa,!0 fds".
  //
  // Returns nullopt if already at EOF
  // or if there is no open file. It is valid to return an empty token if
  // no characters were read before hitting a delimiter.
  //
  // Arguments:
  // - delims: a string containing all of the characters to
  //   be used as delimiters for reading tokens.
  //   NOTE: delims is an optional argument and is by default
  //   set to white space characters
  //
  // Returns:
  // - the next token in the file,
  // - nullopt if alrady at EOF or if the file is not open.
  std::optional<std::string> get_token(const std::string& delims=" \t\n\r\v\f");

  // Returns the current position the user is in to the file.
  //
  // Arguments: None
  //
  // Returns:
  // - The current position we are in the file, which is the
  //   Offset from the start of the file. (e.g, if the user is at
  //   the start of the file, returns 0. If the user has read 2
  //   characters, return 2. etc.).
  // - -1 if there is no open file
  int tell() const;

  // Resets the file to start reading from the beginning
  // of the file that is currently open.
  // Does Nothing if there is no file open currently.
  //
  // Arguments: None
  void rewind();

  // Returns whether or not the file is available for reading
  // (e.g. if the file is open and not at the end of file)
  // Note: The reader is only considered to be at the end of file
  // if it had previously tried to read, and then hit the end
  // of the file, causing it to have an "incomplete" read).
  //
  // Arguements: None
  //
  // Returns:
  // - false if the file reader is at the end of the file
  //   or if there is no file open
  // - true otherwise
  bool good() const;

  // Provided and a synonym for the above
  // this allows us to use this object as if it were a boolean
  // when evaluating an expression.
  // e.g.
  // BufferedFileReader bf("Hello.txt");
  // if (bf) {
  //   cout << "bf is good!" << endl;
  // }
  operator bool() const;

  // Ignore These
  // This is disabling the
  // copy constructor and the assignment operator.
  // We disable them to avoid any issues with buffering.
  // Move is supported though.
  BufferedFileReader(const BufferedFileReader& other) = delete;
  BufferedFileReader& operator=(const BufferedFileReader& other) = delete;

  // Ignore this
  // This is necessary for testing and will be talked about later in the course
  friend class BufferChecker;

 private:
  // Constants
  static constexpr uint64_t BUF_SIZE = 1024;  // the size of the buffer.

  // fields
  size_t curr_length_;  // The current number of characters stored in the buffer
                        // To understand the purpose of this, consider when
                        // a file is less than BUF_SIZE in length.

  size_t curr_index_;  // The current index we are in to the buffer.
                       // necessary since we many not parse the entire
                       // buffer in one function call.

  std::array<char, BUF_SIZE> buffer_;  // The buffer we maintiain for reading
                                       // from the file.

  int fd_;              // The File Descriptor that we use to manage our file.
  bool good_;           // Whether or not the reader is good to read
  void fill_buffer();
  int count_;
  // Suggested Helper:
  // - void fill_buffer();
};

#endif  // BUFFEREDFILEREADER_HPP_
