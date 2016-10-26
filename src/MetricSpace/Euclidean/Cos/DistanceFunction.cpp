#include "DistanceFunction.hpp"
#include "Util/VectorMath.hpp"

namespace MetricSpace {
namespace Euclidean
{
    namespace Cos
    {
        double DistanceFunction::operator() (ConstPointRef x, ConstPointRef y) const
        {
            return 1.0f - Util::cosineSim(x, y);
        }
    }
}}
