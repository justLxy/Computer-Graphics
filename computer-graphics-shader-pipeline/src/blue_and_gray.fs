// Set the pixel color to blue or gray depending on is_moon.
//
// Uniforms:
uniform bool is_moon;
// Outputs:
out vec3 color;
void main()
{
  /////////////////////////////////////////////////////////////////////////////
  // Set color to blue for planet, gray for moon.
  // Use mix to avoid branching: mix(blue, gray, is_moon)
  vec3 blue = vec3(0.0, 0.0, 1.0);
  vec3 gray = vec3(0.5, 0.5, 0.5);
  color = mix(blue, gray, float(is_moon));
  /////////////////////////////////////////////////////////////////////////////
}
