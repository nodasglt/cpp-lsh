#ifndef __UTIL_RANDOM_HPP__
#define __UTIL_RANDOM_HPP__

#include <cstdlib>

namespace Util
{
    struct Random
    {
        uint64_t nextInt ()
        {
            return std::rand();
        }

        /*
        ** Generate a number in [0, max]
        */
        uint64_t nextInt (uint64_t max)
        {
            uint64_t num_bins = max + 1;
            uint64_t num_rand = (uint64_t)RAND_MAX + 1;
            uint64_t bin_size = num_rand / num_bins;
            uint64_t defect   = num_rand % num_bins;

            int64_t x;

            do
            {
                x = nextInt();
            }
            while (num_rand - defect <= (unsigned long)x);

            return x / bin_size;
        }

        /*
        ** Generate a number in [1, max]
        */
        int64_t nextInt (int64_t min, int64_t max)
        {
            return nextInt((uint64_t)(max - min)) + min;
        }

        double nextDouble ()
        {
            return (double)nextInt() / RAND_MAX;
        }

        double nextDouble (double min, double max)
        {
            return (nextDouble() * (max - min)) + min;
        }
    };

    class GaussianRandom
    {

    };
}

#endif /* end of include guard: __UTIL_RANDOM_HPP__ */
