#include "DistanceFunction.hpp"

namespace MetricSpace
{
    namespace DistMatrix
    {
        DistanceFunction::DistanceFunction(Matrix<double>&& distances) : distanceTable(std::move(distances)) {}

        double DistanceFunction::operator() (unsigned x, unsigned y) const
        {
            return distanceTable(x, y);
        }

        unsigned DistanceFunction::operator[] (unsigned int i) const
        {
            return i;
        }

        unsigned int DistanceFunction::getPointNum () const
        {
            return distanceTable.getRowSize();
        }
    }
}
