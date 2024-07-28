#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <memory>
#include <algorithm>
#include <execution>
#include "Renderer.h"
#include "Sphere.h"
#include "Camera.h"
#include "Random.h"
#include "Material.h"

//#define SINGLE_THREADED

class RaytracingInAWeekend : public Application
{
public:

	RaytracingInAWeekend()
	{
		ClearScreenEveryFrame(false);
		BuildWorld();
	}

	void BuildWorld() noexcept
	{
		aspectRatio = float(canvasWidth) / float(canvasHeight);

		Vec3f lookFrom(6.0f, 1.5f, 3.0f);
		Vec3f lookAt(2.0f, 1.0f, 0);
		float distToFocus = (lookFrom - lookAt).length();
		float aperture = 0.04f;
		float fieldOfView = 70.0f;

		worldCam = Camera(lookFrom, lookAt, Vec3f(0, 1.0f, 0), aspectRatio, fieldOfView, aperture, distToFocus);

		World.emplace_back(std::make_unique<Sphere>(1000.0f, Vec3f(0, -1000.0f, -2.0f)));  // lambertian
		World[0].get()->material.SetLambertian(Vec3f(0.35f, 0.15f, 0.35f));
		
		size_t sphereCount = 1;
		for (size_t i = 1; i < 7; i++)
		{
			for (size_t j = 0; j < 8; j++)
			{
				float chooseMat = RANDOM::RandomInterval();
				Vec3f center(i + 0.9f * RANDOM::RandomInterval(-float(j+i), float(j)), 0.2f, j + 0.9f * RANDOM::RandomInterval(-float(i+j), float(j)));
				
				if ((center - Vec3f(4.0f, 0.2f, 0.0f)).length() > 0.9f)
				{
					if (chooseMat < 0.2f) // metallic
					{
						World.emplace_back(std::make_unique<Sphere>(0.2f, center));
						World[sphereCount++].get()->material.SetMetallic(Vec3f(0.5f * (RANDOM::RandomInterval() + RANDOM::RandomInterval()), 0.5f * (RANDOM::RandomInterval() + RANDOM::RandomInterval()), 0.5f * (RANDOM::RandomInterval() + RANDOM::RandomInterval())), RANDOM::RandomInterval());
					}
					else if (chooseMat < 0.7f) // diffuse
					{
						World.emplace_back(std::make_unique<Sphere>(0.2f, center));
						World[sphereCount++].get()->material.SetLambertian(Vec3f(RANDOM::RandomInterval()* RANDOM::RandomInterval(), RANDOM::RandomInterval()* RANDOM::RandomInterval(), RANDOM::RandomInterval()* RANDOM::RandomInterval()));
					}
					else // dieletric
					{
						World.emplace_back(std::make_unique<Sphere>(0.2f, center));
						World[sphereCount++].get()->material.SetDieletric(1 + RANDOM::RandomInterval(0.0f, 1.0f));
					}
				}
			}
		}
		World.emplace_back(std::make_unique<Sphere>(1.0f, Vec3f(0, 1.0f, 0)));
		World[sphereCount++].get()->material.SetDieletric(1.5f);
		World.emplace_back(std::make_unique<Sphere>(1.0f, Vec3f(-4.0f, 1.0f, 0)));
		World[sphereCount++].get()->material.SetLambertian(Vec3f(0.4f,0.2f,0.1f));
		World.emplace_back(std::make_unique<Sphere>(1.0f, Vec3f(4.0f, 1.0f, 0)));
		World[sphereCount++].get()->material.SetMetallic(Vec3f(0.7f, 0.6f, 0.5f), 0.15f);

		m_sphereCount = sphereCount;
	}

	void OnUpdate(float dt) noexcept override
	{
		static float dtAcc = 0;
		static std::string titleBar;
		dtAcc += dt;

		static size_t currentSampleIndex = 1;

		if (dtAcc > 1.f)
		{
			titleBar = "Samples: " + std::to_string(currentSampleIndex) + ", FPS: " + std::to_string(currentFPS) + ", Spheres: " + std::to_string(m_sphereCount);
			SetWindowTitle(titleBar.c_str());
			dtAcc = 0;
		}

#ifdef SINGLE_THREADED
		for (size_t y = 0; y < canvasHeight; y++)
		{
			for (size_t x = 0; x < canvasWidth; x++)
			{
				const float u = float(x) / float(canvasWidth - 1);
				const float v = float(canvasHeight - 1 - y) / float(canvasHeight - 1); // Invert Y axis

				DrawPixel((uint16_t)x, (uint16_t)y, RayColor(worldCam.GetRay(u, v), 0), currentSampleIndex);
			}
		}
#else
		static std::vector<size_t> indices(canvasHeight * canvasWidth);
		std::iota(indices.begin(), indices.end(), 0);

		std::for_each(std::execution::par, indices.begin(), indices.end(), [&](size_t idx) noexcept -> void
			{
				const size_t x = idx % canvasWidth;
				const size_t y = idx / canvasWidth;

				const float u = static_cast<float>(x + RANDOM::RandomInterval()) / static_cast<float>(canvasWidth - 1);
				const float v = static_cast<float>(canvasHeight - 1 - y + RANDOM::RandomInterval()) / static_cast<float>(canvasHeight - 1); // Invert Y axis

				DrawPixel(static_cast<uint16_t>(x), static_cast<uint16_t>(y), RayColor(worldCam.GetRay(u, v), 0), currentSampleIndex);
			});
#endif
		++currentSampleIndex;
	}

	Vec3f RayColor(const Ray& r, int depth)
	{
		HitRegistry rec;

		if (ClosestHit(r, 0.001f, 5000.1f, &rec))
		{
			Ray scattered;
			Vec3f attenuation;
			
			if (depth < 50 && rec.material.Scatter(r, &rec, attenuation, scattered))
			{
				return attenuation * RayColor(scattered, depth + 1);
			}
			else
			{
				return Vec3f(0, 0, 0);
			}
		}
		else
		{
			const Vec3f unit_direction = unit_vector(r.direction);
			const float t = 0.5f * (unit_direction.y + 1.0f);
			return (1.0f-t) * Vec3f(1.0f, 1.0f, 1.0f) + t*Vec3f(0.5f, 0.7f, 1.0f);
		}
	}

	bool ClosestHit(const Ray& r, float t_min, float t_max, HitRegistry* rec) noexcept
	{
		bool hit_anything = false;
		HitRegistry temp_rec;

		for (const auto& object : World)
		{
			if (object->HIT(r, &temp_rec, t_min, t_max))
			{
				hit_anything = true;
				t_max = temp_rec.t;
				*rec = temp_rec;
			}
		}
		return hit_anything;
	}

private:
	std::vector<std::unique_ptr<Hittable>> World = {};
	float aspectRatio = 16.0f / 9.0f;
	Camera worldCam;
	size_t m_sphereCount = 0;
};

#endif