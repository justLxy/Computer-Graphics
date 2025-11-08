#include "reflect.h"

void reflect(
  const std::vector<unsigned char> & input,
  const int width,
  const int height,
  const int num_channels,
  std::vector<unsigned char> & reflected)
{
  reflected.resize(width*height*num_channels);
  ////////////////////////////////////////////////////////////////////////////
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      // Calculate source and destination indices
      // See: Wikipedia - Geometric transformations in image editing
      // https://en.wikipedia.org/wiki/Image_editing#Geometric_transformations
      int src_pixel_idx = (row * width + (width - 1 - col)) * num_channels;
      int dst_pixel_idx = (row * width + col) * num_channels;
      
      for (int channel = 0; channel < num_channels; channel++) {
        reflected[dst_pixel_idx + channel] = input[src_pixel_idx + channel];
      }
    }
  }
  ////////////////////////////////////////////////////////////////////////////
}
