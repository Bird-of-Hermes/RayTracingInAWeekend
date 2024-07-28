#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Material.h"

class Sphere final : public Hittable
{
public:
	constexpr Sphere(float r = 1.0f, const Vec3f& pos = Vec3f(), const Material& mat = Material()) : center(pos), radius(r)
	{
		material = mat;
	}
	~Sphere(){}

	bool HIT(const Ray& r, HitRegistry* rec, float t_min, float t_max) const noexcept override
	{
		Vec3f oc = r.origin - center;
		float a = dot(r.direction, r.direction);
		float b = dot(oc, r.direction);
		float c = dot(oc,oc) - radius*radius;
		float discriminant = b * b - a * c;

		if (discriminant > 0)
		{
			const float sqrtD = sqrtf(discriminant);
			float t = (-b - sqrtD) / a;
			if (t < t_max && t > t_min)
			{
				rec->t = t;
				rec->p = r.PointAtT(t);
				rec->normal = (rec->p - center) / radius;
				rec->material = material;

				return true;
			}

			t = (-b + sqrtD) / a;
			if (t < t_max && t > t_min)
			{
				rec->t = t;
				rec->p = r.PointAtT(t);
				rec->normal = (rec->p - center) / radius;
				rec->material = material;

				return true;
			}
		}
		return false;
	}

	Vec3f center;
	float radius;
};

#endif