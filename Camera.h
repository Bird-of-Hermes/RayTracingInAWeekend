#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

struct Camera
{
public:
	constexpr Camera() noexcept {}
	Camera(const Vec3f& lookFrom, const Vec3f& lookAt, const Vec3f& Up, float aspectRatio = 16.0f/9.0f, float fov = 70.0f, float aperture = 1.0f, float focusDist = 1.0f) noexcept :
		origin(lookFrom)
	{
		lensRadius = aperture / 2;
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(Up, w));
		v = cross(w, u);

		float half_height = tanf(toRadians(fov) / 2.0f);
		float half_width = aspectRatio * half_height;
		lowerLeftCorner = origin - half_width * focusDist * u - half_height* focusDist * v - focusDist * w;
		horizontal = 2.0f * half_width * focusDist * u;
		vertical = 2.0f * half_height * focusDist * v;
	}


	Ray GetRay(float s, float t) const noexcept
	{
		const Vec3f rd = lensRadius * RandomInUnitDisk();
		const Vec3f offset = u * rd.x + v * rd.y;
		return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset );
	}

	Vec3f origin;
	Vec3f lowerLeftCorner;
	Vec3f horizontal;
	Vec3f vertical;
	Vec3f u, v, w;
	float lensRadius = 1.0f;
};

#endif
