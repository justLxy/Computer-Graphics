// Set the pixel color using Blinn-Phong shading (e.g., with constant blue and
// gray material color) with a bumpy texture.
// 
// Uniforms:
uniform mat4 view;
uniform mat4 proj;
uniform float animation_seconds;
uniform bool is_moon;
// Inputs:
//                     linearly interpolated from tessellation evaluation shader
//                     output
in vec3 sphere_fs_in;
in vec3 normal_fs_in;
in vec4 pos_fs_in; 
in vec4 view_pos_fs_in; 
// Outputs:
//               rgb color of this pixel
out vec3 color;
// expects: model, blinn_phong, bump_height, bump_position,
// improved_perlin_noise, tangent
void main()
{
  /////////////////////////////////////////////////////////////////////////////
  vec3 p = sphere_fs_in;
  
  vec3 T, B;
  tangent(normalize(p), T, B);
  
  float eps = 0.0001;
  
  vec3 p_center = bump_position(is_moon, p);
  vec3 p_T = bump_position(is_moon, p + eps * T);
  vec3 p_B = bump_position(is_moon, p + eps * B);
  
  vec3 dT = (p_T - p_center) / eps;
  vec3 dB = (p_B - p_center) / eps;
  vec3 bumped_normal = normalize(cross(dT, dB));
  
  // Transform normal to view space.
  mat4 m = model(is_moon, animation_seconds);
  vec3 normal_view = normalize((transpose(inverse(view * m)) * vec4(bumped_normal, 0.0)).xyz);
  
  // Set up lighting
  float light_angle = animation_seconds * 2.0 * M_PI / 8.0;
  vec3 light_pos = vec3(4.0 * cos(light_angle), 2.0, 4.0 * sin(light_angle));
  vec3 light_dir = normalize((view * vec4(light_pos, 1.0)).xyz - view_pos_fs_in.xyz);
  vec3 view_dir = normalize(-view_pos_fs_in.xyz);
  
  // Material properties
  vec3 ka, kd, ks;
  float phong_exp;
  if (is_moon) {
    ka = vec3(0.1, 0.1, 0.1);
    kd = vec3(0.5, 0.5, 0.5);
    ks = vec3(0.6, 0.6, 0.6);
    phong_exp = 100.0;
  } else {
    ka = vec3(0.0, 0.0, 0.2);
    kd = vec3(0.0, 0.0, 0.8);
    ks = vec3(0.8, 0.8, 0.8);
    phong_exp = 100.0;
  }
  
  color = blinn_phong(ka, kd, ks, phong_exp, normal_view, view_dir, light_dir);
  /////////////////////////////////////////////////////////////////////////////
}
