// Compute Blinn-Phong Shading given a material specification, a point on a
// surface and a light direction. Assume the light is white and has a low
// ambient intensity.
//
// Inputs:
//   ka  rgb ambient color
//   kd  rgb diffuse color
//   ks  rgb specular color
//   p  specular exponent (shininess)
//   n  unit surface normal direction
//   v  unit direction from point on object to eye
//   l  unit light direction
// Returns rgb color
vec3 blinn_phong(
  vec3 ka,
  vec3 kd,
  vec3 ks,
  float p,
  vec3 n,
  vec3 v,
  vec3 l)
{
  /////////////////////////////////////////////////////////////////////////////
  // Blinn-Phong shading model.
  vec3 light_color = vec3(1,1,1);
  float ambient_intensity = 0.1;
  
  vec3 ambient = ka * ambient_intensity;
  
  float diffuse_intensity = max(0.0, dot(n, l));
  vec3 diffuse = kd * light_color * diffuse_intensity;
  
  vec3 h = normalize(v + l);
  float specular_intensity = pow(max(0.0, dot(n, h)), p);
  vec3 specular = ks * light_color * specular_intensity;
  
  return ambient + diffuse + specular;
  /////////////////////////////////////////////////////////////////////////////
}


