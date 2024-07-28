#include "../Material.h"

bool Material::Scatter(const Ray& In, HitRegistry* rec, Vec3f& attenuation, Ray& scattered) const noexcept
{
	switch (type)
	{
		case MaterialType::LAMBERTIAN:
		{
			scattered = Ray(rec->p, (rec->p + rec->normal + RandomInUnitSphere()) - rec->p);
			attenuation = Albedo;
			return true;
		};

		case MaterialType::METALLIC:
		{
			const Vec3f reflected = reflect(unit_vector(In.direction), rec->normal);
			scattered = Ray(rec->p, reflected + Fuzz * RandomInUnitSphere());
			attenuation = Albedo;
			return (dot(scattered.direction, rec->normal) > 0);
		};

		case MaterialType::DIELECTRIC:
		{
			Vec3f outwardNormal;
			Vec3f refracted;
			Vec3f reflected = reflect(In.direction, rec->normal);
			float refractiveIndexRatio;
			attenuation = {1.0f, 1.0f, 1.0f};
			float reflectProbability = 1.0f;
			float cosine;

			if (dot(In.direction, rec->normal) > 0)
			{
				outwardNormal = -rec->normal;
				refractiveIndexRatio = RefractionIndex;
				cosine = RefractionIndex * dot(In.direction, rec->normal) / In.direction.length();
			}
			else
			{
				outwardNormal = rec->normal;
				refractiveIndexRatio = 1.0f / RefractionIndex;
				cosine = -dot(In.direction, rec->normal) / In.direction.length();
			}

			if (refract(In.direction, outwardNormal, refractiveIndexRatio, refracted))
			{
				reflectProbability = schlick(cosine, RefractionIndex);
			}

			if (RANDOM::RandomInterval() < reflectProbability)
			{
				scattered = Ray(rec->p, reflected);
			}
			else
			{
				scattered = Ray(rec->p, refracted);
			}

			return true;
		}
		default:
			return false;
	}
}
