#ifndef __UTIL_RANDOM_HPP__
#define __UTIL_RANDOM_HPP__

#include <cinttypes>

namespace Util
{
    class Random
    {
    public:
        int nextInt () const;

        /*
        ** Range: [0, max]
        */
        int nextInt (int max) const;

        /*
        ** Range: [1, max]
        */
        int nextInt (int min, int max) const;

        /*
        ** Range: [0, 1]
        */
        double nextDouble () const;

        /*
        ** Range: [min, max]
        */
        double nextDouble (double min, double max) const;
    };

    class GaussianRandom
    {
    public:
        GaussianRandom();

        /*
        ** mean = 0
        ** stddev = 1
        */
        double nextDouble () const;

        double nextDouble (double mean, double stddev) const;

    private:
        mutable bool mIsStored;
        mutable double mX, mY;
        Random mRandom;
    };
}

#endif /* end of include guard: __UTIL_RANDOM_HPP__ */
