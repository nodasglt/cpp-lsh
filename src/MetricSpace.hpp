#include "Containers/Array.hpp"

using Point = Array<double>;

double dot (const Point& x, const Point& y)
{
    double sum = 0.0f;
    for (unsigned int i = 0; i < x.getLength(); ++i)
    {
        sum = x[i] * y[i];
    }
    return sum;
}

namespace l2
{
    double dist (const Point& x, const Point& y)
    {
        double sum = 0.0f;
        for (unsigned int i = 0; i < x.getLength(); ++i)
        {
            double dif = x[i] - y[i];
            sum += dif * dif;
        }
        return sum;
    }
}
