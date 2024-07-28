#ifndef NAIVE_MATH_H
#define NAIVE_MATH_H

#include <corecrt_math.h>
#include <iostream>

struct Vec3f
{
	constexpr Vec3f() : x(0), y(0), z(0) {}
	constexpr Vec3f(const Vec3f& other) : x(other.x), y(other.y), z(other.z){}
	constexpr Vec3f(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		struct
		{
			float r;
			float g;
			float b;
		};
		struct
		{
			float u;
			float v;
			float w;
		};
		float e[3];
	};

	inline float operator[] (int i) const noexcept 
	{ 
		return e[i]; 
	}
	inline float& operator[] (int i) noexcept
	{
		return e[i];
	}
	
	inline Vec3f operator*(const Vec3f& v2) const noexcept
	{
		return { x * v2.x, y * v2.y, z * v2.z };
	}
	inline Vec3f operator+(const Vec3f& v2) const noexcept
	{
		return { x + v2.x, y + v2.y, z + v2.z };
	}
	inline Vec3f operator-(const Vec3f& v2) const noexcept
	{
		return { x - v2.x, y - v2.y, z - v2.z };
	}
	inline Vec3f operator-() const noexcept 
	{ 
		return { -x, -y, -z };
	}

	inline Vec3f operator*(float t) const noexcept
	{
		return { x * t, y * t, z * t };
	}

	inline Vec3f operator/(float t) const noexcept
	{
		return { x / t, y / t, z / t };
	}

	inline Vec3f& operator+= (const Vec3f& v2) noexcept
	{
		x += v2.x;
		y += v2.y;
		z += v2.z;
		return *this;
	}
	inline Vec3f& operator-= (const Vec3f& v2) noexcept
	{
		x -= v2.x;
		y -= v2.y;
		z -= v2.z;
		return *this;
	}
	inline Vec3f& operator*= (const Vec3f& v2) noexcept
	{
		x *= v2.x;
		y *= v2.y;
		z *= v2.z;
		return *this;
	}
	inline Vec3f& operator/= (const Vec3f& v2) noexcept
	{
		x /= v2.x;
		y /= v2.y;
		z /= v2.z;
		return *this;
	}
	inline Vec3f& operator*=(float t) noexcept
	{
		x *= t;
		y *= t;
		z *= t;
		return *this;
	}
	inline Vec3f& operator/=(float t) noexcept
	{
		t = 1.0f / t;
		x *= t;
		y *= t;
		z *= t;
		return *this;
	}

	friend Vec3f operator*(float t, const Vec3f& v) noexcept
	{
		return { v.x * t, v.y * t, v.z * t };
	}
	friend Vec3f operator/=(float t, const Vec3f& v) noexcept
	{
		return { v.x / t, v.y / t, v.z/t };
	}

	inline float length() const noexcept
	{
		return sqrtf(x * x + y * y + z * z);
	}
	inline float squared_length() const noexcept
	{
		return x * x + y * y + z * z;
	}
	inline void make_unit_vector()
	{
		const float k = 1.0f / sqrtf(x * x + y * y + z * z);
		x *= k;
		y *= k;
		z *= k;
	}

	friend std::istream& operator>>(std::istream& is, Vec3f& t)
	{
		is >> t.x >> t.y >> t.z;
		return is;
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec3f& t)
	{
		os << "[X], [Y], [Z]: [" << t.x << "], [" << t.y << "], [" << t.z << "]";
		return os;
	}
};

inline Vec3f unit_vector(const Vec3f& v) noexcept
{
	return v / v.length();
}
inline float dot(const Vec3f& v1, const Vec3f& v2) noexcept
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline Vec3f cross(const Vec3f& v1, const Vec3f& v2) noexcept
{
	return { (v1.y * v2.z - v1.z * v2.y), -(v1.x * v2.z - v1.z * v2.x), v1.x * v2.y - v1.y * v2.x };
}
inline Vec3f reflect(const Vec3f& v, const Vec3f& n) noexcept
{
	return v - 2.0f * dot(v, n) * n;
}
inline bool refract(const Vec3f& incidentRay, const Vec3f& normal, float refractiveIndexRatio, Vec3f& refractedRay) noexcept
{
	const Vec3f unitIncidentRay = unit_vector(incidentRay);
	const float dotIRN = dot(unitIncidentRay, normal);
	const float discriminant = 1.0f - refractiveIndexRatio * refractiveIndexRatio * (1.0f - dotIRN * dotIRN);

	if (discriminant > 0)
	{
		refractedRay = refractiveIndexRatio * (unitIncidentRay - normal * dotIRN) - normal * sqrtf(discriminant);
		return true;
	}
	else
	{
		return false;
	}
}

inline float schlick(float cosine, float refractionIndex) noexcept
{
	float r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
	r0 *= r0;

	return r0 + (1.0f - r0) * powf(1.0f - cosine, 5);
}

constexpr float toRadians(float degrees) noexcept
{
	return (3.141592653f / 180.0f) * degrees;
}

using vec3 = Vec3f;
#endif