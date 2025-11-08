#include "rotate.h"

void rotate(
  const std::vector<unsigned char> & input,
  const int width,
  const int height,
  const int num_channels,
  std::vector<unsigned char> & rotated)
{
  rotated.resize(height*width*num_channels);
  ////////////////////////////////////////////////////////////////////////////
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      int src_pixel_idx = (row * width + col) * num_channels;
      
      // See: Wikipedia - Geometric transformations in image editing
      // https://en.wikipedia.org/wiki/Image_editing#Geometric_transformations
      // 90° counter-clockwise: (row, col) -> (width-1-col, row)
      int dst_row = width - 1 - col;
      int dst_col = row;
      int dst_pixel_idx = (dst_row * height + dst_col) * num_channels;
      
      for (int channel = 0; channel < num_channels; channel++) {
        rotated[dst_pixel_idx + channel] = input[src_pixel_idx + channel];
      }
    }
  }
  ////////////////////////////////////////////////////////////////////////////
}
