#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <iostream>
#include <functional>
#include <utility>
#include <array>

template <typename T>
class Array
{
  public:

    using sizeType = unsigned int;
    using iteratorType = T*;
    using constIteratorType = const T*;

  private:

    sizeType mCapacity;
    sizeType mLength;
    T* mData;

    void doubleIfFull()
    {
        if (mCapacity == mLength)
        {
            reserve((mCapacity) ? (mCapacity << 1) : 1);
        }
    }

  public:

    iteratorType begin() { return mData; }
    iteratorType end() { return mData + mLength; }
    constIteratorType begin() const { return mData; }
    constIteratorType end() const { return mData + mLength; }

    Array () : mCapacity(0), mLength(0), mData(nullptr) {}

    explicit Array (sizeType size) : mCapacity(size), mLength(0), mData((size) ? static_cast<T*>(::operator new(sizeof(T) * size)) : nullptr)
    {
        for (; mLength < size; ++mLength)
        {
            new (mData + mLength) T();
        }
    }

    explicit Array (sizeType size, const T& value) : mCapacity(size), mLength(0), mData((size) ? static_cast<T*>(::operator new(sizeof(T) * size)) : nullptr)
    {
        for (; mLength < size; ++mLength)
        {
            new (mData + mLength) T(value);
        }
    }

    Array (const Array& other) : Array()
    {
        reserve(other.mCapacity);

        for (; mLength < other.mLength; ++mLength)
        {
            new (mData + mLength) T(other[mLength]);
        }
    }

    Array (Array&& other) : Array()
    {
        swap(other);
    }

    ~Array ()
    {
        for (sizeType i = 0; i < mLength; ++i)
        {
            mData[i].~T();
        }

        if (mData) ::operator delete (mData);
    }

    void reserve (sizeType newCapacity)
    {
        if (newCapacity > mCapacity)
        {
            T* newData = static_cast<T*>(::operator new(sizeof(T) * newCapacity));

            for (sizeType i = 0; i < mLength; ++i)
            {
                new (newData + i) T(std::move(mData[i]));
                mData[i].~T();
            }

            if (mData) ::operator delete (mData);
            mData = newData;
            mCapacity = newCapacity;
        }
    }

    void resize (sizeType newSize)
    {
        if (newSize > mLength)
        {
            reserve(newSize);
            while (mLength < newSize)
            {
                new (mData + mLength) T();
                ++mLength;
            }
        }
        else if (newSize < mLength)
        {
            while (mLength > newSize)
            {
                --mLength;
                mData[mLength].~T();
            }
        }
    }

    void swap (Array& other)
    {
        std::swap(mCapacity, other.mCapacity);
        std::swap(mLength, other.mLength);
        std::swap(mData, other.mData);
    }

    Array& operator= (Array other)
    {
        swap(other);

        return *this;
    }

    void pushBack (const T& elem)
    {
        doubleIfFull();

        new (mData + mLength) T(elem);
        ++mLength;
    }

    void pushBack (T&& elem)
    {
        doubleIfFull();

        new (mData + mLength) T(std::move(elem));
        ++mLength;
    }

    template<typename ...Args>
    void emplaceBack (Args && ...args)
    {
        doubleIfFull();

        new (mData + mLength) T(std::forward<Args>(args)...);
        ++mLength;
    }

    bool contains (const T& elem) const
    {
        for (const T& item : *this)
        {
            if (item == elem) return true;
        }

        return false;
    }

    bool isEmpty ()
    {
        return (mLength == 0);
    }

    void popBack ()
    {
        if (isEmpty()) throw std::out_of_range( "Index out of range." );

        mLength--;

        mData[mLength].~T();
    }

    T& operator[] (const sizeType index)
    {
        return mData[index];
    }

    const T& operator[] (const sizeType index) const
    {
        return mData[index];
    }

    T& at (const sizeType index)
    {
        if (index > mLength) throw std::out_of_range( "Index out of range." );

        return mData[index];
    }

    const T& at (const sizeType index) const
    {
        if (index > mLength) throw std::out_of_range( "Index out of range." );

        return mData[index];
    }

    void remove (const sizeType index)
    {
        if (index > mLength) throw std::out_of_range( "Index out of range." );

        --mLength;

        for (sizeType i = index; i < mLength; ++i)
        {
            mData[i] = std::move(mData[i + 1]);
        }

        mData[mLength].~T();
    }

    void remove (constIteratorType it)
    {
        remove(sizeType(it - mData));
    }

    sizeType getLength () const
    {
        return mLength;
    }

    sizeType getCapacity() const
    {
        return mCapacity;
    }

    template <typename U>
    friend std::ostream& operator<< (std::ostream &os, const Array<U>& arr);
};

template <typename T>
std::ostream& operator<< (std::ostream &os, const Array<T>& arr)
{
    os << '[';

    bool first = true;

    for (const T& x : arr)
    {
        if (!first) os << ", "; else first = false;
        os << x;
    }

    return os << ']';
}

#endif
