/**************************************************************

	The program reads two BMP image files and
        determines how different they are.
        Adapted from Chirs Murphy's .c version

**************************************************************/

#include "qdbmp.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>

using std::ofstream;
using std::cerr;
using std::cout;
using std::endl;

/* Creates a negative image of the input bitmap file */
int main( int argc, char* argv[] )
{
  UCHAR	r1, g1, b1, r2, g2, b2;
  UINT	width1, height1, width2, height2;
  UINT	x, y;
  BMP*	bmp1;
  BMP*    bmp2;
  
  /* Check arguments */
  if ( argc != 3 && argc != 4 ) {
      cerr << "Usage: " << argv[0] << " <bmp file #1> <bmp file #2> <results file>?" << endl;
      return EXIT_FAILURE;
  }
  
  /* Read the first image file */
  bmp1 = BMP_ReadFile( argv[ 1 ] );
  BMP_CHECK_ERROR( stdout, -1 );
  /* Read the second image file */
  bmp2 = BMP_ReadFile( argv[ 2 ] );
  BMP_CHECK_ERROR( stdout, -1 );
  
  /* Get each image's dimensions */
  width1 = BMP_GetWidth( bmp1 );
  height1 = BMP_GetHeight( bmp1 );
  width2 = BMP_GetWidth( bmp2 );
  height2 = BMP_GetHeight( bmp2 );
  
  // if the size is different, that's bad
  if (width1 != width2) {
    printf("WIDTH IS DIFFERENT\n");
  }
  if (height1 != height2) {
    printf("HEIGHT IS DIFFERENT\n");
  }

  long diff = 0;
  int correct_pixels = 0;
  int incorrect_pixels = 0;

  
  /* Iterate through all the image's pixels */
  for ( x = 0 ; x < width1 ; ++x ) {
   
    for ( y = 0 ; y < height1 ; ++y ) {
      /* Get pixel's RGB values from first image */
      BMP_GetPixelRGB( bmp1, x, y, &r1, &g1, &b1 );
      /* Get pixel's RGB values from second image */
      BMP_GetPixelRGB( bmp2, x, y, &r2, &g2, &b2 );

      if (r1 == r2 && g1 == g2 && b1 == b2) {
	correct_pixels++;
      }
      else {
	incorrect_pixels++;
      }

      if (r1 != r2) {
	//printf("RED IS OFF BY %d\n", abs(r1-r2));
	diff += abs(r1-r2);
      }
      if (g1 != g2) {
	//printf("GREEN IS OFF BY %d\n", abs(g1-g2));
	diff += abs(g1-g2);
      }
      if (b1 != b2) {
	//printf("BLUE IS OFF BY %d\n", abs(b1-b2));
	diff += abs(b1-b2);
      }
      
    }
  }

  float pct_incorrect = 100 * incorrect_pixels / (float)(correct_pixels + incorrect_pixels);
  long max_diff = 255 * incorrect_pixels;
  float pct_diff = 100 * diff / (float)max_diff;

  if (argc == 3) {
    cout << "============================================" << endl;
    cout << "Correct pixels is " << correct_pixels << endl;
    cout << "Incorrect pixels is " << incorrect_pixels << endl;
    cout << std::setprecision(4) << std::fixed;
    cout << "Pct incorrect is " << pct_incorrect << endl;
    cout << "Pct diff is " << pct_diff << endl;
    cout << "============================================" << endl;
  } else {  // argc == 4
    ofstream out(argv[3]);
    out << std::setprecision(4) << std::fixed;
    out << "Correct Pixels: " << correct_pixels << '\n';
    out << "Incorrect pixels: " << incorrect_pixels << '\n';
    out << "Incorrect Pct: " << pct_incorrect << '\n';
    out << "Pct Diff: " << pct_diff << endl;
  }
  
  /* Free all memory allocated for the image */
  BMP_Free( bmp1 );
  BMP_Free( bmp2 );
  
  return 0;
}
