#include <iostream>
#include <cstdlib>

#include "VectorSet.hpp"

int main(int argc, char const* argv[])
{
    VectorSetParser parser;

    VectorSet data = parser.parse(argv[1]);

    std::cout << data[2] << std::endl;

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
