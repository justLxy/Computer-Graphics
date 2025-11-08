// Construct the model transformation matrix. The moon should orbit around the
// origin. The other object should stay still.
//
// Inputs:
//   is_moon  whether we're considering the moon
//   time  seconds on animation clock
// Returns affine model transformation as 4x4 matrix
//
// expects: identity, rotate_about_y, translate, PI
mat4 model(bool is_moon, float time)
{
  /////////////////////////////////////////////////////////////////////////////
  // Moon orbits the origin; planet stays still.
  // Use I + is_moon * (T - I) to avoid branching
  float angle = 2.0 * M_PI * time / 4.0;
  mat4 T = rotate_about_y(angle) * translate(vec3(2.0, 0, 0)) * uniform_scale(0.3);
  mat4 I = identity();
  return I + float(is_moon) * (T - I);
  /////////////////////////////////////////////////////////////////////////////
}
