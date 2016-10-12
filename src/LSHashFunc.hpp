#include "Array.hpp"
#include "Matrix.hpp"
#include <cstdlib>

namespace lsh
{
    struct HashFunction
    {
        virtual Matrix<int> operator() (const Point& p) const = 0;
    };

    struct EuclideanHashFunction : HashFunction
    {
        Matrix<Point> lines; /* TODO: Go with Matrix2D and Points (Arrays) */
        Matrix<double> constants;

        const double window;

        EuclideanHashFunction(unsigned int l, unsigned int m, unsigned int d, double w) : HashFunction(), lines(l, m), constants(l, m), window(w)
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

            std::cout << lines << std::endl;
        }

        Matrix<int> operator() (const Point& p) const
        {
            Matrix<int> keys(constants.getColSize(), constants.getRowSize());

            for (unsigned int i = 0; i < lines.getColSize(); ++i)
            {
                for (unsigned int j = 0; j < lines.getRowSize(); ++j)
                {
                    std::cout << p << std::endl;
                    keys(i, j) = (dot(p, lines(i, j)) + constants(i, j));
                }
            }

            return keys;
        }
    };
};
