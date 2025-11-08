#include "first_hit.h"

bool first_hit(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  int & hit_id, 
  double & t,
  Eigen::Vector3d & n)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////
  
  bool hit_found = false;
  double closest_t = std::numeric_limits<double>::infinity();
  Eigen::Vector3d closest_normal;
  int closest_object_id = -1;
  
  for (size_t i = 0; i < objects.size(); i++) {
    double current_t;
    Eigen::Vector3d current_normal;
    
    if (objects[i]->intersect(ray, min_t, current_t, current_normal)) {
      if (current_t < closest_t) {
        hit_found = true;
        closest_t = current_t;
        closest_normal = current_normal;
        closest_object_id = static_cast<int>(i);
      }
    }
  }
  
  if (hit_found) {
    hit_id = closest_object_id;
    t = closest_t;
    n = closest_normal;
    return true;
  }
  
  return false;
}

