#include "insert_box_into_box.h"

void insert_box_into_box(
  const BoundingBox & A,
  BoundingBox & B)
{
  ////////////////////////////////////////////////////////////////////////////
  // Grow a box B by inserting a box A.
  B.min_corner = B.min_corner.cwiseMin(A.min_corner);
  B.max_corner = B.max_corner.cwiseMax(A.max_corner);
  ////////////////////////////////////////////////////////////////////////////
}

