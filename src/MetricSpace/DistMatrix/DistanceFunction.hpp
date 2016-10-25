#ifndef __METRICSPACE_DISTMATRIX_DISTANCEFUNCTION_HPP__
#define __METRICSPACE_DISTMATRIX_DISTANCEFUNCTION_HPP__

#include "../Generic/DistanceFunction.hpp"
#include "../Generic/DataSet.hpp"
#include "Containers/Matrix.hpp"
#include "DistMatrix.hpp"

namespace MetricSpace
{
    namespace DistMatrix
    {
        struct DistanceFunction : public Generic::DistanceFunction<DataPoint>, public Generic::DataSet<DataPoint>
        {
            DistanceFunction(Matrix<double>&& distances);

            double operator() (unsigned x, unsigned y) const override;

            unsigned operator[] (unsigned int i) const override;
            unsigned int getPointNum () const override;

        private:
            Matrix<double> distanceTable;
        };
    }
}

#endif /* end of include guard: __METRICSPACE_DISTMATRIX_DISTANCEFUNCTION_HPP__ */
