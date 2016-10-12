#include "L2Space.hpp"

namespace L2
{
    HashFunction::HashFunction (unsigned int l, unsigned int m, unsigned int d, double w) : lsh::HashFunction<Point>(l), lines(l, m), constants(l, m), window(w)
    {
        for (unsigned int i = 0; i < lines.getColSize(); ++i)
        {
            for (unsigned int j = 0; j < lines.getRowSize(); ++j)
            {
                Point& vec = lines(i, j);
                vec.reserve(d);
                for (unsigned int k = 0; k < d; ++k)
                {
                    vec.emplaceBack(((double)std::rand())/RAND_MAX); /* TODO: Use Normal Dist */
                }
            }
        }

        for (unsigned int i = 0; i < constants.getColSize(); ++i)
        {
            for (double& c : constants.row(i))
            {
                c = ((double)std::rand())/RAND_MAX * w;
            }
        }
    }

    static double dot (const Point& x, const Point& y)
    {
        double sum = 0.0f;
        for (unsigned int i = 0; i < x.getLength(); ++i)
        {
            sum = x[i] * y[i];
        }
        return sum;
    }

    uint64_t HashFunction::getKeyAtIndex (const Point& p, unsigned int i) const
    {
        uint64_t sum = 0;
        for (unsigned int j = 0; j < lines.getRowSize(); ++j)
        {
            sum += (dot(p, lines(i, j)) + constants(i, j));
        }
        return sum;
    }

    double DistanceFunction::operator() (const Point& x, const Point& y) const
    {
        double sum = 0.0f;
        for (unsigned int i = 0; i < x.getLength(); ++i)
        {
            double dif = x[i] - y[i];
            sum += dif * dif;
        }
        return sum;
    }
};
