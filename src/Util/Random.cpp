#include <cstdlib>
#include <cmath>
#include <cassert>

#include "Random.hpp"

namespace Util
{
    int Random::nextInt () const
    {
        return std::rand();
    }

    /*
    ** Generate a number in [0, max]
    */
    int Random::nextInt (int max) const
    {
        assert(max > 0);

        int64_t randomMax = int64_t{RAND_MAX} + 1;

        int x;

        do
        {
            x = nextInt();
        }
        while ((randomMax - randomMax % (max + 1)) <= x);

        return x / (int)(randomMax / int64_t{ max + 1 });
    }

    /*
    ** Generate a number in [min, max]
    */
    int Random::nextInt (int min, int max) const
    {
        return nextInt(max - min) + min;
    }

    double Random::nextDouble () const
    {
        return nextInt() / double{ RAND_MAX };
    }

    double Random::nextDouble (double min, double max) const
    {
        return (nextDouble() * (max - min)) + min;
    }

    GaussianRandom::GaussianRandom() : mIsStored(false), mX(0.0f), mY(0.0f), mRandom() {}

    double GaussianRandom::nextDouble () const
    {
        using namespace std;

        static constexpr double twoPi = 2.0f * 3.14159265358979323846f;

        if(!mIsStored)
        {
            mY = twoPi * mRandom.nextDouble();
            mX = sqrt((-2.0f) * log(1.0f - mRandom.nextDouble()));
        }

        mIsStored = !mIsStored;

        return mX * (mIsStored ? sin(mY) : cos(mY));
    }

    double GaussianRandom::nextDouble (double mean, double stddev) const
    {
        return nextDouble() * stddev + mean;
    }
}
