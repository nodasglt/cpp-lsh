#include <iostream>
#include <cstdlib>

#include "MetricSpace/Euclidean/DataSet.hpp"
#include "Containers/Array.hpp"
#include "MetricSpace/Euclidean/L2/Metric.hpp"
#include "MetricSpace/Euclidean/Euclidean.hpp"
#include "LocalitySensitiveHashing/HashSet.hpp"

int main(int argc, char const* argv[])
{
    using namespace MetricSpace;
    
    Euclidean::DataSet data = Euclidean::DataSetParser().parse(argv[1]);

    lsh::HashSet<Euclidean::DataPoint> hashSet(Euclidean::L2::HashFunction(5, 5, 5, 2.0f), Euclidean::L2::DistanceFunction(), 4);

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
