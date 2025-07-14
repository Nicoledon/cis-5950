#include <cstdlib>
#include <iostream>
#include <string>
#include "qdbmp.hpp"

using std::cerr;
using std::endl;
using std::string;

constexpr UCHAR MAX_COLOR_VALUE = 255U;

/**
 * This program takes a .bmp image and generates its corresponding "negative"
 * image to a new .bmp file on disk.
 */
int main(int argc, char* argv[]) {
  // Check input commands
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
    return EXIT_FAILURE;
  }

  string input_fname{argv[1]};
  string output_fname{argv[2]};

  // Construct a BitMap object using the input file specified
  BitMap image(input_fname);

  // Check the command above succeed
  if (image.check_error() != BMP_OK) {
    perror("ERROR: Failed to open BMP file.");
    return EXIT_FAILURE;
  }

  // Create a new BitMap for output the negative image
  const unsigned int height = image.height();
  const unsigned int width = image.width();
  BitMap negative(width, height);

  // Check the command above succeed
  if (image.check_error() != BMP_OK) {
    perror("ERROR: Failed to open BMP file.");
    return EXIT_FAILURE;
  }

  // Loop through each pixel and turn into negative
  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      // Read the current pixel RGB color
      RGB color = image.get_pixel(x, y);

      // Calculate the negative RGB color
      RGB reverse_color{
          static_cast<UCHAR>(MAX_COLOR_VALUE - color.red),
          static_cast<UCHAR>(MAX_COLOR_VALUE - color.green),
          static_cast<UCHAR>(MAX_COLOR_VALUE - color.blue),
      };

      // Set the negative color
      negative.set_pixel(x, y, reverse_color);
    }
  }

  // Output the negative image to disk
  negative.write_file(output_fname);

  if (image.check_error() != BMP_OK) {
    perror("ERROR: Failed to open BMP file.");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}