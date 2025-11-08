// Generate a procedural planet and orbiting moon. Use layers of (improved)
// Perlin noise to generate planetary features such as vegetation, gaseous
// clouds, mountains, valleys, ice caps, rivers, oceans. Don't forget about the
// moon. Use `animation_seconds` in your noise input to create (periodic)
// temporal effects.
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
// expects: model, blinn_phong, bump_height, bump_position,
// improved_perlin_noise, tangent
void main()
{
  /////////////////////////////////////////////////////////////////////////////
  vec3 p = sphere_fs_in;
  
  // Compute bumped normal
  vec3 T, B;
  tangent(normalize(p), T, B);
  float eps = 0.0001;
  vec3 p_center = bump_position(is_moon, p);
  vec3 p_T = bump_position(is_moon, p + eps * T);
  vec3 p_B = bump_position(is_moon, p + eps * B);
  vec3 dT = (p_T - p_center) / eps;
  vec3 dB = (p_B - p_center) / eps;
  vec3 bumped_normal = normalize(cross(dT, dB));
  
  // Transform normal to view space
  mat4 m = model(is_moon, animation_seconds);
  vec3 normal_view = normalize((transpose(inverse(view * m)) * vec4(bumped_normal, 0.0)).xyz);
  
  // Lighting setup
  float light_angle = animation_seconds * 2.0 * M_PI / 8.0;
  vec3 light_pos = vec3(4.0 * cos(light_angle), 2.0, 4.0 * sin(light_angle));
  vec3 light_dir = normalize((view * vec4(light_pos, 1.0)).xyz - view_pos_fs_in.xyz);
  vec3 view_dir = normalize(-view_pos_fs_in.xyz);
  
  // Material properties
  vec3 ka, kd, ks;
  float phong_exp;
  
  if (is_moon) {
    // Moon: gray with craters and animated noise.
    float crater_noise = improved_perlin_noise(p * 8.0 + vec3(animation_seconds * 0.1));
    float detail = improved_perlin_noise(p * 20.0);
    vec3 moon_color = mix(vec3(0.4, 0.4, 0.4), vec3(0.7, 0.7, 0.7), 0.5 + 0.5 * crater_noise);
    moon_color += vec3(detail * 0.1);
    
    ka = moon_color * 0.2;
    kd = moon_color * 0.7;
    ks = vec3(0.2, 0.2, 0.2);
    phong_exp = 50.0;
  } else {

    float continent_base = improved_perlin_noise(p * 1.2);
    float continent_shape = improved_perlin_noise(p * 2.5);
    float continent_detail = improved_perlin_noise(p * 5.0);
    
    float landmass = continent_base * 0.5 + continent_shape * 0.35 + continent_detail * 0.15;
    bool is_ocean = landmass < 0.05;
    
    float elevation = improved_perlin_noise(p * 8.0);
    float micro_detail = improved_perlin_noise(p * 20.0);
    float terrain_height = elevation * 0.7 + micro_detail * 0.3;
    
    vec3 base_color;
    if (is_ocean) {
      // Realistic ocean with depth variation and wave shimmer.
      float ocean_depth = landmass + 0.1;
      vec3 deep_ocean = vec3(0.01, 0.05, 0.15);
      vec3 shallow_ocean = vec3(0.05, 0.15, 0.35);
      base_color = mix(deep_ocean, shallow_ocean, clamp(ocean_depth * 3.0, 0.0, 1.0));
      
      float wave = improved_perlin_noise(p * 40.0 + vec3(animation_seconds * 2.0, 0, 0)) * 0.05;
      base_color += vec3(wave);
    } 
    else {
      // Latitude-based climate.
      float latitude = abs(p.y);
      float temperature = 1.0 - latitude;
      float moisture = improved_perlin_noise(p * 6.0) * 0.5 + 0.5;
      
      // Biome colors
      vec3 tropical_forest = vec3(0.05, 0.35, 0.08);
      vec3 temperate_forest = vec3(0.15, 0.45, 0.12);
      vec3 grassland = vec3(0.35, 0.55, 0.20);
      vec3 desert = vec3(0.76, 0.70, 0.50);
      vec3 tundra = vec3(0.60, 0.65, 0.55);
      vec3 mountain_rock = vec3(0.40, 0.35, 0.30);
      vec3 mountain_snow = vec3(0.85, 0.88, 0.90);
      
      // Determine biome based on climate and elevation.
      vec3 biome_color;
      if (terrain_height > 0.5) {
        biome_color = mix(mountain_rock, mountain_snow, (terrain_height - 0.7) * 3.0);
      }
      else if (temperature > 0.7 && moisture > 0.5) {
        biome_color = tropical_forest;
      }
      else if (temperature > 0.6 && moisture < 0.4) {
        biome_color = desert;
      }
      else if (temperature < 0.3) {
        biome_color = tundra;
      }
      else if (moisture > 0.5) {
        biome_color = temperate_forest;
      }
      else {
        biome_color = grassland;
      }
      
      base_color = biome_color + vec3(micro_detail * 0.1);
    }
    
    float cloud_large = improved_perlin_noise(p * 3.0 + vec3(animation_seconds * 0.2, 0, 0));
    float cloud_medium = improved_perlin_noise(p * 8.0 + vec3(animation_seconds * 0.35, 0, 0));
    float cloud_detail = improved_perlin_noise(p * 18.0 + vec3(animation_seconds * 0.5, 0, 0));
    
    float cloud_density = cloud_large * 0.5 + cloud_medium * 0.3 + cloud_detail * 0.2;
    float clouds = improved_smooth_step(clamp((cloud_density - 0.3) / (0.8 - 0.3), 0.0, 1.0));
    
    // Blend clouds over surface.
    vec3 cloud_color = vec3(0.98, 0.98, 1.0);
    base_color = mix(base_color, cloud_color, clouds * 0.75);
    
    ka = base_color * 0.15;
    kd = base_color * 0.75;
    
    if (is_ocean) {
      // High specularity for water.
      ks = vec3(0.9, 0.9, 1.0);
      phong_exp = 300.0;
    } else if (clouds > 0.5) {
      // Soft specularity for clouds.
      ks = vec3(0.4, 0.4, 0.4);
      phong_exp = 80.0;
    } else {
      // Low specularity for land.
      ks = vec3(0.15, 0.15, 0.15);
      phong_exp = 20.0;
    }
  }
  
  color = blinn_phong(ka, kd, ks, phong_exp, normal_view, view_dir, light_dir);
  /////////////////////////////////////////////////////////////////////////////
}
