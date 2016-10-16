#include <chrono>
#include <random>

#include "HashFunction.hpp"
#include "Util/Random.hpp"


namespace MetricSpace {
namespace Euclidean
{
    namespace L2
    {
        HashFunction::HashFunction (unsigned int l, unsigned int m, unsigned int d, double w)
        : Generic::HashFunction<DataPoint>(l), lines(l, m), constants(l, m), rConstants(l, m), window(w)
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

            for (unsigned int i = 0; i < rConstants.getColSize(); ++i)
            {
                for (int32_t& c : rConstants.row(i))
                {
                    c = uniformRandom.nextInt();
                }
            }
        }

        static double dot (const HashFunction::PointRef x, const HashFunction::PointRef y)
        {
            double sum = 0.0f;
            for (unsigned int i = 0; i < x.getLength(); ++i)
            {
                sum = x[i] * y[i];
            }
            return sum;
        }

        inline const Block<double*, double> arrayToDataPoint (const Array<double>& array)
        {
            return {((array.getLength()) ? &(const_cast<double&>(array[0])) : nullptr), array.getLength(), 1};
        }

        uint32_t HashFunction::getKeyAtIndex (const PointRef p, unsigned int i) const
        {
            int64_t sumInt = 0;
            for (unsigned int j = 0; j < lines.getRowSize(); ++j)
            {
                sumInt += rConstants(i, j) * (int64_t)floor((dot(p, arrayToDataPoint(lines(i, j))) + constants(i, j)) / window);
            }
            return abs(sumInt) % 4294967291;
        }
    }
}}
