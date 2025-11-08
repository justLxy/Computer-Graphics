// Filter an input value to perform a smooth step. This function should be a
// cubic polynomial with smooth_step(0) = 0, smooth_step(1) = 1, and zero first
// derivatives at f=0 and f=1. 
//
// Inputs:
//   f  input value
// Returns filtered output value
float smooth_step( float f)
{
  /////////////////////////////////////////////////////////////////////////////
  // Cubic polynomial: 3f^2 - 2f^3
  return f * f * (3.0 - 2.0 * f);
  /////////////////////////////////////////////////////////////////////////////
}

vec3 smooth_step( vec3 f)
{
  /////////////////////////////////////////////////////////////////////////////
  // Apply smooth_step component-wise.
  return vec3(smooth_step(f.x), smooth_step(f.y), smooth_step(f.z));
  /////////////////////////////////////////////////////////////////////////////
}
