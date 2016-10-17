#ifndef __STATICHASHTABLE_HPP__
#define __STATICHASHTABLE_HPP__

#include <stdexcept>
#include <functional>

#include "Array.hpp"

template <typename T>
class StaticHashMap
{
  public:

    struct Pair
    {
      unsigned int key;
      T target;
      Pair (unsigned int k, T&& t) : key(k), target(std::move(t)) {};
      Pair (unsigned int k, const T& t) : key(k), target(t) {};

      //NOT CopyConstructible :: avoid copies, pairs are always unique in a HashTable
      Pair (const Pair&) = delete;
      //MoveConstructible
      Pair (Pair&& other) : Pair(other.key, std::move(other.target)) {};

      //NOT CopyAssignable :: same reason
      Pair& operator= (const Pair& other) = delete;
      //MoveAssignable
      Pair& operator= (Pair&& other)
      {
          key = other.key;
          target = std::move(other.target);

          return *this;
      }
    };

  private:

	Array<Array<Pair>> mTable;

  public:

	explicit StaticHashMap (const unsigned int size) : mTable(size) {}

    StaticHashMap (StaticHashMap&& other) : mTable(std::move(other.mTable)) {}

	~StaticHashMap () {}

    bool exists (const uint64_t key) const
    {
        for (const Pair& x : mTable[key % (uint64_t)mTable.getLength()])
        {
            if (x.key == key) return true;
        }
        return false;
    }

    void add (const uint64_t key, const T& target)
    {
        //if (exists(key)) throw std::invalid_argument( "Key already exists." );

        mTable[key % (uint64_t)mTable.getLength()].pushBack(Pair(key, target));
    }

    void add (const uint64_t key, T&& target)
    {
        //if (exists(key)) throw std::invalid_argument( "Key already exists." );

        mTable[key % (uint64_t)mTable.getLength()].pushBack(Pair(key, std::move(target)));
    }

    const Array<Pair>& operator[] (const uint64_t key) const
    {
        return mTable[key % (uint64_t)mTable.getLength()];
    }

    void for_each (const std::function<void (const Pair&)> func) const
    {
        for (const Array<Pair>& bucket : mTable)
        {
            for (const Pair& x : bucket)
            {
                func (x);
            }
        }
    }

    void for_each_value (const std::function<void (const T&)> func) const
    {
        for_each([&] (const Pair& x)
        {
            func(x.target);
        });
    }

    void for_each_key (const std::function<void (const uint64_t&)> func) const
    {
        for_each([&] (const Pair& x)
        {
            func(x.key);
        });
    }

    typename Array<Array<Pair>>::sizeType getLength() const
    {
        return mTable.getLength();
    }

    template <typename M>
    friend std::ostream& operator<< (std::ostream& os, const StaticHashMap<M>& ht);
};

template <typename T>
std::ostream& operator<< (std::ostream& os, const StaticHashMap<T>& ht)
{
    os << '{';
    bool first = true;
    ht.for_each([&] (const typename StaticHashMap<T>::Pair& x)
    {
        if (!first) os << ", "; else first = false;

        os << x.key << " : " << x.target;
    });
    os << '}';

    return os;
}


#endif /* end of include guard: __STATICHASHTABLE_HPP__ */
