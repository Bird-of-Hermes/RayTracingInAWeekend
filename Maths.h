#ifndef MATHS_H
#define MATHS_H

//	Disclaimer: Parts of this file were straight-up copied from DirectX::Math lib,
//  other parts were improved upon (the lib) and others parts were developed from
//  scrath, below is the required MIT License notice

//	MIT License
//	
//	Copyright(c) Microsoft Corporation.
//	
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files(the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions :
//	
//	The above copyright notice and this permission notice shall be included in all
//	copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//	SOFTWARE

#pragma warning(push)
#pragma warning(disable: 4201)	// VS complaints

#include <math.h>
#include <utility>
#include <array>
#include <vector>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <memory>
#include <iostream>

#ifndef NOMINMAX
#define NOMINMAX
#endif

static constexpr float EPSILON = 0.005f;
static constexpr float PI = 3.14159265f;
static constexpr float PIDIV2 = 1.5707963f;
static constexpr float PIDIV4 = 0.7853982f;

__forceinline float FastSqrt(const float& x)
{
	union
	{
		float x;
		int i;
	} u{};
	u.x = x;
	u.i = (1 << 29) + (u.i >> 1) - (1 << 22);

	const float xhalf = 0.5f * u.x;
	u.x = u.x * (1.5f - xhalf * u.x * u.x); // Newton-Raphson 1st iteration
	u.x = u.x * (1.5f - xhalf * u.x * u.x); // Newton-Raphson 2nd iteration

	return u.x;
}
__forceinline bool Equals(float a, float b)
{
	if (fabsf(a - b) < EPSILON)
		return true;
	else
		return false;
}
__forceinline float Clamp(float val, float minVal, float maxVal) noexcept
{
	using namespace std;
	return max(minVal, min(val, maxVal));
};
__forceinline float Lerp(float v0, float v1, float t) 
{
	return (1 - t) * v0 + t * v1;
}

template <typename T>
concept NumericType = std::is_floating_point_v<T> || std::is_integral_v<T>;

template <typename NumericType = float>
struct Vec2
{
	constexpr Vec2() : x(0), y(0) {}
	constexpr Vec2(NumericType X, NumericType Y) : x(X), y(Y) {}

	inline Vec2<NumericType> operator=(const DirectX::XMFLOAT2& other) noexcept { return Vec2<NumericType>{other.x, other.y}; }
	inline Vec2<NumericType> operator-() const noexcept { return { -x, -y }; }
	inline Vec2<NumericType> operator-(const Vec2<NumericType>& other) noexcept
	{
		return { x - other.x, y - other.y};
	}
	inline Vec2<NumericType> operator+(const Vec2<NumericType>& other) const noexcept { return { x + other.x, y + other.y}; }
	inline Vec2<NumericType>& operator-=(const Vec2<NumericType>& other) noexcept
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	inline Vec2<NumericType>& operator+=(const Vec2<NumericType>& other) noexcept
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	inline Vec2<NumericType>& operator*=(float value) noexcept
	{
		x *= value;
		y *= value;
		return *this;
	}
	inline Vec2<NumericType>& operator*=(const Vec2<NumericType>& other) noexcept
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}
	inline Vec2<NumericType> operator*(const Vec2<NumericType>& other) const noexcept
	{
		return Vec2<NumericType>{x* other.x, y* other.y};
	}
	inline Vec2<NumericType> operator*(float value) const noexcept
	{
		return Vec2<NumericType>{x* value, y* value};
	}

	union
	{
		NumericType x;
		NumericType u;
	};
	union
	{
		NumericType y;
		NumericType v;
	};

	friend std::wostream& operator<<(std::wostream& os, const Vec2& pos)
	{
		os << L"(X: " << pos.x << L", Y: " << pos.y << L")";
		return os;
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec2<NumericType>& pos)
	{
		os << "(X: " << pos.x << ", Y: " << pos.y << ")";
		return os;
	}
};

template <typename NumericType = float>
struct Vec3
{
	constexpr Vec3() : x(0), y(0), z(0) {}
	constexpr Vec3(NumericType X, NumericType Y, NumericType Z) : x(X), y(Y), z(Z) {}

	inline Vec3<NumericType> operator=(const DirectX::XMFLOAT3& other) { return Vec3<NumericType>{other.x, other.y, other.z}; }
	inline Vec3<NumericType> operator-(const Vec3<NumericType>& other) noexcept
	{
		return {x - other.x, y - other.y, z - other.z};
	}
	inline Vec3<NumericType> operator+(const Vec3<NumericType>& other) const noexcept { return { x+other.x, y+other.y, z+other.z}; }
	inline Vec3<NumericType> operator-() const noexcept { return { -x,-y,-z }; }
	inline Vec3<NumericType> operator-(const Vec3<NumericType>& other) const noexcept { return { x - other.x, y - other.y, z - other.z }; }
	inline Vec3<NumericType>& operator-=(const Vec3<NumericType>& other) noexcept
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	inline Vec3<NumericType>& operator+=(const Vec3<NumericType>& other) noexcept
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	inline Vec3<NumericType>& operator*=(float value) noexcept
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}
	inline Vec3<NumericType>& operator*=(const Vec3<NumericType>& other) noexcept 
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this; 
	}
	inline Vec3<NumericType> operator*(const Vec3<NumericType>& other) const noexcept
	{
		return Vec3<NumericType>{x * other.x, y * other.y, z * other.z};
	}
	inline Vec3<NumericType> operator*(float value) const noexcept
	{
		return Vec3<NumericType>{x * value, y * value, z * value};
	}

	friend Vec3<NumericType> operator*(float value, const Vec3<NumericType>& other) noexcept
	{
		return other * value;
	}

	union
	{
		struct
		{
			NumericType x;
			NumericType y;
			NumericType z;
		};
		struct
		{
			NumericType r;
			NumericType g;
			NumericType b;
		};
		struct
		{
			NumericType u;
			NumericType v;
			NumericType w;
		};
	};
	
	

	friend std::ostream& operator<<(std::ostream& os, const Vec3<NumericType>& pos)
	{
		os << "(X: " << pos.x << ", Y: " << pos.y << ", Z: " << pos.z << ")";
		return os;
	}

	friend std::wostream& operator<<(std::wostream& os, const Vec3& pos) 
	{
		os << L"(X: " << pos.x << L", Y: " << pos.y << L", Z: " << pos.z << L")";
		return os;
	}
};

template <typename NumericType = float>
struct Vec4
{
	constexpr Vec4() : x(0), y(0), z(0), w(0) {}
	constexpr Vec4(NumericType X, NumericType Y, NumericType Z, NumericType W) : x(X), y(Y), z(Z), w(W) {}

	inline Vec4<NumericType> operator-() const noexcept { return { -x, -y, -z, -w }; }

	union
	{
		NumericType x;
		NumericType r;
	};
	union
	{
		NumericType y;
		NumericType g;
	};
	union
	{
		NumericType z;
		NumericType b;
	};
	union
	{
		NumericType w;
		NumericType a;
	};
};

using Vec2i = Vec2<int32_t>;
using Vec2u = Vec2<uint32_t>;
using Vec2i64 = Vec2<int64_t>;
using Vec2u64 = Vec2<uint64_t>;
using Vec2f = Vec2<float>;

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;
using Vec3i = Vec3<int32_t>;
using Vec3i64 = Vec3<int64_t>;
using Vec3u64 = Vec3<uint64_t>;

using Vec4f = Vec4<float>;

namespace Tuple
{
	struct Vector
	{
		union
		{
			__m128 v_XYZW;
			float xyzw[4];
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
		};

		// assume it's a point by default
		Vector(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) noexcept;
		Vector(float* xyzw) noexcept;
		Vector(const __m128& xyzw) noexcept;
		Vector(const Vector& vec) noexcept;
		Vector(Vec3<> vec, bool Point = true) noexcept;

		Tuple::Vector operator-() const noexcept;
		__m128 operator-(const Vector& b) const noexcept;
		__m128 operator+(const Vector& b) const noexcept;
		__m128 operator*(const Vector& b) const noexcept;
		__m128 operator*(float b) const noexcept;
		__m128 operator/(const Vector& b) const noexcept;
		__m128 operator/(float b) const noexcept;
		Tuple::Vector& operator+=(const Tuple::Vector& b) noexcept;
		Tuple::Vector& operator-=(const Tuple::Vector& b) noexcept;
		Tuple::Vector& operator*=(const Tuple::Vector& b) noexcept;
		Tuple::Vector& operator*=(float b) noexcept;
		Tuple::Vector& operator/=(float b) noexcept;
		Tuple::Vector& operator/=(const Tuple::Vector& b) noexcept;

		operator float* () { return xyzw; }
		operator Vec3<float>() const { return Vec3<float>{x, y, z}; }
		friend std::ostream& operator<<(std::ostream& os, const Vector& pos)
		{
			os << "Vector (" << pos.x << ", " << pos.y << ", " << pos.z << ", " << pos.w << ")";
			return os;
		}
		inline operator __m128() const noexcept { return v_XYZW; }
	};

	float __vectorcall DotProduct(const Vector& a, const Vector& b) noexcept;
	float __vectorcall Magnitude(const Tuple::Vector& vector) noexcept;
	Vector __vectorcall Normalize(const Tuple::Vector& vector) noexcept;
	Vector __vectorcall NormalizeXYZ(const Tuple::Vector& vector) noexcept;
	Vector __vectorcall ScaleTuple(const Tuple::Vector& a, float multiplier) noexcept;
	Vector __vectorcall CrossProduct(const Vector& a, const Vector& b) noexcept;
	Vector __vectorcall Reflect(const Vector& in, const Vector& normal) noexcept;
}

// ------------------- //
// - Tuple namespace - //
// ------------------- //

// Constructors
inline Tuple::Vector::Vector(float x, float y, float z, float w) noexcept
{
	w = std::fabsf(w);
	w = (w > 1) ? 1 : w;
	v_XYZW = _mm_set_ps(w, z, y, x);
}
inline Tuple::Vector::Vector(float* xyzw) noexcept
{
	xyzw[3] = std::fabsf(xyzw[3]);
	xyzw[3] = (xyzw[3] > 1) ? 1 : xyzw[3];
	v_XYZW = _mm_loadu_ps(xyzw);
}
inline Tuple::Vector::Vector(const __m128& xyzw) noexcept
{
	v_XYZW = xyzw;
	v_XYZW.m128_f32[3] = std::fabsf(xyzw.m128_f32[3]);
	v_XYZW.m128_f32[3] = (v_XYZW.m128_f32[3] > 1) ? 1 : v_XYZW.m128_f32[3];
}
inline Tuple::Vector::Vector(const Vector& vec) noexcept
{
	v_XYZW = vec.v_XYZW;
}
inline Tuple::Vector::Vector(Vec3<> vec, bool point) noexcept
{
	v_XYZW = _mm_set_ps((float)point, vec.z, vec.y, vec.x);
}

// Overloads
inline Tuple::Vector Tuple::Vector::operator-() const noexcept
{
	return Tuple::Vector{ -x, -y, -z, w };
}
inline __m128 Tuple::Vector::operator-(const Vector& b) const noexcept
{
	return _mm_sub_ps(v_XYZW, b.v_XYZW);
}
inline __m128 Tuple::Vector::operator+(const Tuple::Vector& b) const noexcept
{
	return _mm_add_ps(v_XYZW, b.v_XYZW);
}
inline __m128 Tuple::Vector::operator*(const Tuple::Vector& b) const noexcept
{
	return _mm_mul_ps(v_XYZW, b.v_XYZW);
}
inline __m128 Tuple::Vector::operator*(float b) const noexcept
{
	return _mm_mul_ps(v_XYZW, _mm_set_ps1(b));
}
inline __m128 Tuple::Vector::operator/(const Vector& b) const noexcept
{
	return _mm_div_ps(v_XYZW, b.v_XYZW);
}
inline __m128 Tuple::Vector::operator/(float b) const noexcept
{
	return _mm_mul_ps(v_XYZW, _mm_set_ps1(b));
}

// Assign Overloads
inline Tuple::Vector& Tuple::Vector::operator+=(const Tuple::Vector& b) noexcept
{
	v_XYZW = _mm_add_ps(v_XYZW, b.v_XYZW);
	return *this;
}
inline Tuple::Vector& Tuple::Vector::operator-=(const Tuple::Vector& b) noexcept
{
	v_XYZW = _mm_sub_ps(v_XYZW, b.v_XYZW);
	return *this;
}
inline Tuple::Vector& Tuple::Vector::operator*=(const Tuple::Vector& b) noexcept
{
	v_XYZW = _mm_mul_ps(v_XYZW, b.v_XYZW);
	return *this;
}
inline Tuple::Vector& Tuple::Vector::operator*=(float b) noexcept
{
	v_XYZW = _mm_mul_ps(v_XYZW, _mm_set_ps1(b));
	return *this;
}
inline Tuple::Vector& Tuple::Vector::operator/=(const Tuple::Vector& b) noexcept
{
	v_XYZW = _mm_div_ps(v_XYZW, b.v_XYZW);
	return *this;
}
inline Tuple::Vector& Tuple::Vector::operator/=(float b) noexcept
{
	v_XYZW = _mm_mul_ps(v_XYZW, _mm_set_ps1(1.0f / b));
	return *this;
}

// Utility functions
inline float		 __vectorcall Tuple::DotProduct(const Vector& a, const Vector& b) noexcept
{
	return _mm_dp_ps(a.v_XYZW, b.v_XYZW, 0x71).m128_f32[0];
}
inline float		 __vectorcall Tuple::Magnitude(const Tuple::Vector& vector) noexcept
{
	return _mm_sqrt_ps(_mm_dp_ps(vector.v_XYZW, vector.v_XYZW, 0x71)).m128_f32[0];
}
inline Tuple::Vector __vectorcall Tuple::Normalize(const Tuple::Vector& vector) noexcept
{
	// here _mm_setr_ps() is gaining 10% speed over vector.v_XYZW, probably cache-related || indirection cost
	return Tuple::Vector(_mm_mul_ps(_mm_setr_ps(vector.x, vector.y, vector.z, NULL), _mm_set_ps1(1.0f / Magnitude(vector))));
}
inline Tuple::Vector __vectorcall Tuple::NormalizeXYZ(const Tuple::Vector& vector) noexcept
{
	using namespace DirectX;

	const auto temp = XMFLOAT3{ vector.x, vector.y, vector.z };
	return XMVector3Normalize(XMLoadFloat3(&temp));
}
inline Tuple::Vector __vectorcall Tuple::ScaleTuple(const Tuple::Vector& a, float multiplier) noexcept
{
	return Tuple::Vector{ _mm_mul_ps(a.v_XYZW, _mm_set_ps1(multiplier)) };
}
inline Tuple::Vector __vectorcall Tuple::CrossProduct(const Tuple::Vector& a, const Tuple::Vector& b) noexcept
{
	// credits to https://fastcpp.blogspot.com/2011/04/vector-cross-product-using-sse-code.html Anonymous commentary

	const __m128 a1 = _mm_setr_ps(a.x, a.y, a.z, NULL);
	const __m128 b1 = _mm_setr_ps(b.x, b.y, b.z, NULL);
	const __m128 result = _mm_sub_ps(_mm_mul_ps(b1, _mm_shuffle_ps(a1, a1, _MM_SHUFFLE(3, 0, 2, 1))), _mm_mul_ps(a1, _mm_shuffle_ps(b1, b1, _MM_SHUFFLE(3, 0, 2, 1))));

	return -Tuple::Vector(_mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 0, 2, 1)));
}
inline Tuple::Vector __vectorcall Tuple::Reflect(const Tuple::Vector& in, const Tuple::Vector& normal) noexcept
{
	return Tuple::Vector{ _mm_fnmadd_ps(_mm_set_ps1(2.0f * Tuple::DotProduct(in, normal)), normal.v_XYZW, in.v_XYZW) };
}

// --------------- //
// - Color Class - //
// --------------- //

struct Color
{
	// alignas(16)
	union
	{
		__m128 RGBA;
		float rgba[4];
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
	};

	explicit Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f) noexcept;
	explicit Color(__m128 intrinsics) noexcept;
	explicit Color(float rgba[4]) noexcept;
	uint32_t PackedColor() const noexcept;

	bool operator==(const Color& other) const noexcept;
	Color __vectorcall operator+(const Color& other) const noexcept;
	Color __vectorcall operator-(const Color& other) const noexcept;
	Color __vectorcall operator*(const Color& other) const noexcept;
	Color __vectorcall operator*(float other) const noexcept;
	Color __vectorcall operator/(float other) const noexcept;
	operator float* () { return rgba; }

	friend std::ostream& operator<<(std::ostream& os, const Color& cor)
	{
		os << "RGBA (" << cor.r << ", " << cor.g << ", " << cor.b << ", " << cor.a << ")";
		return os;
	}
};

// Color inline's
inline Color::Color(float red, float green, float blue, float alpha) noexcept
{
	// checking/clamping
	red > 1.0f ? red = 1.0f : red < 0.0f ? red = 0.0f : NULL;
	green > 1.0f ? green = 1.0f : green < 0.0f ? green = 0.0f : NULL;
	blue > 1.0f ? blue = 1.0f : blue < 0.0f ? blue = 0.0f : NULL;
	alpha > 1.0f ? alpha = 1.0f : alpha < 0.0f ? alpha = 0.0f : NULL;

	RGBA = _mm_set_ps(alpha, blue, green, red);
}
inline Color::Color(__m128 intrinsics) noexcept
{
	// checking/clamping
	intrinsics.m128_f32[0] > 1.0f ? intrinsics.m128_f32[0] = 1.0f : intrinsics.m128_f32[0] < 0.0f ? intrinsics.m128_f32[0] = 0.0f : NULL;
	intrinsics.m128_f32[1] > 1.0f ? intrinsics.m128_f32[1] = 1.0f : intrinsics.m128_f32[1] < 0.0f ? intrinsics.m128_f32[1] = 0.0f : NULL;
	intrinsics.m128_f32[2] > 1.0f ? intrinsics.m128_f32[2] = 1.0f : intrinsics.m128_f32[2] < 0.0f ? intrinsics.m128_f32[2] = 0.0f : NULL;
	intrinsics.m128_f32[3] > 1.0f ? intrinsics.m128_f32[3] = 1.0f : intrinsics.m128_f32[3] < 0.0f ? intrinsics.m128_f32[3] = 0.0f : NULL;

	RGBA = intrinsics;
}
inline Color::Color(float rgba[4]) noexcept
{
	// checking/clamping
	rgba[0] > 1.0f ? rgba[0] = 1.0f : rgba[0] < 0.0f ? rgba[0] = 0.0f : NULL;
	rgba[1] > 1.0f ? rgba[1] = 1.0f : rgba[1] < 0.0f ? rgba[1] = 0.0f : NULL;
	rgba[2] > 1.0f ? rgba[2] = 1.0f : rgba[2] < 0.0f ? rgba[2] = 0.0f : NULL;
	rgba[3] > 1.0f ? rgba[3] = 1.0f : rgba[3] < 0.0f ? rgba[3] = 0.0f : NULL;

	RGBA = _mm_loadu_ps(rgba);
}
inline uint32_t Color::PackedColor() const noexcept
{
	struct PColor
	{
		union
		{
			struct
			{
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a;
			};
			uint32_t cRGBA;
		};
	} const temp{ static_cast<uint8_t>(r * 255.0f), static_cast<uint8_t>(g * 255.0f), static_cast<uint8_t>(b * 255.0f), static_cast<uint8_t>(a * 255.0f) };

	return temp.cRGBA;
}

// Operator overloads
inline bool Color::operator==(const Color& other) const noexcept
{
	if (!memcmp(rgba, other.rgba, 4 * sizeof(float)))
		return true;
	else
		return false;
}
inline Color __vectorcall Color::operator+(const Color& other) const noexcept
{
	return Color{ _mm_add_ps(RGBA, other.RGBA) };
}
inline Color __vectorcall Color::operator-(const Color& other) const noexcept
{
	return Color{ _mm_sub_ps(RGBA, other.RGBA) };
}
inline Color __vectorcall Color::operator*(const Color& other) const noexcept
{
	return Color{ _mm_mul_ps(RGBA, other.RGBA) };
}
inline Color __vectorcall Color::operator*(float other) const noexcept
{
	return Color{ _mm_mul_ps(RGBA, _mm_set_ps1(other)) };
}
inline Color __vectorcall Color::operator/(float other) const noexcept
{
	return Color{ _mm_div_ps(RGBA, _mm_set_ps1(other)) };
}


//~~~~~~~~~~~~~~~~~ Colors ~~~~~~~~~~~~~~~~//
// Primary
#define RED		Color{1.0f, 0.0f, 0.0f, 1.0f}
#define GREEN	Color{0.0f, 1.0f, 0.0f, 1.0f}
#define BLUE	Color{0.0f, 0.0f, 1.0f, 1.0f}
#define WHITE	Color{1.0f, 1.0f, 1.0f, 1.0f}
#define BLACK	Color{0.0f, 0.0f, 0.0f, 1.0f}

// Secondary
#define MAGENTA	Color{1.0f, 0.0f, 1.0f, 1.0f}
#define YELLOW	Color{1.0f, 1.0f, 0.0f, 1.0f}
#define CYAN	Color{0.0f, 1.0f, 1.0f, 1.0f}
#define GRAY	Color{0.5f, 0.5f, 0.5f, 1.0f}
#define BROWN	Color{0.5882f, 0.2941f, 0.0f, 1.0f}

// Tertiary
#define ORANGE	Color{1.0f, 0.647f, 0.0f, 1.0f}
#define VIOLET	Color{0.933f, 0.51f, 0.933f, 1.0f}
#define LMGREEN Color{0.196f, 0.804f, 0.196f, 1.0f}
#define PRPBLUE Color{0.541f, 0.169f, 0.882f, 1.0f}
#define TOMATO	Color{1.0f, 0.388f, 0.278f, 1.0f}

// Specifics
#define RYLBLUE Color{0.0f, 0.0f, 0.9822f, 1.0f}
#define PINK	Color{1.0f, 0.7961f, 0.8588f, 1.0f}
#define TURQSE	Color{0.251f, 0.8784f, 0.8157f, 1.0f}
#define GOLD	Color{1.0f, 0.8431f, 0.0f, 1.0f}
#define SILVER	Color{0.7529f, 0.7529f, 0.7529f, 1.0f}


// ---------------------- //
// ---- Matrix Class ---- //
// ---------------------- //

#define DO_NOT_MULTIPLY_FLAG 0xDeadBeef // unused atm
class Matrix4x4f
{
public:

	// constructors
	Matrix4x4f()
	{
		setConstantAsIdentity(1.0f);
	}
	Matrix4x4f(float mat[16]) noexcept
	{
		Matrix.lower = _mm256_loadu_ps(mat);
		Matrix.upper = _mm256_loadu_ps(mat + 8);
	}
	Matrix4x4f(const std::array<float, 16>& mat)
	{
		Matrix.lower = _mm256_loadu_ps(mat.data());
		Matrix.upper = _mm256_loadu_ps(mat.data() + 8);
	}
	Matrix4x4f(float a00, float a01, float a02, float a03, float a10, float a11, float a12, float a13, float a20, float a21, float a22, float a23, float a30, float a31, float a32, float a33)
	{
		const float mat[16] = { a00, a01, a02, a03, a10, a11, a12, a13, a20, a21, a22, a23, a30, a31, a32, a33 };
		Matrix.lower = _mm256_loadu_ps(mat);
		Matrix.upper = _mm256_loadu_ps(mat + 8);
	}
	Matrix4x4f(const Matrix4x4f& other)
	{
		Matrix.lower = other.Matrix.lower;
		Matrix.upper = other.Matrix.upper;
	}
	Matrix4x4f(const __m256& a, const __m256& b)
	{
		Matrix.lower = a;
		Matrix.upper = b;
	}
	Matrix4x4f(const __m128& Row0, const __m128& Row1, const __m128& Row2, const __m128& Row3)
	{
		Matrix.row0 = Row0;
		Matrix.row1 = Row1;
		Matrix.row2 = Row2;
		Matrix.row3 = Row3;
	}
	Matrix4x4f(const DirectX::XMMATRIX& other)
	{
		Matrix.row0 = other.r[0];
		Matrix.row1 = other.r[1];
		Matrix.row2 = other.r[2];
		Matrix.row3 = other.r[3];
	}

	// class utilities
	inline void setConstant(float constant) noexcept
	{
		//std::memcpy(SIMD_matrix, &constant, 16 * sizeof(float));
		Matrix.lower = _mm256_set1_ps(constant);
		Matrix.upper = _mm256_set1_ps(constant);
	}
	inline void setConstantAsIdentity(float constant) noexcept
	{
		//std::memset(SIMD_matrix, 0, 16 * sizeof(float));
		setConstant(0.0f);
		Matrix.elements[0] = constant;
		Matrix.elements[5] = constant;
		Matrix.elements[10] = constant;
		Matrix.elements[15] = constant;
	}
	inline bool isEqual(const Matrix4x4f& b) const noexcept
	{
		for (size_t i = 0; i < 16; i++)
		{
			if (fabsf(Matrix.elements[i] - b.Matrix.elements[i]) > EPSILON)
			{
				return false;
			}
		}
		return true;
	}
	inline const Matrix4x4f __vectorcall Transposed() const noexcept
	{
		__m128 row0{ Matrix.row0 }, row1{ Matrix.row1 }, row2{ Matrix.row2 }, row3{ Matrix.row3 };

		// transpose
		_MM_TRANSPOSE4_PS(row0, row1, row2, row3);

		// load
		const __m256 temp1 = _mm256_insertf128_ps(_mm256_castps128_ps256(row0), row1, 0x1);
		const __m256 temp2 = _mm256_insertf128_ps(_mm256_castps128_ps256(row2), row3, 0x1);

		return { temp1, temp2 };
	}
	inline const Matrix4x4f __vectorcall Invert() const noexcept
	{
		// Transpose elements
		const Matrix4x4f TMat = this->Transposed();

		__m128 V00 = _mm_shuffle_ps(TMat.Matrix.row2, TMat.Matrix.row2, _MM_SHUFFLE(1, 1, 0, 0));
		__m128 V10 = _mm_shuffle_ps(TMat.Matrix.row3, TMat.Matrix.row3, _MM_SHUFFLE(3, 2, 3, 2));
		__m128 V01 = _mm_shuffle_ps(TMat.Matrix.row0, TMat.Matrix.row0, _MM_SHUFFLE(1, 1, 0, 0));
		__m128 V11 = _mm_shuffle_ps(TMat.Matrix.row1, TMat.Matrix.row1, _MM_SHUFFLE(3, 2, 3, 2));
		__m128 V02 = _mm_shuffle_ps(TMat.Matrix.row2, TMat.Matrix.row0, _MM_SHUFFLE(2, 0, 2, 0));
		__m128 V12 = _mm_shuffle_ps(TMat.Matrix.row3, TMat.Matrix.row1, _MM_SHUFFLE(3, 1, 3, 1));

		__m128 D0 = _mm_mul_ps(V00, V10);
		__m128 D1 = _mm_mul_ps(V01, V11);
		__m128 D2 = _mm_mul_ps(V02, V12);

		V00 = _mm_shuffle_ps(TMat.Matrix.row2, TMat.Matrix.row2, _MM_SHUFFLE(3, 2, 3, 2));
		V10 = _mm_shuffle_ps(TMat.Matrix.row3, TMat.Matrix.row3, _MM_SHUFFLE(1, 1, 0, 0));
		V01 = _mm_shuffle_ps(TMat.Matrix.row0, TMat.Matrix.row0, _MM_SHUFFLE(3, 2, 3, 2));
		V11 = _mm_shuffle_ps(TMat.Matrix.row1, TMat.Matrix.row1, _MM_SHUFFLE(1, 1, 0, 0));
		V02 = _mm_shuffle_ps(TMat.Matrix.row2, TMat.Matrix.row0, _MM_SHUFFLE(3, 1, 3, 1));
		V12 = _mm_shuffle_ps(TMat.Matrix.row3, TMat.Matrix.row1, _MM_SHUFFLE(2, 0, 2, 0));

		D0 = _mm_fnmadd_ps(V00, V10, D0);
		D1 = _mm_fnmadd_ps(V01, V11, D1);
		D2 = _mm_fnmadd_ps(V02, V12, D2);

		// V11 = D0Y,D0W,D2Y,D2Y
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
		V00 = _mm_shuffle_ps(TMat.Matrix.row1, TMat.Matrix.row1, _MM_SHUFFLE(1, 0, 2, 1));
		V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
		V01 = _mm_shuffle_ps(TMat.Matrix.row0, TMat.Matrix.row0, _MM_SHUFFLE(0, 1, 0, 2));
		V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));

		// V13 = D1Y,D1W,D2W,D2W
		__m128 V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
		V02 = _mm_shuffle_ps(TMat.Matrix.row3, TMat.Matrix.row3, _MM_SHUFFLE(1, 0, 2, 1));
		__m128 V03 = _mm_shuffle_ps(TMat.Matrix.row2, TMat.Matrix.row2, _MM_SHUFFLE(0, 1, 0, 2));
		V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
		V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

		__m128 C0 = _mm_mul_ps(V00, V10);
		__m128 C2 = _mm_mul_ps(V01, V11);
		__m128 C4 = _mm_mul_ps(V02, V12);
		__m128 C6 = _mm_mul_ps(V03, V13);

		// V11 = D0X,D0Y,D2X,D2X
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
		V00 = _mm_shuffle_ps(TMat.Matrix.row1, TMat.Matrix.row1, _MM_SHUFFLE(2, 1, 3, 2));
		V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
		V01 = _mm_shuffle_ps(TMat.Matrix.row0, TMat.Matrix.row0, _MM_SHUFFLE(1, 3, 2, 3));
		V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));

		// V13 = D1X,D1Y,D2Z,D2Z
		V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
		V02 = _mm_shuffle_ps(TMat.Matrix.row3, TMat.Matrix.row3, _MM_SHUFFLE(2, 1, 3, 2));
		V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
		V03 = _mm_shuffle_ps(TMat.Matrix.row2, TMat.Matrix.row2, _MM_SHUFFLE(1, 3, 2, 3));
		V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

		C0 = _mm_fnmadd_ps(V00, V10, C0);
		C2 = _mm_fnmadd_ps(V01, V11, C2);
		C4 = _mm_fnmadd_ps(V02, V12, C4);
		C6 = _mm_fnmadd_ps(V03, V13, C6);

		V00 = _mm_shuffle_ps(TMat.Matrix.row1, TMat.Matrix.row1, _MM_SHUFFLE(0, 3, 0, 3));
		// V10 = D0Z,D0Z,D2X,D2Y
		V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
		V10 = _mm_shuffle_ps(V10, V10, _MM_SHUFFLE(0, 2, 3, 0));
		V01 = _mm_shuffle_ps(TMat.Matrix.row0, TMat.Matrix.row0, _MM_SHUFFLE(2, 0, 3, 1));
		// V11 = D0X,D0W,D2X,D2Y
		V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
		V11 = _mm_shuffle_ps(V11, V11, _MM_SHUFFLE(2, 1, 0, 3));
		V02 = _mm_shuffle_ps(TMat.Matrix.row3, TMat.Matrix.row3, _MM_SHUFFLE(0, 3, 0, 3));
		// V12 = D1Z,D1Z,D2Z,D2W
		V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
		V12 = _mm_shuffle_ps(V12, V12, _MM_SHUFFLE(0, 2, 3, 0));
		V03 = _mm_shuffle_ps(TMat.Matrix.row2, TMat.Matrix.row2, _MM_SHUFFLE(2, 0, 3, 1));
		// V13 = D1X,D1W,D2Z,D2W
		V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
		V13 = _mm_shuffle_ps(V13, V13, _MM_SHUFFLE(2, 1, 0, 3));

		V00 = _mm_mul_ps(V00, V10);
		V01 = _mm_mul_ps(V01, V11);
		V02 = _mm_mul_ps(V02, V12);
		V03 = _mm_mul_ps(V03, V13);
		__m128 C1 = _mm_sub_ps(C0, V00);
		C0 = _mm_add_ps(C0, V00);
		__m128 C3 = _mm_add_ps(C2, V01);
		C2 = _mm_sub_ps(C2, V01);
		__m128 C5 = _mm_sub_ps(C4, V02);
		C4 = _mm_add_ps(C4, V02);
		__m128 C7 = _mm_add_ps(C6, V03);
		C6 = _mm_sub_ps(C6, V03);

		C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
		C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
		C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
		C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
		C0 = _mm_shuffle_ps(C0, C0, _MM_SHUFFLE(3, 1, 2, 0));
		C2 = _mm_shuffle_ps(C2, C2, _MM_SHUFFLE(3, 1, 2, 0));
		C4 = _mm_shuffle_ps(C4, C4, _MM_SHUFFLE(3, 1, 2, 0));
		C6 = _mm_shuffle_ps(C6, C6, _MM_SHUFFLE(3, 1, 2, 0));

		// Get the determinant
		static const __m128 one = _mm_set_ps1(1.0f);
		__m128 vTemp = _mm_dp_ps(C0, TMat.Matrix.row0, 0xff); // cofactors
		vTemp = _mm_div_ps(one, vTemp);

		//   row0,					row1				   row2				      row3
		return { _mm_mul_ps(C0, vTemp), _mm_mul_ps(C2, vTemp), _mm_mul_ps(C4, vTemp), _mm_mul_ps(C6, vTemp) };
	}

	// operator overloads
	Matrix4x4f& operator=(const Matrix4x4f& other)
	{
		// when crashing around here, it's because there's a shader accessing an out of bounds resource, such as sampling
		// or some buffer resource is being corrupted
		if (this != &other)
		{
			Matrix.lower = other.Matrix.lower;
			Matrix.upper = other.Matrix.upper;
		}

		return *this;
	}
	const float& operator[](int index) const noexcept
	{
		return Matrix.elements[index];
	}
	float& operator[](int index) noexcept
	{
		return Matrix.elements[index];
	}

	inline Matrix4x4f& __vectorcall operator+=(const Matrix4x4f& b) noexcept
	{
		Matrix.row0 = _mm_add_ps(Matrix.row0, b.Matrix.row0);
		Matrix.row1 = _mm_add_ps(Matrix.row1, b.Matrix.row1);
		Matrix.row2 = _mm_add_ps(Matrix.row2, b.Matrix.row2);
		Matrix.row3 = _mm_add_ps(Matrix.row3, b.Matrix.row3);

		return *this;
	}
	inline Matrix4x4f& __vectorcall operator-=(const Matrix4x4f& b) noexcept
	{
		Matrix.row0 = _mm_sub_ps(Matrix.row0, b.Matrix.row0);
		Matrix.row1 = _mm_sub_ps(Matrix.row1, b.Matrix.row1);
		Matrix.row2 = _mm_sub_ps(Matrix.row2, b.Matrix.row2);
		Matrix.row3 = _mm_sub_ps(Matrix.row3, b.Matrix.row3);

		return *this;
	}
	const Matrix4x4f __vectorcall operator*(const Matrix4x4f& b) const noexcept
	{
		// credits to DirectX::XMMatrixMultiply()
		__m256 a0 = _mm256_shuffle_ps(Matrix.lower, Matrix.lower, _MM_SHUFFLE(0, 0, 0, 0)); // elements[0]
		__m256 a1 = _mm256_shuffle_ps(Matrix.upper, Matrix.upper, _MM_SHUFFLE(0, 0, 0, 0)); // elements[8]
		__m256 b0 = _mm256_permute2f128_ps(b.Matrix.lower, b.Matrix.lower, 0x00);
		const __m256 c0 = _mm256_mul_ps(a0, b0);
		const __m256 c1 = _mm256_mul_ps(a1, b0);

		a0 = _mm256_shuffle_ps(Matrix.lower, Matrix.lower, _MM_SHUFFLE(1, 1, 1, 1)); // elements[1]
		a1 = _mm256_shuffle_ps(Matrix.upper, Matrix.upper, _MM_SHUFFLE(1, 1, 1, 1)); // elements[10]
		b0 = _mm256_permute2f128_ps(b.Matrix.lower, b.Matrix.lower, 0x11);
		const __m256 c2 = _mm256_fmadd_ps(a0, b0, c0);
		const __m256 c3 = _mm256_fmadd_ps(a1, b0, c1);

		a0 = _mm256_shuffle_ps(Matrix.lower, Matrix.lower, _MM_SHUFFLE(2, 2, 2, 2)); // elements[2]
		a1 = _mm256_shuffle_ps(Matrix.upper, Matrix.upper, _MM_SHUFFLE(2, 2, 2, 2)); // elements[11]
		__m256 b1 = _mm256_permute2f128_ps(b.Matrix.upper, b.Matrix.upper, 0x00);
		const __m256 c4 = _mm256_mul_ps(a0, b1);
		const __m256 c5 = _mm256_mul_ps(a1, b1);

		a0 = _mm256_shuffle_ps(Matrix.lower, Matrix.lower, _MM_SHUFFLE(3, 3, 3, 3)); // elements[3]
		a1 = _mm256_shuffle_ps(Matrix.upper, Matrix.upper, _MM_SHUFFLE(3, 3, 3, 3)); // elements[12]
		b1 = _mm256_permute2f128_ps(b.Matrix.upper, b.Matrix.upper, 0x11);
		const __m256 c6 = _mm256_fmadd_ps(a0, b1, c4);
		const __m256 c7 = _mm256_fmadd_ps(a1, b1, c5);

		const __m256 result1 = _mm256_add_ps(c2, c6);
		const __m256 result2 = _mm256_add_ps(c3, c7);

		return { result1, result2 };
	}
	const Matrix4x4f& __vectorcall operator*=(const Matrix4x4f& b) noexcept
	{
		// credits to DirectX::XMMatrixMultiply()

		__m256 a0 = _mm256_shuffle_ps(Matrix.lower, Matrix.lower, _MM_SHUFFLE(0, 0, 0, 0)); // elements[0]
		__m256 a1 = _mm256_shuffle_ps(Matrix.upper, Matrix.upper, _MM_SHUFFLE(0, 0, 0, 0)); // elements[8]
		__m256 b0 = _mm256_permute2f128_ps(b.Matrix.lower, b.Matrix.lower, 0x00);
		const __m256 c0 = _mm256_mul_ps(a0, b0);
		const __m256 c1 = _mm256_mul_ps(a1, b0);

		a0 = _mm256_shuffle_ps(Matrix.lower, Matrix.lower, _MM_SHUFFLE(1, 1, 1, 1)); // elements[1]
		a1 = _mm256_shuffle_ps(Matrix.upper, Matrix.upper, _MM_SHUFFLE(1, 1, 1, 1)); // elements[10]
		b0 = _mm256_permute2f128_ps(b.Matrix.lower, b.Matrix.lower, 0x11);
		const __m256 c2 = _mm256_fmadd_ps(a0, b0, c0);
		const __m256 c3 = _mm256_fmadd_ps(a1, b0, c1);

		a0 = _mm256_shuffle_ps(Matrix.lower, Matrix.lower, _MM_SHUFFLE(2, 2, 2, 2)); // elements[2]
		a1 = _mm256_shuffle_ps(Matrix.upper, Matrix.upper, _MM_SHUFFLE(2, 2, 2, 2)); // elements[11]
		__m256 b1 = _mm256_permute2f128_ps(b.Matrix.upper, b.Matrix.upper, 0x00);
		const __m256 c4 = _mm256_mul_ps(a0, b1);
		const __m256 c5 = _mm256_mul_ps(a1, b1);

		a0 = _mm256_shuffle_ps(Matrix.lower, Matrix.lower, _MM_SHUFFLE(3, 3, 3, 3)); // elements[3]
		a1 = _mm256_shuffle_ps(Matrix.upper, Matrix.upper, _MM_SHUFFLE(3, 3, 3, 3)); // elements[12]
		b1 = _mm256_permute2f128_ps(b.Matrix.upper, b.Matrix.upper, 0x11);
		const __m256 c6 = _mm256_fmadd_ps(a0, b1, c4);
		const __m256 c7 = _mm256_fmadd_ps(a1, b1, c5);

		Matrix.upper = _mm256_add_ps(c2, c6);
		Matrix.lower = _mm256_add_ps(c3, c7);

		return *this;
	}
	const Tuple::Vector __vectorcall operator*(const Tuple::Vector& b) const noexcept
	{
		__m128 result = _mm_shuffle_ps(b, b, _MM_SHUFFLE(2, 2, 2, 2)); // Z
		result = _mm_fmadd_ps(result, Matrix.row2, Matrix.row3);
		__m128 temp = _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 1, 1)); // Y
		result = _mm_fmadd_ps(temp, Matrix.row1, result);
		temp = _mm_shuffle_ps(b, b, _MM_SHUFFLE(0, 0, 0, 0)); // X
		result = _mm_fmadd_ps(temp, Matrix.row0, result);
		return result;
	}

	// friend utilities
	friend std::ostream& operator<<(std::ostream& os, const Matrix4x4f& a) noexcept
	{
		for (int i = 0; i < 16; i++)
		{
			os << a.Matrix.elements[i] << " ";
			if (i == 3 || i == 7 || i == 11 || i == 15)
				os << std::endl;
		}
		return os;
	}
	friend float __vectorcall Determinant(const Matrix4x4f& matrix) noexcept;
	friend Matrix4x4f ViewTransform(const Tuple::Vector& from, const Tuple::Vector& to, const Tuple::Vector& up) noexcept;
	friend Tuple::Vector CoordTransform(const Matrix4x4f& mat, const Tuple::Vector& vector);
	friend Matrix4x4f PerspectiveMatrixFOV(float FovAngleY, float Aspect, float NearZ, float FarZ) noexcept;

public:
	union
	{
		struct // AVX
		{
			__m256 lower;
			__m256 upper;
		};
		struct // SSE
		{
			__m128 row0;
			__m128 row1;
			__m128 row2;
			__m128 row3;
		};
		float elements[16];
	} Matrix;
};

// Utilities for the Matrix4x4f class

__forceinline const Matrix4x4f IdentityMat4x4f() noexcept
{
	return
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}
__forceinline const Matrix4x4f Inversed_Transposed__LIGHTINGCALC(const Matrix4x4f& matrix)
{
	Matrix4x4f temp = matrix.Transposed();
	temp.Matrix.row3 = DirectX::XMVectorSet(0, 0, 0, 1);

	return temp.Invert();
}
__forceinline float __vectorcall Determinant(const Matrix4x4f& matrix) noexcept
{
	// credits to DirectX::XMMatrixDeterminant;

	static constexpr __m128 Sign = { 1.0f, -1.0f, 1.0f, -1.0f }; // + , - , + , -

	__m128 v0 = _mm_shuffle_ps(matrix.Matrix.row2, matrix.Matrix.row2, _MM_SHUFFLE(0, 0, 0, 1));
	__m128 v1 = _mm_shuffle_ps(matrix.Matrix.row3, matrix.Matrix.row3, _MM_SHUFFLE(1, 1, 2, 2));
	__m128 v2 = _mm_shuffle_ps(matrix.Matrix.row2, matrix.Matrix.row2, _MM_SHUFFLE(0, 0, 0, 1));
	__m128 v3 = _mm_shuffle_ps(matrix.Matrix.row3, matrix.Matrix.row3, _MM_SHUFFLE(2, 3, 3, 3));
	__m128 v4 = _mm_shuffle_ps(matrix.Matrix.row2, matrix.Matrix.row2, _MM_SHUFFLE(1, 1, 2, 2));
	__m128 v5 = _mm_shuffle_ps(matrix.Matrix.row3, matrix.Matrix.row3, _MM_SHUFFLE(2, 3, 3, 3));

	__m128 P0 = _mm_mul_ps(v0, v1);
	__m128 P1 = _mm_mul_ps(v2, v3);
	__m128 P2 = _mm_mul_ps(v4, v5);

	v0 = _mm_shuffle_ps(matrix.Matrix.row2, matrix.Matrix.row2, _MM_SHUFFLE(1, 1, 2, 2));
	v1 = _mm_shuffle_ps(matrix.Matrix.row3, matrix.Matrix.row3, _MM_SHUFFLE(0, 0, 0, 1));
	v2 = _mm_shuffle_ps(matrix.Matrix.row2, matrix.Matrix.row2, _MM_SHUFFLE(2, 3, 3, 3));
	v3 = _mm_shuffle_ps(matrix.Matrix.row3, matrix.Matrix.row3, _MM_SHUFFLE(0, 0, 0, 1));
	v4 = _mm_shuffle_ps(matrix.Matrix.row2, matrix.Matrix.row2, _MM_SHUFFLE(2, 3, 3, 3));
	v5 = _mm_shuffle_ps(matrix.Matrix.row3, matrix.Matrix.row3, _MM_SHUFFLE(1, 1, 2, 2));

	P0 = _mm_fnmadd_ps(v0, v1, P0);
	P1 = _mm_fnmadd_ps(v2, v3, P1);
	P2 = _mm_fnmadd_ps(v4, v5, P2);

	v0 = _mm_shuffle_ps(matrix.Matrix.row1, matrix.Matrix.row1, _MM_SHUFFLE(2, 3, 3, 3));
	v1 = _mm_shuffle_ps(matrix.Matrix.row1, matrix.Matrix.row1, _MM_SHUFFLE(1, 1, 2, 2));
	v2 = _mm_shuffle_ps(matrix.Matrix.row1, matrix.Matrix.row1, _MM_SHUFFLE(0, 0, 0, 1));

	const __m128 S = _mm_mul_ps(matrix.Matrix.row0, Sign);
	__m128 R = _mm_mul_ps(v0, P0);
	R = _mm_fnmadd_ps(v1, P1, R);
	R = _mm_fmadd_ps(v2, P2, R);

	return _mm_dp_ps(S, R, 0xff).m128_f32[0];
}
static constexpr float toRadians(float degrees) noexcept
{
	return degrees * 0.0174533f;
}
static constexpr float toDegrees(float radians) noexcept
{
	return radians * 57.295776f;
}

// Matrix Transforms
__forceinline Matrix4x4f Translate(const Tuple::Vector& point) noexcept
{
	return
	{
		1.0f, 0.0f, 0.0f, point.x,
		0.0f, 1.0f, 0.0f, point.y,
		0.0f, 0.0f, 1.0f, point.z,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
__forceinline Matrix4x4f Translate(float x, float y, float z) noexcept
{
	return
	{
		1.0f, 0.0f, 0.0f, x,
		0.0f, 1.0f, 0.0f, y,
		0.0f, 0.0f, 1.0f, z,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
__forceinline Matrix4x4f Translate(const Vec3<>& xyz)
{
	return
	{
		1.0f, 0.0f, 0.0f, xyz.x,
		0.0f, 1.0f, 0.0f, xyz.y,
		0.0f, 0.0f, 1.0f, xyz.z,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
__forceinline Matrix4x4f Scale(float x, float y, float z) noexcept
{
	return
	{
		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
__forceinline Matrix4x4f Scale(const Tuple::Vector& vector) noexcept
{
	return
	{
		vector.x, 0.0f, 0.0f, 0.0f,
		0.0f, vector.y, 0.0f, 0.0f,
		0.0f, 0.0f, vector.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
__forceinline Matrix4x4f Scale(const Vec3<float>& xyz)
{
	return
	{
		xyz.x, 0.0f, 0.0f, 0.0f,
		0.0f, xyz.y, 0.0f, 0.0f,
		0.0f, 0.0f, xyz.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
__forceinline Matrix4x4f RotateX(float angle) noexcept
{
	angle = toRadians(angle);
	const float cosa = cos(angle);
	const float sina = sin(angle);

	return
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosa, -sina, 0.0f,
		0.0f, sina,  cosa, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
__forceinline Matrix4x4f RotateY(float angle) noexcept
{
	angle = toRadians(angle);
	const float cosa = cosf(angle);
	const float sina = sinf(angle);

	return
	{
		cosa, 0.0f, sina, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sina,0.0f, cosa, 0.0f,
		0.0f,0.0f,	0.0f, 1.0f
	};
}
__forceinline Matrix4x4f RotateZ(float angle) noexcept
{
	angle = toRadians(angle);
	const float cosa = cos(angle);
	const float sina = sin(angle);

	return
	{
		cosa, -sina, 0.0f, 0.0f,
		sina,  cosa, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
__forceinline Matrix4x4f RotateX_RAD(float radians) noexcept
{
	const float cosa = cosf(radians);
	const float sina = sinf(radians);

	return
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosa, -sina, 0.0f,
		0.0f, sina,  cosa, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
__forceinline Matrix4x4f RotateY_RAD(float radians) noexcept
{
	const float cosa = cosf(radians);
	const float sina = sinf(radians);

	return
	{
		cosa, 0.0f, sina, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sina,0.0f, cosa, 0.0f,
		0.0f,0.0f,	0.0f, 1.0f
	};
}
__forceinline Matrix4x4f RotateZ_RAD(float radians) noexcept
{
	const float cosa = cosf(radians);
	const float sina = sinf(radians);

	return
	{
		cosa, -sina, 0.0f, 0.0f,
		sina,  cosa, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
__forceinline Matrix4x4f RotateXYZ_Euler(const Vec3<float>& angles) noexcept
{
	const float x = toRadians(angles.x);
	const float y = toRadians(angles.y);
	const float z = toRadians(angles.z);

	const float sinx = sinf(x);
	const float siny = sinf(y);
	const float sinz = sinf(z);
	const float cosx = cosf(x);
	const float cosy = cosf(y);
	const float cosz = cosf(z);

	return
	{
		cosy * cosz, cosz * sinx * siny - cosx * sinz, sinx * sinz + cosx * cosz * siny, 0.0f,
		cosy * sinz, cosx * cosz + sinx * siny * sinz, cosx * siny * sinz - cosz * sinx, 0.0f,
		-siny, cosy * sinx, cosx * cosy, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
__forceinline Matrix4x4f RotateXYZ_Euler(float x, float y, float z) noexcept
{
	x = toRadians(x);
	y = toRadians(y);
	z = toRadians(z);

	const float sinx = sin(x);
	const float siny = sin(y);
	const float sinz = sin(z);
	const float cosx = cos(x);
	const float cosy = cos(y);
	const float cosz = cos(z);

	return
	{
		cosy * cosz, cosz * sinx * siny - cosx * sinz, sinx * sinz + cosx * cosz * siny, 0.0f,
		cosy * sinz, cosx * cosz + sinx * siny * sinz, cosx * siny * sinz - cosz * sinx, 0.0f,
		-siny, cosy * sinx, cosx * cosy, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
__forceinline Matrix4x4f RotateXYZ_Quartenions(const Vec3<float>& degrees)
{
	// Mere abstraction on top of DirectX Quartenions
	using namespace DirectX;

	return DirectX::XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(toRadians(degrees.x), toRadians(degrees.y), toRadians(degrees.z)));
}

// Use this one if the angles are already in radians
__forceinline Matrix4x4f RotateXYZ_QuartenionsRAD(const Vec3<float>& radians)
{
	// Mere abstraction on top of DirectX Quartenions
	using namespace DirectX;

	return DirectX::XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(radians.x, radians.y, radians.z));
}
__forceinline Matrix4x4f Shear(float xy, float xz, float yx, float yz, float zx, float zy) noexcept
{
	return
	{
			1.0f, yx, zx, 0.0f,
			xy, 1.0f, zy, 0.0,
			xz, yz, 1.0f, 0.0,
			 0,  0,  0,   1.0f
	};
}
__forceinline Matrix4x4f ViewTransform(const Tuple::Vector& from, const Tuple::Vector& to, const Tuple::Vector& up) noexcept
{
	return DirectX::XMMatrixLookAtLH(from, to, up);
}
__forceinline Tuple::Vector CoordTransform(const Matrix4x4f& mat, const Tuple::Vector& vector)
{
	// CTRL+C/CTRL+V from DirectX::XMVector3TransformCoord

	const __m128 Z = _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(2, 2, 2, 2));
	const __m128 Y = _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(1, 1, 1, 1));
	const __m128 X = _mm_shuffle_ps(vector, vector, _MM_SHUFFLE(0, 0, 0, 0));

	__m128 Result = _mm_fmadd_ps(Z, mat.Matrix.row2, mat.Matrix.row3);
	Result = _mm_fmadd_ps(Y, mat.Matrix.row1, Result);
	Result = _mm_fmadd_ps(X, mat.Matrix.row0, Result);

	const __m128 W = _mm_shuffle_ps(Result, Result, _MM_SHUFFLE(3, 3, 3, 3));
	return _mm_div_ps(Result, W);
}

// FovAngleY is in degrees
__forceinline Matrix4x4f PerspectiveMatrixFOV(float FovAngleY, float Aspect, float NearZ, float FarZ) noexcept
{
	using namespace DirectX;

	// DirectX::XMMatrixPerspectiveFovLH here

	float SinFov;
	float CosFov;

	FovAngleY = toRadians(FovAngleY);
	DirectX::XMScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

	float fRange = FarZ / (FarZ - NearZ);
	float Height = CosFov / SinFov;
	__m128 rMem = { Height / Aspect, Height, fRange, -fRange * NearZ };
	// Copy from memory to SSE register
	__m128 vValues = rMem;
	__m128 vTemp = _mm_setzero_ps();
	// Copy x only
	vTemp = _mm_move_ss(vTemp, vValues);
	// Height / AspectRatio,0,0,0
	Matrix4x4f M;
	M.Matrix.row0 = vTemp;
	// 0,Height,0,0
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_XMMaskY);
	M.Matrix.row1 = vTemp;
	// x=fRange,y=-fRange * NearZ,0,1.0f
	vTemp = _mm_setzero_ps();
	vValues = _mm_shuffle_ps(vValues, g_XMIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
	// 0,0,fRange,1.0f
	vTemp = _mm_shuffle_ps(_mm_setzero_ps(), vValues, _MM_SHUFFLE(3, 0, 0, 0));
	M.Matrix.row2 = vTemp;
	// 0,0,-fRange * NearZ,0.0f
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
	M.Matrix.row3 = vTemp;

	return M;
}
__forceinline Matrix4x4f OrthogonalMatrix(float width, float height, float NearZ, float FarZ)
{
	using namespace DirectX;

	Matrix4x4f M;
	const float fRange = 1.0f / (FarZ - NearZ);
	// Note: This is recorded on the stack
	Tuple::Vector rMem
	{
		2.0f / width,
		2.0f / height,
		fRange,
		-fRange * NearZ
	};
	// Copy from memory to SSE register
	Tuple::Vector vValues = rMem;
	Tuple::Vector vTemp = _mm_setzero_ps();
	// Copy x only
	vTemp = _mm_move_ss(vTemp, vValues);
	// 2.0f / ViewWidth,0,0,0
	M.Matrix.row0 = vTemp;
	// 0,2.0f / ViewHeight,0,0
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_XMMaskY);
	M.Matrix.row1 = vTemp;
	// x=fRange,y=-fRange * NearZ,0,1.0f
	vTemp = _mm_setzero_ps();
	vValues = _mm_shuffle_ps(vValues, g_XMIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
	// 0,0,fRange,0.0f
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
	M.Matrix.row2 = vTemp;
	// 0,0,-fRange * NearZ,1.0f
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
	M.Matrix.row3 = vTemp;

	return M;
}
#define DefaultOrientation IdentityMat4x4f()

#pragma warning(pop)
#endif