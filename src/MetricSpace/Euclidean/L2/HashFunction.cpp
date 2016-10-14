#include "HashFunction.hpp"

#include <random>
#include <chrono>

namespace MetricSpace {
namespace Euclidean
{
    namespace L2
    {
        HashFunction::HashFunction (unsigned int l, unsigned int m, unsigned int d, double w) : lsh::HashFunction<DataPoint>(l), lines(l, m), constants(l, m), window(w)
        {
            std::default_random_engine generator(8453438388); //std::chrono::system_clock::now().time_since_epoch().count()
            std::uniform_real_distribution<double> udist(0.0f, w);
            std::normal_distribution<double> ndist(0.0f, 1.0f);

            for (unsigned int i = 0; i < lines.getColSize(); ++i)
            {
                for (unsigned int j = 0; j < lines.getRowSize(); ++j)
                {
                    Array<double>& vec = lines(i, j);
                    vec.reserve(d);
                    for (unsigned int k = 0; k < d; ++k)
                    {
                        vec.emplaceBack(ndist(generator)); /* TODO: Use Normal Dist */
                    }
                }
            }

            for (unsigned int i = 0; i < constants.getColSize(); ++i)
            {
                for (double& c : constants.row(i))
                {
                    c = udist(generator);
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

        uint64_t HashFunction::getKeyAtIndex (const PointRef p, unsigned int i) const
        {
            double sum = 0.0f;
            for (unsigned int j = 0; j < lines.getRowSize(); ++j)
            {
                sum += ((dot(p, const_cast<Array<double>&>(lines(i, j))) + constants(i, j)) / window);
            }
            return (uint64_t)(((sum > 0) ? sum : -sum));
        }
    }
}}
