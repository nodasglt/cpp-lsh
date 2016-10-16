#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

#include "MetricSpace/Euclidean/DataSet.hpp"
#include "Containers/Array.hpp"
#include "MetricSpace/Euclidean/L2/Metric.hpp"
#include "MetricSpace/Euclidean/Euclidean.hpp"
#include "LocalitySensitiveHashing/HashSet.hpp"

int main(int argc, char const* argv[])
{
    std::srand(std::time(nullptr));

    using namespace MetricSpace::Euclidean;

    auto dataSet = DataSetParser().parse(argv[1]);

    L2::HashFunction hashFunc(4, 20, 100, 2.0f);

    L2::DistanceFunction distFunc;

    lsh::HashSet<DataPoint> hashSet(hashFunc, distFunc, 125, dataSet);

    hashSet.add(dataSet);

    int ok = 0;

    for (unsigned int t = 0; t < 300; ++t)
    {
        Array<int> found;

        std::clock_t begin = std::clock();

        double sDist = std::numeric_limits<double>::max();
        unsigned int r = 999999;
        auto sum = hashSet.forEachPointInRange(250000.0f, dataSet[t], [&] (unsigned int x, double dist)
        {
            if (dist > 0 && dist < sDist)
            {
                sDist = dist;
                r = x;
            }
            found.emplaceBack(x);
        });

        if (r != 999999)
        {
            std::cout << "NN : " << t << " -> " << r << " sum: " << sum << std::endl;
        }
        else
        {
            std::cout << "NN : " << t << " -> [FAIL]" << " sum: " << sum << std::endl;
        }

        if (found.getLength() > 1) ++ok;

        std::clock_t end = std::clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

//        std::cout << "time: " << elapsed_secs << " sum: " << sum << std::endl;

//        std::clock_t begin2 = std::clock();

//        for (unsigned int i = 0; i < dataSet.getPointNum(); ++i)
//        {
//            auto x = distFunc(dataSet[i], dataSet[t]);
//            if (x < 1300.0f)
//            {
//                if (x) std::cout << t << "| " << i << " : " << x << "\t" << ((found.contains(i)) ? "[OK]" : "[FAIL]") << std::endl;
//            }
//        }
//
//        std::clock_t end2 = std::clock();
//        double elapsed_secs2 = double(end2 - begin2) / CLOCKS_PER_SEC;

//        std::cout << "time: " << elapsed_secs2 << std::endl;
    }

    std::cout << "stats: " << ok << std::endl;

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
