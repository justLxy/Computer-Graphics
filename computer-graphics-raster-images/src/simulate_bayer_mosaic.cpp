#include "simulate_bayer_mosaic.h"

void simulate_bayer_mosaic(
  const std::vector<unsigned char> & rgb,
  const int & width,
  const int & height,
  std::vector<unsigned char> & bayer)
{
  bayer.resize(width*height);
  ////////////////////////////////////////////////////////////////////////////
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      int pixel_index = row * width + col;
      int rgb_index = pixel_index * 3;
      
      if (row % 2 == 0) { // Even rows
        if (col % 2 == 0) {
          // Green channel
          bayer[pixel_index] = rgb[rgb_index + 1];
        } else {
          // Blue channel
          bayer[pixel_index] = rgb[rgb_index + 2];
        }
      } else { // Odd rows
        if (col % 2 == 0) {
          // Red channel
          bayer[pixel_index] = rgb[rgb_index + 0];
        } else {
          // Green channel
          bayer[pixel_index] = rgb[rgb_index + 1];
        }
      }
    }
  }
  ////////////////////////////////////////////////////////////////////////////
}
