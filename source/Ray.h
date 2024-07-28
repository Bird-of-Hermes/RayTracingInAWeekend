#ifndef RAY_H
#define RAY_H

#include "NaiveMath.h"

struct Ray
{
	Vec3f PointAtT(float t) const noexcept
	{
		return origin + t * direction;
	}
	Vec3f origin;
	Vec3f direction;
};
#endif