#ifndef __STATICHASHTABLE_HPP__
#define __STATICHASHTABLE_HPP__

#include <stdexcept>
#include <functional>

#include "Array.hpp"

template <typename KeyType, typename T>
class StaticHashMap
{
  public:

    struct Pair
    {
      KeyType key;
      T target;
      Pair (KeyType k, T&& t) : key(k), target(std::move(t)) {};
      Pair (KeyType k, const T& t) : key(k), target(t) {};

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

    bool exists (const KeyType key) const
    {
        for (const Pair& x : mTable[key % mTable.getLength()])
        {
            if (x.key == key) return true;
        }
        return false;
    }

    void add (const KeyType key, const T& target) /* Copy-Contruct */
    {
        if (exists(key)) throw std::invalid_argument( "Key already exists." );

        mTable[key % mTable.getLength()].pushBack(Pair(key, target));
    }

    void add (const KeyType key, T&& target) /* Move-Contruct */
    {
        if (exists(key)) throw std::invalid_argument( "Key already exists." );

        mTable[key % mTable.getLength()].pushBack(Pair(key, std::move(target)));
    }

    void remove (const KeyType key)
    {
        Array<Pair>& bucket = mTable[key % mTable.getLength()];

        for (typename Array<Pair>::sizeType i = 0; i < bucket.getLength(); i++)
        {
            if (bucket[i].key == key) return bucket.remove(i);
        }

        throw std::invalid_argument( "Item not in HashTable." );
    }

    T& operator[] (const KeyType key)
    {
        for (Pair& x : mTable[key % mTable.getLength()])
        {
            if (x.key == key) return x.target;
        }

        throw std::invalid_argument( "Item not in HashTable." );
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

    void for_each_key (const std::function<void (const KeyType&)> func) const
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

    template <typename K, typename M>
    friend std::ostream& operator<< (std::ostream& os, const StaticHashMap<K, M>& ht);
};

template <typename KeyType, typename T>
std::ostream& operator<< (std::ostream& os, const StaticHashMap<KeyType, T>& ht)
{
    os << '{';
    bool first = true;
    ht.for_each([&] (auto& x)
    {
        if (!first) os << ", "; else first = false;

        os << x.key << " : " << x.target;
    });
    os << '}';

    return os;
}


#endif /* end of include guard: __STATICHASHTABLE_HPP__ */
