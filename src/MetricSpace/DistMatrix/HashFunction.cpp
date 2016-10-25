#include "HashFunction.hpp"
#include "Util/Random.hpp"
#include "Containers/BitArray.hpp"


namespace MetricSpace
{
    namespace DistMatrix
    {
        HashFunction::HashFunction (unsigned int hashTablesNum, unsigned int functionsPerHashTable, const DistanceFunction& dist)
         : Generic::HashFunction<DataPoint>(hashTablesNum), distFunc(dist), lines(hashTablesNum, functionsPerHashTable)
        {
            Util::Random uniformRandom;

            auto pointNum = distFunc.getPointNum();

            for (unsigned i = 0; i < hashTablesNum; ++i)
            {
                for (unsigned j = 0; j < functionsPerHashTable; ++j)
                {
                    unsigned x, y;
                    do
                    {
                        x = uniformRandom.nextInt(pointNum - 1) ;
                        y = uniformRandom.nextInt(pointNum - 1) ;
                    }
                    while (x == y);

                    double sum = 0.0;
                    for(unsigned n = 0; n < pointNum; ++n)
                    {
                        sum += project(x, y, n);
                    }

                    double midValue = sum / (double)pointNum;

                    lines(i,j).x = x;
                    lines(i,j).y = y;
                    lines(i,j).midValue = midValue;
                }
            }
        }

        double HashFunction::project (ConstPointRef x, ConstPointRef y, ConstPointRef indexToProject) const
        {
            auto distX = distFunc(x, indexToProject);
            auto distY = distFunc(y, indexToProject);
            auto distXY = distFunc(x, y);

            return (distX * distX + distY * distY + distXY * distXY) / (2.0f * distXY);
        }

        double HashFunction::project (line l, ConstPointRef indexToProject) const
        {
            return project(l.x, l.y, indexToProject);
        }

        uint64_t HashFunction::getKeyAtIndex (ConstPointRef p, unsigned int i) const
        {
        	BitArray<64> hash;

            for(unsigned j = 0; j < lines.getRowSize(); j++)
            {
                hash[j] = (project(lines(i,j), p) >= lines(i, j).midValue);
            }

            return (uint64_t)hash ;
        }
    }
}
