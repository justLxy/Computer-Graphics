#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"

bool raycolor(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector< std::shared_ptr<Light> > & lights,
  const int num_recursive_calls,
  Eigen::Vector3d & rgb)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  ////////////////////////////////////////////////////////////////////////////
  
  // Maximum recursion depth
  const int max_recursion_depth = 5;
  
  // Initialize color
  rgb = Eigen::Vector3d(0,0,0);
  
  // Check for hit in the scene
  int hit_id;
  double t;
  Eigen::Vector3d n;
  
  bool hit_found = first_hit(ray, min_t, objects, hit_id, t, n);
  
  if (!hit_found) {
    return false;
  }
  
  // Compute Blinn-Phong shading for the hit
  rgb = blinn_phong_shading(ray, hit_id, t, n, objects, lights);
  
  // Handle reflections if the material has mirror properties and we haven't exceeded recursion limit
  if (num_recursive_calls < max_recursion_depth) {
    const Material& material = *(objects[hit_id]->material);
    
    // Check if the material has reflective properties
    double reflection_strength = material.km.norm();
    if (reflection_strength > 0) {
      // Compute hit point
      Eigen::Vector3d hit_point = ray.origin + t * ray.direction;
      
      // Compute reflected direction
      Eigen::Vector3d reflected_dir = reflect(ray.direction.normalized(), n);
      
      // Create reflected ray
      Ray reflected_ray;
      reflected_ray.origin = hit_point;
      reflected_ray.direction = reflected_dir;
      
      // Recursively trace the reflected ray
      Eigen::Vector3d reflected_color;
      double epsilon = 1e-10;
      if (raycolor(reflected_ray, epsilon, objects, lights, num_recursive_calls + 1, reflected_color)) {
        // Add reflected color weighted by the mirror coefficient
        rgb += (material.km.array() * reflected_color.array()).matrix();
      }
    }
  }
  
  return true;
}
