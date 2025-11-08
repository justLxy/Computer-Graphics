#include "ray_intersect_triangle.h"
#include <Eigen/Geometry>

bool ray_intersect_triangle(
  const Ray & ray,
  const Eigen::RowVector3d & A,
  const Eigen::RowVector3d & B,
  const Eigen::RowVector3d & C,
  const double min_t,
  const double max_t,
  double & t)
{
  ////////////////////////////////////////////////////////////////////////////
  const double EPSILON = 1e-10;
  
  // Convert to Vector3d
  Eigen::Vector3d v0 = A.transpose();
  Eigen::Vector3d v1 = B.transpose();
  Eigen::Vector3d v2 = C.transpose();
  Eigen::Vector3d dir = ray.direction;
  Eigen::Vector3d orig = ray.origin;
  
  // Edge vectors
  Eigen::Vector3d edge1 = v1 - v0;
  Eigen::Vector3d edge2 = v2 - v0;
  
  // Calculate determinant
  Eigen::Vector3d pvec = dir.cross(edge2);
  double det = edge1.dot(pvec);
  
  // Ray is parallel to triangle
  if (det > -EPSILON && det < EPSILON) {
    return false;
  }
  
  double inv_det = 1.0 / det;
  
  // Calculate u
  Eigen::Vector3d tvec = orig - v0;
  double u = tvec.dot(pvec) * inv_det;
  
  // Check if intersection is outside triangle
  if (u < 0.0 || u > 1.0) {
    return false;
  }
  
  // Calculate v
  Eigen::Vector3d qvec = tvec.cross(edge1);
  double v = dir.dot(qvec) * inv_det;
  
  // Check if intersection is outside triangle
  if (v < 0.0 || u + v > 1.0) {
    return false;
  }
  
  // Compute t
  t = edge2.dot(qvec) * inv_det;
  
  // Check if t is within valid range
  return (t >= min_t && t <= max_t);
  ////////////////////////////////////////////////////////////////////////////
}

