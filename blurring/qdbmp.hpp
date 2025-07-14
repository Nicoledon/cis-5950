#ifndef QDBMP_H_
#define QDBMP_H_

#include <string>
#include <iostream>

// extern "C" needed to use something written in C
extern "C" {
  #include "cqdbmp.h"
}

/**
 * A struct to represent a RGB color.
 */ 
struct RGB {
  UCHAR red, green, blue;
  RGB(UCHAR red, UCHAR green, UCHAR blue) : red(red), green(green), blue(blue) {}
  
  // ignore these
  RGB(const RGB& other) = default;
  RGB& operator=(const RGB& other) = default;
  RGB(RGB&& other) = default;
  RGB& operator=(RGB&& other) = default;
  ~RGB() = default;
};

// Function so that we can do stuff like:
// RGB bleh {...};
// cout << bleh;
std::ostream& operator<<(std::ostream& out, RGB to_print);

/**
 * A class that represent a .bmp image. 
 * 
 * This class uses cqdbmp library to interact with the image, such as load/save a .bmp file,
 *   get/set a pixel in the .bmp file, and error checking. 
 * 
 * Note that all methods in this class may read/set error code in the cqdbmp library, 
 *   so you need to modify this class to support multi-threaded program.
 */
class BitMap {
 public:
  // constructors
  BitMap(UINT width, UINT height);
  BitMap(std::string file);
  ~BitMap();

  // getters
  UINT width();
  UINT height();
  RGB get_pixel(UINT x, UINT y);

  // setters
  void set_pixel(UINT x, UINT y, RGB rgb);

  // I/O
  void write_file(std::string file);

  // error
  BMP_STATUS check_error();

  // The four lines below the comments make it so
  // that if you want to pass a BitMap as a parameter
  // you must use a reference. You also cannot
  // assign one bitmap to be equal to another.
  // USE REFERENCES OR POINTERS.
  //
  // If you want to know more, this is disabling the
  // copy constructor and the assignment operator.
  // These will be covered later in the class
  BitMap(const BitMap& other) = delete;
  BitMap& operator=(const BitMap& other) = delete;
  BitMap(BitMap&& other) = delete;
  BitMap& operator=(BitMap&& other) = delete;

 private:
  BMP *m_bmpPtr;
};

#endif  // QDBMP_H_