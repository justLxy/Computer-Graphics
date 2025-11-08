#include "box_box_intersect.h"
bool box_box_intersect(
  const BoundingBox & A,
  const BoundingBox & B)
{
  ////////////////////////////////////////////////////////////////////////////
  // Two boxes intersect if they overlap in all three dimensions.
  // Simplest way to determine if two 3D boxes intersect? (2018, February 28). Mathematics Stack Exchange. https://math.stackexchange.com/questions/2651710/simplest-way-to-determine-if-two-3d-boxes-intersect
  for (int i = 0; i < 3; i++) {
    if (A.max_corner(i) < B.min_corner(i) || B.max_corner(i) < A.min_corner(i)) {
      return false;
    }
  }
  return true;
  ////////////////////////////////////////////////////////////////////////////
}

