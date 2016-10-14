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

    Euclidean::L2::HashFunction hashFunc(5, 5, 100, 2.0f);

    Euclidean::L2::DistanceFunction distFunc;

    lsh::HashSet<Euclidean::DataPoint> hashSet(hashFunc, distFunc, 10, data);

    hashSet.add(data);

    hashSet.forEachPointInRange(100000.0f, data[0], [] (unsigned int x)
    {
        std::cout << x << std::endl;
    });

    return 0;

    for (unsigned int i = 0; i < data.getPointNum(); ++i)
    {
        if (distFunc(data[i], data[0]) < 100000.0f)
        {
            std::cout << i << std::endl;
        }
    }

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
