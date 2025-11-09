#pragma once
#include <vector>
#include "cg_math.h"
#include "ray.h"
#include "obj_loader.h"
#include "bvh.h"

struct DirectionalLight {
	Vec3 dir = normalize(Vec3(-1, -1, -1));
	Vec3 color = {1.0, 0.95, 0.9};
	double intensity = 1.0;
};

struct Scene {
	Mesh mesh;
	BVH bvh;
	DirectionalLight sun;
	Vec3 ambient = {0.15,0.15,0.18};
	// Materials that should appear brighter regardless of light (e.g., painted sky plane)
	std::vector<int> emissive_mat_ids;
	double emissive_boost = 0.0; // added to color as kd*boost (0 disables)
	// UV offset for emissive materials (e.g., scrolling clouds on background board)
	Vec2 emissive_uv_offset = {0.0, 0.0};
	// Emissive rendering controls (for bright sky clouds)
	double emissive_gain = 1.0;            // multiply emissive kd
	double emissive_exposure_mul = 1.0;    // extra exposure only for emissive path
	bool emissive_linear = false;          // if true, bypass filmic tonemap for emissive
	// Global shading controls
	double exposure = 1.8;          // multiplies color before tone mapping
	double specular_strength = 0.5; // multiplier for specular highlight
	double shadow_ambient = 0.25;   // base in-shadow brightness as fraction of kd
	// Simple hemispheric skylight fill (adds light from above to brighten ground)
	double sky_fill = 0.0;          // strength [0,inf), 0 disables
	Vec3 sky_tint = {0.6,0.75,0.95}; // color tint of skylight
	
	void set_mesh(Mesh&& m){
		mesh = std::move(m);
		bvh.build(mesh.triangles);
	}

	// Shade at ray intersection using lambert + Blinn-Phong and texture if available
	Vec3 shade(const Ray& ray, int triIdx, const Vec3& n, const Vec2& uv, double b0,double b1,double b2) const;
	bool trace_shadow(const Vec3& p, const Vec3& lightDir) const;
};


