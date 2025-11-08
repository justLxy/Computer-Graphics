// Create a bumpy surface by using procedural noise to generate a height (
// displacement in normal direction).
//
// Inputs:
//   is_moon  whether we're looking at the moon or centre planet
//   s  3D position of seed for noise generation
// Returns elevation adjust along normal (values between -0.1 and 0.1 are
//   reasonable.
float bump_height( bool is_moon, vec3 s)
{
  /////////////////////////////////////////////////////////////////////////////
  float height = 0.0;
  
  // Both moon and planet should have bumps
  if (is_moon) {
    // Moon: rougher, more crater-like.
    height += improved_perlin_noise(s * 5.0) * 0.05;
    height += improved_perlin_noise(s * 10.0) * 0.025;
    height += improved_perlin_noise(s * 20.0) * 0.015;
  } else {
    // Planet: smoother continents.
    height += improved_perlin_noise(s * 3.0) * 0.04;
    height += improved_perlin_noise(s * 6.0) * 0.02;
    height += improved_perlin_noise(s * 12.0) * 0.01;
  }
  
  return height;
  /////////////////////////////////////////////////////////////////////////////
}
