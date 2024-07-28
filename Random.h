#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <intrin.h>
#include <climits>
#include "NaiveMath.h"

namespace RANDOM
{
    [[maybe_unused]] inline static uint32_t Wang_Hash(uint32_t seed) noexcept
    {
        seed = 9 * ((seed ^ 61) ^ (seed >> 16));
        seed = 0x27d4eb2d * (seed ^ (seed >> 4));
        seed = seed ^ (seed >> 15);

        return seed;
    }

    [[maybe_unused]] inline static uint32_t PCG_Hash(uint32_t input) noexcept
    {
        const uint32_t state = input * 747796405u + 2891336453u;
        const uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
        return (word >> 22u) ^ word;
    }

    [[maybe_unused]] inline static float RandomValue(uint32_t value) noexcept
    {
        static constexpr float min = 1.0f / (float)UINT32_MAX + 1u;
        return (float)PCG_Hash(value) * min;
    }

    inline thread_local std::default_random_engine generator = {};

    template <typename NumericType = float>
    [[nodiscard]] inline static float RandomInterval(NumericType min = 0.0f, NumericType max = 0.999999f) noexcept
    {
        static std::uniform_real_distribution<NumericType> toReturn(min, max);
        return toReturn(generator);
    };
};

inline Vec3f RandomInUnitSphere() noexcept
{
    //static uint32_t value = 0xdeadbeef;
    //RANDOM::RandomValue(value)
    Vec3f p;
    do
    {
        //++value;
        p = 2.0f * Vec3f(RANDOM::RandomInterval(), RANDOM::RandomInterval(), RANDOM::RandomInterval()) - Vec3f(1, 1, 1);
    } while (p.squared_length() >= 1.0f);
    return p;
}

inline Vec3f RandomInUnitDisk() noexcept
{
    Vec3f p;
    do
    {
        p = 2.0f * Vec3f(RANDOM::RandomInterval(), RANDOM::RandomInterval(), RANDOM::RandomInterval()) - Vec3f(1.0f, 1.0f, 0.0f);
    } while (dot(p,p) >= 1.0f);
    return p;
}

#endif