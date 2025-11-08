#include "AABBTree.h"
#include "ray_intersect_box.h"

// See AABBTree.h for API
bool AABBTree::ray_intersect(
  const Ray& ray,
  const double min_t,
  const double max_t,
  double & t,
  std::shared_ptr<Object> & descendant) const 
{
  ////////////////////////////////////////////////////////////////////////////
  // Pruning: If a ray does not intersect the bounding box of the current node, none of its child nodes can intersect it.
  if (!ray_intersect_box(ray, box, min_t, max_t)) {
    return false;
  }
  
  // Check intersections with children
  double current_max_t = max_t;
  bool hit = false;
  
  double t_left = current_max_t;
  std::shared_ptr<Object> desc_left;
  bool hit_left = left && left->ray_intersect(ray, min_t, current_max_t, t_left, desc_left);

  if (hit_left) {
    hit = true;
    current_max_t = t_left;
    t = t_left;
    descendant = desc_left ? desc_left : left;
  }

  double t_right = current_max_t;
  std::shared_ptr<Object> desc_right;
  bool hit_right = right && right->ray_intersect(ray, min_t, current_max_t, t_right, desc_right);

  if (hit_right) {
    hit = true;
    t = t_right;
    descendant = desc_right ? desc_right : right;
  }
  
  return hit;
  ////////////////////////////////////////////////////////////////////////////
}

