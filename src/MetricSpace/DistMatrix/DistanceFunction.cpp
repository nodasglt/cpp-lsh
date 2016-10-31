#include "DistanceFunction.hpp"
#include <cassert>

namespace MetricSpace
{
    namespace DistMatrix
    {
        double DistanceFunction::operator() (ConstPointRef x, ConstPointRef y) const
        {
            if (x.flag == y.flag)
            {
                if (x.flag == DataPoint::Flag::test)
                {
                    assert(0);
                }

                if (x.flag == DataPoint::Flag::data)
                {
                    return (x.id < y.id) ? x.values[y.id] : y.values[x.id];
                }
            }

            return (x.flag == DataPoint::Flag::test) ? x.values[y.id] : y.values[x.id];
        }
    }
}
