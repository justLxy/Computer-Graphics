#include "scene.h"
#include <algorithm>

bool Scene::trace_shadow(const Vec3& p, const Vec3& lightDir) const{
	Ray sr;
	sr.origin = p + 1e-4 * lightDir;
	sr.dir = lightDir;
	for(int step=0; step<64; ++step){
		int hit; double t; Vec3 n; Vec2 uv; double b0,b1,b2;
		if(!bvh.intersect(mesh.triangles, sr, hit, t, n, uv, b0,b1,b2)) return false;
		// Ignore emissive/background materials when casting shadows so the sky plate
		// does not receive or cast shadows.
		const Triangle& tri = mesh.triangles[(size_t)hit];
		if(!emissive_sky_mat_ids.empty() &&
		   std::find(emissive_sky_mat_ids.begin(), emissive_sky_mat_ids.end(), tri.material_id) != emissive_sky_mat_ids.end()){
			// Advance past this triangle and continue searching
			sr.origin = sr.origin + sr.dir * (t + 1e-4);
			continue;
		}
		return true;
	}
	// Safety: if too many steps, assume occluded
	return true;
}

Vec3 Scene::shade(const Ray& ray, int triIdx, const Vec3& n, const Vec2& uv, double b0,double b1,double b2) const{
	const Triangle& t = mesh.triangles[(size_t)triIdx];
	Vec3 kd = {0.7,0.7,0.7};
	int tex_id = -1;
	if(t.material_id>=0 && t.material_id < (int)mesh.materials.size()){
		const Material& m = mesh.materials[(size_t)t.material_id];
		kd = m.kd;
		if(m.tex_id>=0 && m.tex_id < (int)mesh.textures.size()){
			tex_id = m.tex_id;
			kd = mesh.textures[(size_t)tex_id].sample(uv);
		}
	}
	const bool isSkyEmissive = !emissive_sky_mat_ids.empty() &&
		(std::find(emissive_sky_mat_ids.begin(), emissive_sky_mat_ids.end(), t.material_id) != emissive_sky_mat_ids.end());
	const bool isWindowEmissive = !emissive_window_mat_ids.empty() &&
		(std::find(emissive_window_mat_ids.begin(), emissive_window_mat_ids.end(), t.material_id) != emissive_window_mat_ids.end());
	// Lighting
	Vec3 L = normalize(-sun.dir);
	Vec3 V = normalize(-ray.dir);
	Vec3 N = normalize(n);
	Vec3 light_col = sun.color * sun.intensity;
	Vec3 col = ambient * kd;
	// If this is a background/emissive plate (e.g., sky board), render it unlit and
	// ignore occluders to prevent scene silhouettes from appearing on the sky.
	if(isSkyEmissive){
		// Scroll texture coordinates for emissive materials (e.g., drifting clouds)
		if(tex_id>=0){
			Vec2 uv2 = uv + emissive_uv_offset;
			kd = mesh.textures[(size_t)tex_id].sample(uv2);
		}
		const double base = 0.7; // self-illumination base
		Vec3 e = kd * (base + std::max(0.0, emissive_boost)) * std::max(0.0, emissive_gain);
		if(emissive_linear){
			return clamp01(e);
		}else{
			// Filmic tone mapping (Hable/Uncharted2) with white point 11.2
			auto uncharted2 = [](double x)->double{
				const double A=0.15, B=0.50, C=0.10, D=0.20, E=0.02, F=0.30;
				return ((x*(A*x + C*B) + D*E) / (x*(A*x + B) + D*F)) - E/F;
			};
			const double white = 11.2;
			const double whiteScale = 1.0 / uncharted2(white);
			Vec3 x = e * std::max(0.0, exposure * emissive_exposure_mul);
			Vec3 mapped = {
				uncharted2(x.x) * whiteScale,
				uncharted2(x.y) * whiteScale,
				uncharted2(x.z) * whiteScale
			};
			return clamp01(mapped);
		}
	}
	// If window/light emissive material, render self-illumination (no shadowing)
	if(isWindowEmissive){
		const double base = 0.7;
		Vec3 e = kd * (base + std::max(0.0, window_emissive_boost)) * std::max(0.0, window_emissive_gain);
		if(window_emissive_linear){
			return clamp01(e);
		}else{
			auto uncharted2 = [](double x)->double{
				const double A=0.15, B=0.50, C=0.10, D=0.20, E=0.02, F=0.30;
				return ((x*(A*x + C*B) + D*E) / (x*(A*x + B) + D*F)) - E/F;
			};
			const double white = 11.2;
			const double whiteScale = 1.0 / uncharted2(white);
			Vec3 x = e * std::max(0.0, exposure * window_emissive_exposure_mul);
			Vec3 mapped = {
				uncharted2(x.x) * whiteScale,
				uncharted2(x.y) * whiteScale,
				uncharted2(x.z) * whiteScale
			};
			return clamp01(mapped);
		}
	}
	// Shadow test
	// Compute actual hitPoint
	const Vec3 hp = t.p0*b0 + t.p1*b1 + t.p2*b2;
	const bool inShadow = trace_shadow(hp, L);
	if(!inShadow){
		double ndotl = std::max(0.0, dot(N, L));
		Vec3 diffuse = kd * ndotl;
		Vec3 H = normalize(L + V);
		double ndoth = std::max(0.0, dot(N, H));
		double spec = std::pow(ndoth, 32.0);
		Vec3 specular = Vec3{1.0,1.0,1.0} * spec * specular_strength;
		col += (diffuse + specular) * light_col;
	}else{
		col += kd * shadow_ambient; // brighter base in shadow
	}
	// Hemispheric skylight fill (brightens upward-facing surfaces)
	if(sky_fill > 0.0){
		double hemi = std::max(0.0, N.y); // 0 for downward, 1 for up
		col += (kd * sky_tint) * (sky_fill * hemi);
	}
	// Emissive boost for designated materials (e.g., background sky board)
	if(!emissive_sky_mat_ids.empty()){
		if(std::find(emissive_sky_mat_ids.begin(), emissive_sky_mat_ids.end(), t.material_id) != emissive_sky_mat_ids.end()){
			col += kd * std::max(0.0, emissive_boost);
		}
	}
	// Filmic tone mapping (Hable/Uncharted2) with white point 11.2
	auto uncharted2 = [](double x)->double{
		const double A=0.15, B=0.50, C=0.10, D=0.20, E=0.02, F=0.30;
		return ((x*(A*x + C*B) + D*E) / (x*(A*x + B) + D*F)) - E/F;
	};
	const double white = 11.2;
	const double whiteScale = 1.0 / uncharted2(white);
	Vec3 x = col * std::max(0.0, exposure);
	Vec3 mapped = {
		uncharted2(x.x) * whiteScale,
		uncharted2(x.y) * whiteScale,
		uncharted2(x.z) * whiteScale
	};
	return clamp01(mapped);
}


