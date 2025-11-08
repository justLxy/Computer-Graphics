#include "Plane.h"
#include "Ray.h"

bool Plane::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////
  
  double denominator = ray.direction.dot(normal);
  
  if (abs(denominator) < 1e-10) {
    return false;
  }
  
  double numerator = -(ray.origin - point).dot(normal);
  
  double hit_t = numerator / denominator;
  
  if (hit_t < min_t) {
    return false;
  }
  
  t = hit_t;
  
  if (ray.direction.dot(normal) > 0) {
    n = -normal;  
  } else {
    n = normal;   
  }
  
  return true;
}

