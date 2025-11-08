#include "Sphere.h"
#include "Ray.h"
bool Sphere::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////
  
  Eigen::Vector3d oc = ray.origin - center;
  
  double a = ray.direction.dot(ray.direction);
  double b = 2.0 * oc.dot(ray.direction);
  double c = oc.dot(oc) - radius * radius;
  
  double discriminant = b * b - 4.0 * a * c;
  
  if (discriminant < 0.0) {
    return false;
  }
  
  double sqrt_discriminant = sqrt(discriminant);
  double t1 = (-b - sqrt_discriminant) / (2.0 * a);
  double t2 = (-b + sqrt_discriminant) / (2.0 * a);
  
  double hit_t;
  if (t1 >= min_t) {
    hit_t = t1;
  } else if (t2 >= min_t) {
    hit_t = t2;
  } else {
    return false;
  }
  
  t = hit_t;
  
  Eigen::Vector3d intersection_point = ray.origin + t * ray.direction;
  n = (intersection_point - center) / radius;  
  
  return true;
}

