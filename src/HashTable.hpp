#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdexcept>
#include <functional>

#include "Array.hpp"

template <typename T>
class HashTable
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

	explicit HashTable (const unsigned int size) : mTable(size) {}

	~HashTable () {}

    bool exists (const unsigned int key) const
    {
        for (const Pair& x : mTable[key % mTable.getLength()])
        {
            if (x.key == key) return true;
        }
        return false;
    }

    void add (const unsigned int key, const T& target)
    {
        if (exists(key)) throw std::invalid_argument( "Key already exists." );

        mTable[key % mTable.getLength()].pushBack(Pair(key, target));
    }

    void add (const unsigned int key, T&& target)
    {
        if (exists(key)) throw std::invalid_argument( "Key already exists." );

        mTable[key % mTable.getLength()].pushBack(Pair(key, std::move(target)));
    }

    void remove (const unsigned int key)
    {
        auto& bucket = mTable[key % mTable.getLength()];

        for (const Pair& x : bucket)
        {
            if (x.key == key) return bucket.remove(&x);
        }

        throw std::invalid_argument( "Item not in HashTable." );
    }

    T& operator[] (const unsigned int key)
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

    void for_each_key (const std::function<void (const int&)> func) const
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
    friend std::ostream& operator<< (std::ostream& os, const HashTable<M>& ht);
};

template <typename T>
std::ostream& operator<< (std::ostream& os, const HashTable<T>& ht)
{
    os << '{';
    bool first = true;
    ht.for_each([&] (const typename HashTable<T>::Pair& x)
    {
        if (!first) os << ", "; else first = false;

        os << x.key << " : " << x.target;
    });
    os << '}';

    return os;
}

#endif
