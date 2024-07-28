#ifndef HITTABLE_H
#define HITTABLE_H

#include <vector>
#include "Ray.h"
#include "Random.h"
#include "Material.h"

struct HitRegistry;

class Hittable
{
public:
	virtual bool HIT(const Ray& r, HitRegistry* rec, float t_min = 0, float t_max = 10000.0f) const noexcept = 0;
	virtual ~Hittable() {};

	Material material;
};

#endif