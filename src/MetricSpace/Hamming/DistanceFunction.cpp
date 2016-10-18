#include "DistanceFunction.hpp"

namespace MetricSpace
{
    namespace Hamming
    {
        double DistanceFunction::operator() (ConstPointRef x, ConstPointRef y) const
        {
            return (x ^ y).count();
        }
    }
}
