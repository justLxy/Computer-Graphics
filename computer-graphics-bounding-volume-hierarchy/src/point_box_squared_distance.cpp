#include "point_box_squared_distance.h"

double point_box_squared_distance(
  const Eigen::RowVector3d & query,
  const BoundingBox & box)
{
  ////////////////////////////////////////////////////////////////////////////
  double sqrd = 0;
  for (int i = 0; i < 3; i++) {
    if (query(i) < box.min_corner(i)) {
      // Point is below the box in this dimension
      double diff = box.min_corner(i) - query(i);
      sqrd += diff * diff;
    } else if (query(i) > box.max_corner(i)) {
      // Point is above the box in this dimension
      double diff = query(i) - box.max_corner(i);
      sqrd += diff * diff;
    }
    // Otherwise point is within box bounds in this dimension, 0
  }
  return sqrd;
  ////////////////////////////////////////////////////////////////////////////
}
