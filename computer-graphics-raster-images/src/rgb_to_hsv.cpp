#include "rgb_to_hsv.h"

void rgb_to_hsv(
  const double r,
  const double g,
  const double b,
  double & h,
  double & s,
  double & v)
{
  ////////////////////////////////////////////////////////////////////////////
  double max_val = r;
  if (g > max_val) max_val = g;
  if (b > max_val) max_val = b;
  
  double min_val = r;
  if (g < min_val) min_val = g;
  if (b < min_val) min_val = b;
  
  double delta = max_val - min_val;
  
  v = max_val;
  
  if (max_val == 0.0) {
    s = 0.0;
  } else {
    s = delta / max_val;
  }
  
  if (delta == 0.0) {
    h = 0.0;
  } else {
    if (max_val == r) {
      h = 60.0 * ((g - b) / delta);
    } else if (max_val == g) {
      h = 60.0 * (2.0 + (b - r) / delta);
    } else { // max_val == b
      h = 60.0 * (4.0 + (r - g) / delta);
    }
    
    if (h < 0.0) {
      h += 360.0;
    }
  }
  ////////////////////////////////////////////////////////////////////////////
}
