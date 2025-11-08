#include "TriangleSoup.h"
#include "Ray.h"
// Hint
#include "first_hit.h"

bool TriangleSoup::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////
  
  bool hit_found = false;
  double closest_t = std::numeric_limits<double>::infinity();
  Eigen::Vector3d closest_normal;
  
  for (const auto& triangle : triangles) {
    double current_t;
    Eigen::Vector3d current_normal;
    
    if (triangle->intersect(ray, min_t, current_t, current_normal)) {
      if (current_t < closest_t) {
        hit_found = true;
        closest_t = current_t;
        closest_normal = current_normal;
      }
    }
  }
  
  if (hit_found) {
    t = closest_t;
    n = closest_normal;
    return true;
  }
  
  return false;
}



