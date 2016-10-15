#ifndef __UTIL_RANDOM_HPP__
#define __UTIL_RANDOM_HPP__

#include <cinttypes>

namespace Util
{
    struct Random
    {
        uint64_t nextInt () const;

        /*
        ** Generate a number in [0, max]
        */
        uint64_t nextInt (uint64_t max) const;

        /*
        ** Generate a number in [1, max]
        */
        int64_t nextInt (int64_t min, int64_t max) const;

        double nextDouble () const;

        double nextDouble (double min, double max) const;
    };

    class GaussianRandom
    {

    };
}

#endif /* end of include guard: __UTIL_RANDOM_HPP__ */
