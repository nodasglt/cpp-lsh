#include <cstdlib>
#include <cmath>

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

        uint64_t x;

        do
        {
            x = nextInt();
        }
        while ((randomMax - randomMax % (max + 1)) <= x);

        return x / (randomMax / (max + 1));
    }

    /*
    ** Generate a number in [min, max]
    */
    int64_t Random::nextInt (int64_t min, int64_t max) const
    {
        return nextInt((uint64_t)(max - min)) + min;
    }

    double Random::nextDouble () const
    {
        return nextInt() * (1.0f / RAND_MAX);
    }

    double Random::nextDouble (double min, double max) const
    {
        return (nextDouble() * (max - min)) + min;
    }

    GaussianRandom::GaussianRandom() : mIsStored(false), mX(0.0f), mY(0.0f), mRandom() {}

    double GaussianRandom::nextDouble ()
    {
        using namespace std;

        const double twoPi = 2.0f * 3.14159265358979323846f;

        if(!mIsStored)
        {
            mY = twoPi * mRandom.nextDouble();
            mX = sqrt((-2.0f) * log(1.0f - mRandom.nextDouble()));
        }

        mIsStored = !mIsStored;

        return mX * (mIsStored ? sin(mY) : cos(mY));
    }

    double GaussianRandom::nextDouble (double mean, double stddev)
    {
        return nextDouble() * stddev + mean;
    }
}
