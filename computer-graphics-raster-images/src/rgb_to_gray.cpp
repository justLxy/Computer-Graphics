#include "rgb_to_gray.h"

void rgb_to_gray(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  std::vector<unsigned char> & gray)
{
  gray.resize(height*width);
  ////////////////////////////////////////////////////////////////////////////
  for (int i = 0; i < height * width; i++) {
    unsigned char r = rgb[3*i + 0];
    unsigned char g = rgb[3*i + 1];  
    unsigned char b = rgb[3*i + 2];
    
    double gray_value = 0.2126 * r + 0.7152 * g + 0.0722 * b;
    
    gray[i] = static_cast<unsigned char>(gray_value);
  }
  ////////////////////////////////////////////////////////////////////////////
}


