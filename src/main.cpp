#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

#include "MetricSpace/Euclidean/L2/Metric.hpp"
#include "LocalitySensitiveHashing/HashSet.hpp"

int main(int argc, char const* argv[])
{
    std::srand(std::time(nullptr));

    using namespace MetricSpace::Euclidean;

    auto dataSet = DataSetParser().parse(argv[1]);

    L2::HashFunction hashFunc(4, 6, 100, 2.0f);

    L2::DistanceFunction distFunc;

    lsh::HashSet<DataPoint> hashSet(hashFunc, distFunc, 125, dataSet);

    int ok = 0;

    std::clock_t begin = std::clock();

    for (unsigned int t = 0; t < 100; ++t)
    {
        auto result = hashSet[dataSet[t]];

        if (result.found)
        {
            std::cout << "NN : " << t << " -> " << result.index << " sum: " << result.sum << std::endl;
        }
        else
        {
            std::cout << "NN : " << t << " -> [FAIL]" << " sum: " << result.sum << std::endl;
        }

        if (result.found) ++ok;
    }

    std::clock_t end = std::clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "time: " << elapsed_secs << std::endl;

    std::cout << "stats: " << ok << std::endl;

    return 0;
}

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
