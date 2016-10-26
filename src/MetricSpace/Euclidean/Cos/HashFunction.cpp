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
        : Generic::HashFunction<DataPoint>(l), lines(l, m)
        {
            Util::GaussianRandom normalRandom;

            for (unsigned int i = 0; i < lines.getColSize(); ++i)
            {
                for (unsigned int j = 0; j < lines.getRowSize(); ++j)
                {
                    Array<double>& vec = lines(i, j);
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
            for (unsigned int j = 0; j < lines.getRowSize(); ++j)
            {
                hash[j] = (Util::dot(lines(i, j), p) >= 0.0f);
                //if (i == 0) std::cout << Util::dot(lines(i, j), p) << std::endl;
            }
            //if (i == 0) std::cout << (uint64_t)hash << std::endl;
            return (uint64_t)hash;
        }
    }
}}
