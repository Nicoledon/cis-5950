#include "qdbmp.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
using std::cerr;
using std::endl;
using std::string;

constexpr UCHAR MAX_COLOR_VALUE = 255U;
std::vector<UINT> average_pixel(const  int &height,
                                const  int &width,
                                const  int &y, const  int &x,
                                const  int size,  BitMap &image) {
  UINT sum_r = 0;
  UINT sum_g = 0;
  UINT sum_b = 0;
  UINT count = 0;
  std::vector<UINT> vec;
  for (int i = x - size; i <= x + size; i++) {
    for (int j = y - size; j <= y + size; j++) {
      if ( i < 0 || j < 0 || i >= width || j >= height) {
        continue;
      } else {
        RGB color = image.get_pixel(i, j);
        sum_r += color.red;
        sum_g += color.green;
        sum_b += color.blue;
        count += 1;
      }
    }
  }
  vec.push_back(sum_r / count);
  vec.push_back(sum_g / count);
  vec.push_back(sum_b / count);
  return vec;
}
/**
 * This program takes a .bmp image and generates its corresponding "negative"
 * image to a new .bmp file on disk.
 */
int main(int argc, char *argv[]) {
  // Check input commands
  if (argc != 4) {
    cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
    return EXIT_FAILURE;
  }

  string input_fname{argv[1]};
  string output_fname{argv[2]};
  string str_size{argv[3]};
  int size = stoi(str_size);
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
  BitMap average(width, height);

  // Check the command above succeed
  if (image.check_error() != BMP_OK) {
    perror("ERROR: Failed to open BMP file.");
    return EXIT_FAILURE;
  }

  // Loop through each pixel and turn into negative
  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      // Read the current pixel RGB color
      auto vec = average_pixel((int)height , (int)width , y , x , size, image);
      // Calculate the negative RGB color
      RGB average_color{
          static_cast<UCHAR>(vec[0]),
          static_cast<UCHAR>(vec[1]),
          static_cast<UCHAR>(vec[2])
      };

      // Set the negative color
      average.set_pixel(x, y, average_color);
    }
  }

  // Output the negative image to disk
  average.write_file(output_fname);

  if (image.check_error() != BMP_OK) {
    perror("ERROR: Failed to open BMP file.");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}