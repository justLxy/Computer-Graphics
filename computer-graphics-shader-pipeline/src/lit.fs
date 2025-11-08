// Add (hard code) an orbiting (point or directional) light to the scene. Light
// the scene using the Blinn-Phong Lighting Model.
//
// Uniforms:
uniform mat4 view;
uniform mat4 proj;
uniform float animation_seconds;
uniform bool is_moon;
// Inputs:
in vec3 sphere_fs_in;
in vec3 normal_fs_in;
in vec4 pos_fs_in; 
in vec4 view_pos_fs_in; 
// Outputs:
out vec3 color;
// expects: PI, blinn_phong
void main()
{
  /////////////////////////////////////////////////////////////////////////////
  // Light the scene with an orbiting light using the Blinn-Phong model.
  float light_angle = animation_seconds * 2.0 * M_PI / 8.0;
  vec3 light_pos = vec3(4.0 * cos(light_angle), 2.0, 4.0 * sin(light_angle));
  vec3 light_dir = normalize((view * vec4(light_pos, 1.0)).xyz - view_pos_fs_in.xyz);
  
  // View direction (camera is at origin in view space)
  vec3 view_dir = normalize(-view_pos_fs_in.xyz);
  
  // Material properties
  vec3 ka, kd, ks;
  float phong_exp;
  if (is_moon) {
    ka = vec3(0.1, 0.1, 0.1);
    kd = vec3(0.5, 0.5, 0.5);
    ks = vec3(0.6, 0.6, 0.6);
    phong_exp = 1000.0;
  } else {
    ka = vec3(0.0, 0.0, 0.2);
    kd = vec3(0.0, 0.0, 0.8);
    ks = vec3(0.8, 0.8, 0.8);
    phong_exp = 1000.0;
  }
  
  color = blinn_phong(ka, kd, ks, phong_exp, normalize(normal_fs_in), view_dir, light_dir);
  /////////////////////////////////////////////////////////////////////////////
}
