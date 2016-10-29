#include "HashFunction.hpp"
#include "Util/Random.hpp"
#include "Containers/BitArray.hpp"


namespace MetricSpace
{
    namespace DistMatrix
    {
        HashFunction::HashFunction (unsigned int hashTablesNum, unsigned int functionsPerHashTable, const DistanceFunction* dist)
         : Generic::HashFunction<DataPoint>(hashTablesNum), mDistFunc(dist), mLines(hashTablesNum, functionsPerHashTable)
        {
            assert(functionsPerHashTable <= 64);

            Util::Random uniformRandom;

            auto pointNum = mDistFunc->getPointNum();

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

                    mLines(i, j).x = x;
                    mLines(i, j).y = y;
                    mLines(i, j).length = (*mDistFunc)(x, y);

                    double sum = 0.0;
                    for(unsigned n = 0; n < pointNum; ++n)
                    {
                        sum += project(mLines(i, j), n);
                    }

                    double midValue = sum / (double)pointNum;

                    mLines(i, j).midValue = midValue;
                }
            }
        }
        void HashFunction::setDistFunction (const DistanceFunction* dist)
        {
            mDistFunc = dist;
        }

        double HashFunction::project (line l, ConstPointRef indexToProject) const
        {
            auto distX = (*mDistFunc)(indexToProject, l.x);
            auto distY = (*mDistFunc)(indexToProject, l.y);

            return (distX * distX + distY * distY + l.length * l.length) / (2.0f * l.length);
        }

        uint64_t HashFunction::getKeyAtIndex (ConstPointRef p, unsigned int i) const
        {
        	BitArray<64> hash;

            for(unsigned j = 0; j < mLines.getRowSize(); j++)
            {
                hash[j] = (project(mLines(i,j), p) >= mLines(i, j).midValue);
            }

            return (uint64_t)hash ;
        }
    }
}
