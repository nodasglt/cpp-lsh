#include <chrono>
#include <random>

#include "HashFunction.hpp"
#include "Util/Random.hpp"
#include "Util/VectorMath.hpp"


namespace MetricSpace {
namespace Euclidean
{
    namespace L2
    {
        HashFunction::HashFunction (unsigned int l, unsigned int m, unsigned int d, double w)
        : Generic::HashFunction<DataPoint>(l), lines(l, m), constants(l, m), hashFunctionWeights(l, m), window(w)
        {
            Util::Random uniformRandom;
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

            for (unsigned int i = 0; i < constants.getColSize(); ++i)
            {
                for (double& c : constants.row(i))
                {
                    c = uniformRandom.nextDouble(0.0f, w);
                }
            }

            for (unsigned int i = 0; i < hashFunctionWeights.getColSize(); ++i)
            {
                for (int& c : hashFunctionWeights.row(i))
                {
                    c = int{uniformRandom.nextInt(4096)};
                }
            }
        }

        uint64_t HashFunction::getKeyAtIndex (ConstPointRef p, unsigned int i) const
        {
            int64_t sumInt = 0;
            for (unsigned int j = 0; j < lines.getRowSize(); ++j)
            {
                sumInt += int64_t{hashFunctionWeights(i, j)} * (int64_t)floor( ( Util::dot(p, lines(i, j)) + constants(i, j) ) / window );
            }
            return ((sumInt < 0) ? -sumInt : sumInt); //% 4294967291;
        }
    }
}}
