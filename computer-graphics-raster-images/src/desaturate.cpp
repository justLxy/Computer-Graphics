#include "desaturate.h"
#include "hsv_to_rgb.h"
#include "rgb_to_hsv.h"

void desaturate(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  const double factor,
  std::vector<unsigned char> & desaturated)
{
  desaturated.resize(rgb.size());
  ////////////////////////////////////////////////////////////////////////////
  for (int i = 0; i < width * height; ++i) {
    double r = rgb[i * 3 + 0] / 255.0;
    double g = rgb[i * 3 + 1] / 255.0;
    double b = rgb[i * 3 + 2] / 255.0;

    double h, s, v;
    rgb_to_hsv(r, g, b, h, s, v);

    s *= (1.0 - factor);

    hsv_to_rgb(h, s, v, r, g, b);

    desaturated[i * 3 + 0] = r * 255;
    desaturated[i * 3 + 1] = g * 255;
    desaturated[i * 3 + 2] = b * 255;
  }
  ////////////////////////////////////////////////////////////////////////////
}
