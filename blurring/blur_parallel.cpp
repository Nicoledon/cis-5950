#include "qdbmp.hpp"
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <string>
#include <vector>
#include <array>
using std::cerr;
using std::endl;
using std::string;
using std::array;
constexpr UCHAR MAX_COLOR_VALUE = 255U;
pthread_mutex_t lock;
struct thread_item {
        int height;
        int width;
        int y;
        int x;
        int size;
        BitMap *image;
        BitMap *average;      
};
void *average_pixel(void * arg) {
  struct thread_item *item = reinterpret_cast<struct thread_item *> (arg);
  UINT sum_r = 0;
  UINT sum_g = 0;
  UINT sum_b = 0;
  UINT count = 0;
  std::array<UINT, 3> vec;
  for (int i = item->x - item->size; i <= item->x + item->size; i++) {
    for (int j = item->y - item->size; j <= item->y + item->size; j++) {
      if (i < 0 || j < 0 || i >= item->width || j >= item->height) {
        continue;
      } else {
        RGB color = item->image->get_pixel(i, j);
        sum_r += color.red;
        sum_g += color.green;
        sum_b += color.blue;
        count += 1;
      }
    }
  }
  vec[0] = (sum_r / count);
  vec[1] = (sum_g / count);
  vec[2] = (sum_b / count);
  RGB average_color{static_cast<UCHAR>(vec[0]), static_cast<UCHAR>(vec[1]),
                    static_cast<UCHAR>(vec[2])};

  // Set the negative color
  pthread_mutex_lock(&lock);
  item->average->set_pixel(item->x, item->y, average_color);
  pthread_mutex_unlock(&lock);
  delete item;
  return nullptr;
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
  pthread_mutex_init(&lock , nullptr);
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
  const int n_thread = height * width;
  array<pthread_t , n_thread>thds{};
  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      // Read the current pixel RGB color
      struct thread_item * args = new struct thread_item;
      args->height = (int)height;
      args->width = (int)width;
      args->y = y ;
      args->x = x;
      args->image = &image;
      args->average = &average;
      pthread_create(&thds.at(y * x) , nullptr , &average_pixel , args);
      // Calculate the negative RGB color
    }
  }
  for(int i = 0 ; i < n_thread ; i ++){
      pthread_join(thds.at(i) ,nullptr);
  }
  pthread_mutex_destroy(&lock);
  // Output the negative image to disk
  average.write_file(output_fname);

  if (image.check_error() != BMP_OK) {
    perror("ERROR: Failed to open BMP file.");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}