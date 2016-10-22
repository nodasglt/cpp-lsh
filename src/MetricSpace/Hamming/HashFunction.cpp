#include <chrono>
#include <random>

#include "HashFunction.hpp"
#include "Util/Random.hpp"


namespace MetricSpace
{
    namespace Hamming
    {
        HashFunction::HashFunction (unsigned int l, unsigned int m, unsigned int d)
         : Generic::HashFunction<DataPoint>(l), returnBitIndex(l, m)
        {
            assert(d <= 64);

            Util::Random uniformRandom;

            for (unsigned int n = 0; n < returnBitIndex.getColSize(); ++n)
            {
                for (auto& i : returnBitIndex.row(n))
                {
                    i = uniformRandom.nextInt(d - 1);
                }
            }
        }

        uint64_t HashFunction::getKeyAtIndex (ConstPointRef p, unsigned int i) const
        {
            BitArray<64> hash;
            for (unsigned int j = 0; j < returnBitIndex.getRowSize(); ++j)
            {
                hash[j] = p[returnBitIndex(i, j)];
            }
            return (uint64_t)hash;
        }
    }
}
