// Generate a pseudorandom unit 3D vector
// 
// Inputs:
//   seed  3D seed
// Returns psuedorandom, unit 3D vector drawn from uniform distribution over
// the unit sphere (assuming random2 is uniform over [0,1]²).
//
// expects: random2.glsl, PI.glsl
vec3 random_direction( vec3 seed)
{
  /////////////////////////////////////////////////////////////////////////////
  vec2 rand = random2(seed);
  float theta = 2.0 * M_PI * rand.x;
  float phi = acos(2.0 * rand.y - 1.0);
  
  float x = sin(phi) * cos(theta);
  float y = sin(phi) * sin(theta);
  float z = cos(phi);
  
  return vec3(x, y, z);
  /////////////////////////////////////////////////////////////////////////////
}
