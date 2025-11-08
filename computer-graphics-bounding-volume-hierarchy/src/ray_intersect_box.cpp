#include "ray_intersect_box.h"
#include <iostream>
#include <algorithm>

bool ray_intersect_box(
  const Ray & ray,
  const BoundingBox& box,
  const double min_t,
  const double max_t)
{
  ////////////////////////////////////////////////////////////////////////////
  double tmin = min_t;
  double tmax = max_t;
  
  for (int i = 0; i < 3; i++) {
    if (std::abs(ray.direction(i)) < 1e-10) {
      // Ray is parallel to the slab
      // origin is within the slab
      if (ray.origin(i) < box.min_corner(i) || ray.origin(i) > box.max_corner(i)) {
        return false;
      }
    } else {
      // Compute intersection
      double t1 = (box.min_corner(i) - ray.origin(i)) / ray.direction(i);
      double t2 = (box.max_corner(i) - ray.origin(i)) / ray.direction(i);
      
      // t1 is the near intersection and t2 is the far
      if (t1 > t2) std::swap(t1, t2);
      
      tmin = std::max(tmin, t1);
      tmax = std::min(tmax, t2);
      
      // No overlap
      if (tmin > tmax) {
        return false;
      }
    }
  }
  
  // An intersection if tmax >= tmin
  return true;
  ////////////////////////////////////////////////////////////////////////////
}
