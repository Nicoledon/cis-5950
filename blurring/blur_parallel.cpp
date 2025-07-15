#include "qdbmp.hpp"
#include <array>
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <string>
#include <vector>
#include <stack>
using std::array;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std:: stack;
pthread_mutex_t lock;
struct thd_arg {
  int height;
  int width;
  int y;
  int x;
  int size;
  BitMap *ref;
  BitMap *image;
};
void *average_pixel(void *arg) {

  vector<struct thd_arg *>*threads =  reinterpret_cast<vector<struct thd_arg *>*>(arg);
  for(struct thd_arg * &a : *threads){
  UINT sum_r = 0;
  UINT sum_g = 0;
  UINT sum_b = 0;
  UINT count = 0;
  int height = a->height;
  int width = a->width;
  int y = a->y;
  int x = a->x;
  int size = a->size;
  BitMap *image = a->image;
  for (int i = x - size; i <= x + size; i++) {
    for (int j = y - size; j <= y + size; j++) {
      if (i < 0 || j < 0 || i >= width || j >= height) {
        continue;
      } else {
        RGB color = a->ref->get_pixel(i, j);
        sum_r += color.red;
        sum_g += color.green;
        sum_b += color.blue;
        count += 1;
      }
    }
  }
  sum_r /= count;
  sum_g /= count;
  sum_b /= count;
  RGB average_color{static_cast<UCHAR>(sum_r), static_cast<UCHAR>(sum_g),
                    static_cast<UCHAR>(sum_b)};
  
  pthread_mutex_lock(&lock);
  image->set_pixel(x, y, average_color);
  pthread_mutex_unlock(&lock);
  delete a;
  }
  return nullptr;
}
/**
 * This program takes a .bmp image and generates its corresponding "negative"
 * image to a new .bmp file on disk.
 */
int main(int argc, char *argv[]) {
  // Check input commands
  if (argc != 5) {
    cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
    return EXIT_FAILURE;
  }

  string input_fname{argv[1]};
  string output_fname{argv[2]};
  string str_size{argv[3]};
  string thread_size{argv[4]};
  int size = stoi(str_size);
  int n_threads = stoi(thread_size);
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
  pthread_t thds[n_threads];
  pthread_mutex_init(&lock, nullptr);
  int count = 0;

  // Loop through each pixel and turn into negative
  for (size_t y = 0; y < height; ++y) {
      vector<struct thd_arg *> *threads = new vector <struct thd_arg *>;
    for (size_t x = 0; x < width; ++x) {
      // Read the current pixel RGB color
      // Calculate the negative RGB color
      struct thd_arg *args = new struct thd_arg;
      args->width = width;
      args->height = height;
      args->x = x;
      args->y = y;
      args->size = size;
      args->ref = &image;
      args->image = &average;
      threads->push_back(args);
      // Set the negative color
    }
    pthread_create(&thds[count++] , nullptr , &average_pixel , threads);
      if(count == n_threads){
          for (int i = 0 ; i < count ; i ++){
              pthread_join(thds[i] , nullptr);
          }
          count = 0;
      }
  }
  for(int i = 0 ;i < count ; i ++ ){
      pthread_join(thds[i] , nullptr);
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
