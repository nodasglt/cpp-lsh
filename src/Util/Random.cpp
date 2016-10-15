#include <cstdlib>

#include "Random.hpp"

namespace Util
{
    uint64_t Random::nextInt () const
    {
        return std::rand();
    }

    /*
    ** Generate a number in [0, max]
    */
    uint64_t Random::nextInt (uint64_t max) const
    {
        uint64_t randomMax = (uint64_t)RAND_MAX + 1;

        int64_t x;

        do
        {
            x = (uint64_t)nextInt();
        }
        while ((randomMax - randomMax % (max + 1)) <= x);

        return x / (randomMax / (max + 1));
    }

    /*
    ** Generate a number in [1, max]
    */
    int64_t Random::nextInt (int64_t min, int64_t max) const
    {
        return nextInt((uint64_t)(max - min)) + min;
    }

    double Random::nextDouble () const
    {
        return (double)nextInt() / RAND_MAX;
    }

    double Random::nextDouble (double min, double max) const
    {
        return (nextDouble() * (max - min)) + min;
    }
}
