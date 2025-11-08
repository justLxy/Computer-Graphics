#include "demosaic.h"

void demosaic(
  const std::vector<unsigned char> & bayer,
  const int & width,
  const int & height,
  std::vector<unsigned char> & rgb)
{
  rgb.resize(width*height*3);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////
  
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      int pixel_index = row * width + col;
      int rgb_index = pixel_index * 3;
      
      unsigned char r = 0, g = 0, b = 0;
      
      if (row % 2 == 0) { // Even rows
        if (col % 2 == 0) {
          // Green pixel at (even, even)
          g = bayer[pixel_index];
          
          int r_sum = 0, r_count = 0;
          if (row > 0) { r_sum += bayer[(row-1)*width + col]; r_count++; }
          if (row < height-1) { r_sum += bayer[(row+1)*width + col]; r_count++; }
          r = r_count > 0 ? (r_sum + r_count/2) / r_count : g;
          
          int b_sum = 0, b_count = 0;
          if (col > 0) { b_sum += bayer[row*width + (col-1)]; b_count++; }
          if (col < width-1) { b_sum += bayer[row*width + (col+1)]; b_count++; }
          b = b_count > 0 ? (b_sum + b_count/2) / b_count : g;
          
        } else {
          // Blue pixel at (even, odd)
          b = bayer[pixel_index];
          
          int g_sum = 0, g_count = 0;
          if (row > 0) { g_sum += bayer[(row-1)*width + col]; g_count++; }
          if (row < height-1) { g_sum += bayer[(row+1)*width + col]; g_count++; }
          if (col > 0) { g_sum += bayer[row*width + (col-1)]; g_count++; }
          if (col < width-1) { g_sum += bayer[row*width + (col+1)]; g_count++; }
          g = g_count > 0 ? (g_sum + g_count/2) / g_count : 0;
          
          int r_sum = 0, r_count = 0;
          if (row > 0 && col > 0) { r_sum += bayer[(row-1)*width + (col-1)]; r_count++; }
          if (row > 0 && col < width-1) { r_sum += bayer[(row-1)*width + (col+1)]; r_count++; }
          if (row < height-1 && col > 0) { r_sum += bayer[(row+1)*width + (col-1)]; r_count++; }
          if (row < height-1 && col < width-1) { r_sum += bayer[(row+1)*width + (col+1)]; r_count++; }
          r = r_count > 0 ? (r_sum + r_count/2) / r_count : 0;
        }
      } else { // Odd rows
        if (col % 2 == 0) {
          // Red pixel at (odd, even)
          r = bayer[pixel_index];
          
          int g_sum = 0, g_count = 0;
          if (row > 0) { g_sum += bayer[(row-1)*width + col]; g_count++; }
          if (row < height-1) { g_sum += bayer[(row+1)*width + col]; g_count++; }
          if (col > 0) { g_sum += bayer[row*width + (col-1)]; g_count++; }
          if (col < width-1) { g_sum += bayer[row*width + (col+1)]; g_count++; }
          g = g_count > 0 ? (g_sum + g_count/2) / g_count : 0;
          
          int b_sum = 0, b_count = 0;
          if (row > 0 && col > 0) { b_sum += bayer[(row-1)*width + (col-1)]; b_count++; }
          if (row > 0 && col < width-1) { b_sum += bayer[(row-1)*width + (col+1)]; b_count++; }
          if (row < height-1 && col > 0) { b_sum += bayer[(row+1)*width + (col-1)]; b_count++; }
          if (row < height-1 && col < width-1) { b_sum += bayer[(row+1)*width + (col+1)]; b_count++; }
          b = b_count > 0 ? (b_sum + b_count/2) / b_count : 0;
          
        } else {
          // Green pixel at (odd, odd)
          g = bayer[pixel_index];
          
          int b_sum = 0, b_count = 0;
          if (row > 0) { b_sum += bayer[(row-1)*width + col]; b_count++; }
          if (row < height-1) { b_sum += bayer[(row+1)*width + col]; b_count++; }
          b = b_count > 0 ? (b_sum + b_count/2) / b_count : 0;
          
          int r_sum = 0, r_count = 0;
          if (col > 0) { r_sum += bayer[row*width + (col-1)]; r_count++; }
          if (col < width-1) { r_sum += bayer[row*width + (col+1)]; r_count++; }
          r = r_count > 0 ? (r_sum + r_count/2) / r_count : 0;
        }
      }
      
      rgb[rgb_index + 0] = r;
      rgb[rgb_index + 1] = g;
      rgb[rgb_index + 2] = b;
    }
  }
}
