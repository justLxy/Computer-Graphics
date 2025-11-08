#include "triangle_area_normal.h"
#include <Eigen/Geometry>

Eigen::RowVector3d triangle_area_normal(
  const Eigen::RowVector3d & a, 
  const Eigen::RowVector3d & b, 
  const Eigen::RowVector3d & c)
{
  Eigen::RowVector3d u = b - a;
  Eigen::RowVector3d v = c - a;
  
  // Cross product length = 2 * area, so divide by 2
  return u.cross(v) / 2.0;
}
