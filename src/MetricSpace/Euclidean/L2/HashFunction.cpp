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
        : Generic::HashFunction<DataPoint>(l), mLines(l, m), mConstants(l, m), mHashFunctionWeights(l, m), mWindow(w)
        {
            Util::Random uniformRandom;
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

            for (unsigned int i = 0; i < mConstants.getColSize(); ++i)
            {
                for (double& c : mConstants.row(i))
                {
                    c = uniformRandom.nextDouble(0.0f, w);
                }
            }

            for (unsigned int i = 0; i < mHashFunctionWeights.getColSize(); ++i)
            {
                for (int& c : mHashFunctionWeights.row(i))
                {
                    //NOTE: I have no idea what the max value should be. I kept 4096 because it "worked".
                    c = int{uniformRandom.nextInt(4096)};
                }
            }
        }

        uint64_t HashFunction::getKeyAtIndex (ConstPointRef p, unsigned int i) const
        {
            int64_t sumInt = 0;
            for (unsigned int j = 0; j < mLines.getRowSize(); ++j)
            {
                sumInt += int64_t{mHashFunctionWeights(i, j)} * (int64_t)floor( ( Util::dot(p, mLines(i, j)) + mConstants(i, j) ) / mWindow );
            }
            return ((sumInt < 0) ? -sumInt : sumInt); //% 4294967291;
        }
    }
}}
