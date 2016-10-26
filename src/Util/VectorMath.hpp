#ifndef __UTIL_VECTORMATH_HPP__
#define __UTIL_VECTORMATH_HPP__

#include <cmath>
#include <iostream>

namespace Util
{
    template<typename VecTypeA, typename VecTypeB>
    double dot (const VecTypeA& x, const VecTypeB& y)
    {
        double sum = 0.0f;
        for (unsigned int i = 0; i < x.getLength(); ++i)
        {
            sum += x[i] * y[i];
        }
        return sum;
    }

    template<typename VecType>
    double magnitude (const VecType& x)
    {
        double sum = 0.0f;
        for (unsigned int i = 0; i < x.getLength(); ++i)
        {
            sum += x[i] * x[i];
        }
        return std::sqrt(sum);
    }

    template<typename VecTypeA, typename VecTypeB>
    double cosineSim (const VecTypeA& x, const VecTypeB& y)
    {
        return dot(x, y) / (magnitude(x) * magnitude(y));
    }

    template<typename VecType, typename ScalarType>
    void mult (VecType& x, ScalarType y)
    {
        for (unsigned int i = 0; i < x.getLength(); ++i)
        {
            x[i] *= y;
        }
    }
}

#endif /* end of include guard: __UTIL_VECTORMATH_HPP__ */
