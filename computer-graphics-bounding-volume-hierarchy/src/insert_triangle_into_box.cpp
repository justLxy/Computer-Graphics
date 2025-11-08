#include "insert_triangle_into_box.h"

void insert_triangle_into_box(
  const Eigen::RowVector3d & a,
  const Eigen::RowVector3d & b,
  const Eigen::RowVector3d & c,
  BoundingBox & B)
{
  ////////////////////////////////////////////////////////////////////////////
  // Grow a box B by inserting a triangle with corners a, b, and c.
  B.min_corner = B.min_corner.cwiseMin(a).cwiseMin(b).cwiseMin(c);
  B.max_corner = B.max_corner.cwiseMax(a).cwiseMax(b).cwiseMax(c);
  ////////////////////////////////////////////////////////////////////////////
}


