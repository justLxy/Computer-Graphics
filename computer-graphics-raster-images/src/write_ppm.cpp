#include "write_ppm.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_ppm(
  const std::string & filename,
  const std::vector<unsigned char> & data,
  const int width,
  const int height,
  const int num_channels)
{
  assert(
    (num_channels == 3 || num_channels ==1 ) &&
    ".ppm only supports RGB or grayscale images");
  ////////////////////////////////////////////////////////////////////////////
  // Open output file stream
  // https://en.cppreference.com/w/cpp/io/basic_ofstream
  std::ofstream file(filename);
  if (!file.is_open()) {
    return false;
  }
  
  // Write PPM header (see Netpbm PPM format specification)
  // https://netpbm.sourceforge.net/doc/ppm.html
  if (num_channels == 1) {
    file << "P2\n";
  } else {
    file << "P3\n";
  }
  
  file << width << " " << height << "\n";
  file << "255\n";
  
  for (int i = 0; i < height * width * num_channels; i++) {
    file << static_cast<int>(data[i]);
    
    if (num_channels == 1) {
      file << "\n";
    } else {
      if ((i + 1) % 3 == 0) {
        file << "\n";
      } else {
        file << " ";
      }
    }
  }
  
  file.close();
  return true;
  ////////////////////////////////////////////////////////////////////////////
}
