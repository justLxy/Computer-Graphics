#include "Triangle.h"
#include "Ray.h"
#include <Eigen/Dense>

bool Triangle::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////
  
  Eigen::Vector3d A = std::get<0>(corners);
  Eigen::Vector3d B = std::get<1>(corners);  
  Eigen::Vector3d C = std::get<2>(corners);
  
  Eigen::Vector3d edge1 = B - A;
  Eigen::Vector3d edge2 = C - A;
  
  Eigen::Vector3d h = ray.direction.cross(edge2);
  double det = edge1.dot(h);
  
  if (abs(det) < 1e-10) {
    return false;
  }
  
  double inv_det = 1.0 / det;
  
  Eigen::Vector3d s = ray.origin - A;
  
  double u = inv_det * s.dot(h);
  if (u < 0.0 || u > 1.0) {
    return false;
  }
  
  Eigen::Vector3d q = s.cross(edge1);
  double v = inv_det * ray.direction.dot(q);
  if (v < 0.0 || u + v > 1.0) {
    return false;
  }
  
  double hit_t = inv_det * edge2.dot(q);
  
  if (hit_t < min_t) {
    return false;
  }
  
  t = hit_t;
  
  Eigen::Vector3d triangle_normal = edge1.cross(edge2).normalized();
  
  if (ray.direction.dot(triangle_normal) > 0) {
    n = -triangle_normal;  
  } else {
    n = triangle_normal;  
  }
  
  return true;
}


