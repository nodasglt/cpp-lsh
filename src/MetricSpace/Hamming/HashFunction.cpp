#include <chrono>
#include <random>

#include "HashFunction.hpp"
#include "Util/Random.hpp"


namespace MetricSpace
{
    namespace Hamming
    {
        HashFunction::HashFunction (unsigned int l, unsigned int m, unsigned int d)
         : Generic::HashFunction<DataPoint>(l), mSelectedBitIndices(l, m)
        {
            assert(d <= 64);

            Util::Random uniformRandom;

            for (unsigned int n = 0; n < mSelectedBitIndices.getColSize(); ++n)
            {
                for (auto& i : mSelectedBitIndices.row(n))
                {
                    i = uniformRandom.nextInt(d - 1);
                }
            }
        }

        uint64_t HashFunction::getKeyAtIndex (ConstPointRef p, unsigned int i) const
        {
            BitArray<64> hash;
            for (unsigned int j = 0; j < mSelectedBitIndices.getRowSize(); ++j)
            {
                hash[j] = p[mSelectedBitIndices(i, j)];
            }
            return (uint64_t)hash;
        }
    }
}
