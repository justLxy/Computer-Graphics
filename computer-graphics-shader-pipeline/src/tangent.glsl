// Input:
//   N  3D unit normal vector
// Outputs:
//   T  3D unit tangent vector
//   B  3D unit bitangent vector
void tangent(in vec3 N, out vec3 T, out vec3 B)
{
  /////////////////////////////////////////////////////////////////////////////
  // Find an orthogonal tangent and bitangent to the normal N.
  vec3 up = abs(N.y) < 0.999 ? vec3(0, 1, 0) : vec3(1, 0, 0);
  T = normalize(cross(up, N));
  B = normalize(cross(N, T));
  /////////////////////////////////////////////////////////////////////////////
}
