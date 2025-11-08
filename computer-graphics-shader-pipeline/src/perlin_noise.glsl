// Given a 3d position as a seed, compute a smooth procedural noise
// value: "Perlin Noise", also known as "Gradient noise".
//
// Inputs:
//   st  3D seed
// Returns a smooth value between (-1,1)
//
// expects: random_direction, smooth_step
float perlin_noise( vec3 st) 
{
  /////////////////////////////////////////////////////////////////////////////
  vec3 i = floor(st);
  vec3 f = fract(st);
  
  vec3 u = smooth_step(f);
  
  // Sample random gradients at 8 corners of the cube
  vec3 c000 = random_direction(i + vec3(0,0,0));
  vec3 c100 = random_direction(i + vec3(1,0,0));
  vec3 c010 = random_direction(i + vec3(0,1,0));
  vec3 c110 = random_direction(i + vec3(1,1,0));
  vec3 c001 = random_direction(i + vec3(0,0,1));
  vec3 c101 = random_direction(i + vec3(1,0,1));
  vec3 c011 = random_direction(i + vec3(0,1,1));
  vec3 c111 = random_direction(i + vec3(1,1,1));
  
  // Compute dot products with distance vectors
  float v000 = dot(c000, f - vec3(0,0,0));
  float v100 = dot(c100, f - vec3(1,0,0));
  float v010 = dot(c010, f - vec3(0,1,0));
  float v110 = dot(c110, f - vec3(1,1,0));
  float v001 = dot(c001, f - vec3(0,0,1));
  float v101 = dot(c101, f - vec3(1,0,1));
  float v011 = dot(c011, f - vec3(0,1,1));
  float v111 = dot(c111, f - vec3(1,1,1));
  
  // Trilinear interpolation of dot products.
  float v00 = mix(v000, v100, u.x);
  float v10 = mix(v010, v110, u.x);
  float v01 = mix(v001, v101, u.x);
  float v11 = mix(v011, v111, u.x);
  
  float v0 = mix(v00, v10, u.y);
  float v1 = mix(v01, v11, u.y);
  
  return mix(v0, v1, u.z);
  /////////////////////////////////////////////////////////////////////////////
}

