#include "PointLight.h"

void PointLight::direction(
  const Eigen::Vector3d & q, Eigen::Vector3d & d, double & max_t) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:
  ////////////////////////////////////////////////////////////////////////////
  Eigen::Vector3d light_vector = p - q;
  
  // The parametric distance to the light
  max_t = light_vector.norm();
  
  d = light_vector.normalized();
}
