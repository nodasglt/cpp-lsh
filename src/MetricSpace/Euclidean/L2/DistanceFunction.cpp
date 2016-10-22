#include "DistanceFunction.hpp"
#include <cmath>

namespace MetricSpace {
namespace Euclidean
{
    namespace L2
    {
        double DistanceFunction::operator() (const PointRef x, const PointRef y) const
        {
            double sum = 0.0f;
            for (unsigned int i = 0; i < x.getLength(); ++i)
            {
                double dif = x[i] - y[i];
                sum += dif * dif;
            }
            return sqrt(sum);
        }
    }
}}
