#include "Containers/Matrix.hpp"
#include "L2Space.hpp"

template<typename T>
class DataSet
{
    virtual T operator[] (unsigned int i) const = 0;

    virtual unsigned int getPointNum () const = 0;
};
