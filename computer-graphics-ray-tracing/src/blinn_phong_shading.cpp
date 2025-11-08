#include "blinn_phong_shading.h"
// Hint:
#include "first_hit.h"
#include <iostream>
#include <algorithm>
#include <cmath>

Eigen::Vector3d blinn_phong_shading(
  const Ray & ray,
  const int & hit_id, 
  const double & t,
  const Eigen::Vector3d & n,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector<std::shared_ptr<Light> > & lights)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  ////////////////////////////////////////////////////////////////////////////
  
  // Get material of the hit object
  const Material& material = *(objects[hit_id]->material);
  
  // Hit point
  Eigen::Vector3d hit_point = ray.origin + t * ray.direction;
  
  // View direction
  Eigen::Vector3d view_dir = -ray.direction.normalized();
  
  // Ambient lighting
  double ia = 0.1;
  Eigen::Vector3d color = material.ka * ia;
  
  // Process each light source
  for (const auto& light : lights) {
    // Get direction to light and max parametric distance
    Eigen::Vector3d light_dir;
    double max_t_light;
    light->direction(hit_point, light_dir, max_t_light);
    
    // Check for shadows by shooting a ray toward the light
    Ray shadow_ray;
    shadow_ray.origin = hit_point;
    shadow_ray.direction = light_dir;
    
    int shadow_hit_id;
    double shadow_t;
    Eigen::Vector3d shadow_n;
    
    // Avoid self-intersection
    double epsilon = 1e-10;
    bool in_shadow = first_hit(shadow_ray, epsilon, objects, shadow_hit_id, shadow_t, shadow_n);
    
    // If in shadow and the shadow hit is closer than the light, skip this light
    if (in_shadow && shadow_t < max_t_light) {
      continue;
    }
    
    // Not in shadow, compute lighting
    double n_dot_l = std::max(0.0, n.dot(light_dir));
    
    // Diffuse component
    Eigen::Vector3d diffuse = (material.kd.array() * light->I.array()).matrix() * n_dot_l;
    
    // Specular component (Blinn-Phong)
    Eigen::Vector3d half_vector = (light_dir + view_dir).normalized();
    double n_dot_h = std::max(0.0, n.dot(half_vector));
    double specular_factor = std::pow(n_dot_h, material.phong_exponent);
    Eigen::Vector3d specular = (material.ks.array() * light->I.array()).matrix() * specular_factor;
    
    color += diffuse + specular;
  }
  
  return color;
}
