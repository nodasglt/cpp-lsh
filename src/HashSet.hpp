#include <functional>

#include "HashFunction.hpp"
#include "DistanceFunction.hpp"
#include "Containers/Array.hpp"
#include "Containers/StaticHashMap.hpp"

namespace lsh
{
    template<typename PointType>;
    class HashSet
    {
    private:
        const HashFunction<PointType>& mHashFunc;
        const DistanceFunction<PointType>& mDistFunc;
        Array<HashMap</*uint64_t, */PointType>> mHashMapArray;
        /* TODO: Store DataPoints to a list */

    public:
        HashSet (const HashFunction<PointType>& hashFunc, const DistanceFunction<PointType>& distFunc, unsigned int hashMapSize) : mHashFunc(hashFunc), mDistFunc(distFunc), mHashMapArray()
        {
            mHashMapArray.reserve(mHashFunc.getKeyNum());

            for (unsigned int i = 0; i < mHashFunc.getKeyNum(); ++i)
            {
                mHashMapArray.emplaceBack(hashMapSize);
            }
        }

        void add (const PointType& x)
        {
            auto keySet = mHashFunc(x);

            for (auto i = mHashMapArray.getLength(); i >= 0; --i)
            {
                mHashMapArray[i].add(keySet[i], x);
            }
        }

        unsigned int forEachNNinRange (unsigned int r, std::function<void (const PointType&)> func);

        const PointType& operator[] (const PointType& p);
    }
}
