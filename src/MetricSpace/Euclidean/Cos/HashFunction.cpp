#include "HashFunction.hpp"
#include "Util/Random.hpp"
#include "Util/VectorMath.hpp"
#include "Containers/BitArray.hpp"


namespace MetricSpace {
namespace Euclidean
{
    namespace Cos
    {
        HashFunction::HashFunction (unsigned int l, unsigned int m, unsigned int d)
        : Generic::HashFunction<DataPoint>(l), mLines(l, m)
        {
            assert(m <= 64);

            Util::GaussianRandom normalRandom;

            for (unsigned int i = 0; i < mLines.getColSize(); ++i)
            {
                for (unsigned int j = 0; j < mLines.getRowSize(); ++j)
                {
                    Array<double>& vec = mLines(i, j);
                    vec.reserve(d);
                    for (unsigned int k = 0; k < d; ++k)
                    {
                        vec.emplaceBack(normalRandom.nextDouble());
                    }
                }
            }
        }

        uint64_t HashFunction::getKeyAtIndex (ConstPointRef p, unsigned int i) const
        {
            BitArray<64> hash;
            for (unsigned int j = 0; j < mLines.getRowSize(); ++j)
            {
                hash[j] = (Util::dot(mLines(i, j), p) >= 0.0f);
            }
            return (uint64_t)hash;
        }
    }
}}
