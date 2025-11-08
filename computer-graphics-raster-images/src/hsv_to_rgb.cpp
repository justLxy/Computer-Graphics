#include "hsv_to_rgb.h"

void hsv_to_rgb(
  const double h,
  const double s,
  const double v,
  double & r,
  double & g,
  double & b)
{
  ////////////////////////////////////////////////////////////////////////////
  if (s == 0.0) {
    // Grayscale
    r = g = b = v;
    return;
  }
  
  // Normalize
  double h_norm = h;
  while (h_norm >= 360.0) h_norm -= 360.0;
  while (h_norm < 0.0) h_norm += 360.0;
  
  double h_sector = h_norm / 60.0;
  int i = (int)h_sector;
  double f = h_sector - i;          
  
  double p = v * (1.0 - s);
  double q = v * (1.0 - s * f);
  double t = v * (1.0 - s * (1.0 - f));
  
  switch (i) {
    case 0:  // Red to Yellow (0-60°)
      r = v; g = t; b = p;
      break;
    case 1:  // Yellow to Green (60-120°)
      r = q; g = v; b = p;
      break;
    case 2:  // Green to Cyan (120-180°)
      r = p; g = v; b = t;
      break;
    case 3:  // Cyan to Blue (180-240°)
      r = p; g = q; b = v;
      break;
    case 4:  // Blue to Magenta (240-300°)
      r = t; g = p; b = v;
      break;
    case 5:  // Magenta to Red (300-360°)
      r = v; g = p; b = q;
      break;
    default:
      r = g = b = 0.0;
      break;
  }
  ////////////////////////////////////////////////////////////////////////////
}
