#include "ray_intersect_triangle_mesh_brute_force.h"
#include "ray_intersect_triangle.h"

bool ray_intersect_triangle_mesh_brute_force(
  const Ray & ray,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double min_t,
  const double max_t,
  double & hit_t,
  int & hit_f)
{
  ////////////////////////////////////////////////////////////////////////////
  // loop through all triangles and find the closest hit
  bool hit = false;
  hit_t = max_t;
  hit_f = -1;
  
  for (int f = 0; f < F.rows(); f++) {
    double t;
    if (ray_intersect_triangle(ray, V.row(F(f, 0)), V.row(F(f, 1)), V.row(F(f, 2)), 
                               min_t, hit_t, t)) {
      hit = true;
      hit_t = t;
      hit_f = f;
    }
  }
  
  return hit;
  ////////////////////////////////////////////////////////////////////////////
}
