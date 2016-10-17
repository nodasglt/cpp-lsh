#ifndef __UTIL_RANDOM_HPP__
#define __UTIL_RANDOM_HPP__

#include <cinttypes>

namespace Util
{
    struct Random
    {
        int nextInt () const;

        /*
        ** Generate a number in [0, max]
        */
        int nextInt (int max) const;

        /*
        ** Generate a number in [1, max]
        */
        int nextInt (int min, int max) const;

        double nextDouble () const;

        double nextDouble (double min, double max) const;
    };

    struct GaussianRandom
    {
        GaussianRandom();

        double nextDouble () const;

        double nextDouble (double mean, double stddev) const;

    private:
        mutable bool mIsStored;
        mutable double mX, mY;
        Random mRandom;
    };
}

#endif /* end of include guard: __UTIL_RANDOM_HPP__ */
