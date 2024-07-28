#ifndef MATERIAL_H
#define MATERIAL_H

#include "Ray.h"
#include "Random.h"

struct HitRegistry;

enum class MaterialType : uint8_t
{
	LAMBERTIAN,
	METALLIC,
	DIELECTRIC,
};

class Material
{
public:
	constexpr Material() noexcept {}

	void SetDieletric(float refractionIndex) noexcept
	{
		type = MaterialType::DIELECTRIC;
		RefractionIndex = refractionIndex;
	}

	void SetLambertian(const Vec3f& albedo) noexcept
	{
		type = MaterialType::LAMBERTIAN;
		Albedo = albedo;
	}

	void SetMetallic(const Vec3f& albedo, float fuzz) noexcept
	{
		type = MaterialType::METALLIC;
		Albedo = albedo;
		Fuzz = fminf(fuzz, 1.0f);
	}

	bool Scatter(const Ray& In, HitRegistry* rec, Vec3f& attenuation, Ray& scattered) const noexcept;
	Vec3f Albedo;
	float ScatterChance = 0.2f;
	float Fuzz = 1.0f;
	float RefractionIndex = 1.0f;
	MaterialType type =  MaterialType::LAMBERTIAN;
};

struct HitRegistry
{
	constexpr HitRegistry() {}

	float t = -1;
	vec3 p = { 0,0,0 };
	vec3 normal = { 0,0,0 };
	Material material;
};
#endif
