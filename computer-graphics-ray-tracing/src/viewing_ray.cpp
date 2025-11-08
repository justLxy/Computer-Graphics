#include "viewing_ray.h"

void viewing_ray(
  const Camera & camera,
  const int i,
  const int j,
  const int width,
  const int height,
  Ray & ray)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  ////////////////////////////////////////////////////////////////////////////
  
  Eigen::Vector3d image_plane_center = camera.e - camera.d * camera.w;
  
  double u_coord = (j + 0.5) * camera.width / width - camera.width / 2.0;
  double v_coord = camera.height / 2.0 - (i + 0.5) * camera.height / height;
  
  Eigen::Vector3d image_plane_point = image_plane_center + 
                                     u_coord * camera.u + 
                                     v_coord * camera.v;
  
  ray.origin = camera.e;

  ray.direction = image_plane_point - camera.e;
}

