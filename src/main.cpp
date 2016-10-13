#include <iostream>
#include <cstdlib>

#include "VectorSet.hpp"
#include "Containers/Array.hpp"
#include "EuclideanDistance.hpp"
#include "EuclideanHash.hpp"

int main(int argc, char const* argv[])
{
    L2::HashFunction(5,5,5,5);
    L2::DistanceFunction();

    return 0;
}

/*
int main (int argc, char* argv[])
{
    L2::HashFunction func(2, 4, 5, 2.0f);

    L2::Point p;

    p.reserve(5);

    for (unsigned int i = 0; i < 5; ++i)
    {
        p.pushBack((double)std::rand()/RAND_MAX + 5.0f);
    }

    std::cout << p << std::endl;

    for (auto x : func(p))
    {
        std::cout << x << std::endl;
    }

	return 0;
}
*/
