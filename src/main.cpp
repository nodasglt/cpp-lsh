#include <iostream>
#include <cstdlib>

#include "HashTable.hpp"
#include "MetricSpace.hpp"
#include "Hash.hpp"

int main (int argc, char* argv[])
{
    lsh::EuclideanHashFunction func(2, 4, 5, 2.0f);

    Point p;

    p.reserve(5);

    for (unsigned int i = 0; i < 5; ++i)
    {
        p.pushBack((double)std::rand()/RAND_MAX + 5.0f);
    }

    std::cout << p << std::endl;

    std::cout << func(p) << std::endl;

	return 0;
}
