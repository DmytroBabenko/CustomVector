//
// Created by Dmytro Babenko on 2018-12-18.
//

#ifndef CUSTOMVECTOR_MY_VECTOR_H
#define CUSTOMVECTOR_MY_VECTOR_H


// #include <mach/mach_types.h> // <-- це якийсь маківський нестандартний хідер..
#include <cstdint>

template <typename T>
class my_vector
{
private:
    static const size_t MAX_SIZE = 1000000000;
public:
    typedef T* iterator;
    typedef const T* const_iterator;

public:
    my_vector();
    explicit my_vector(size_t size);
    explicit my_vector(size_t size, const T& value);
    my_vector(std::initializer_list<T> init);

    my_vector(const my_vector<T>& iOther);
    my_vector(my_vector<T>&& iOther) noexcept;


    my_vector& operator=(const my_vector<T>& iOther);
    my_vector&operator=(my_vector<T>&& iOther)noexcept;

    ~my_vector();

public:
    bool is_empty() const;
    size_t size() const;
    size_t capacity() const;
    void clear();
    void resize(size_t new_size);
    size_t max_size() const;

    void reserve(size_t new_capacity);

    T& at(size_t index);
    const T& at(size_t index) const;

    T& front();
    const T& front() const;

    T& back();
    const T& back() const;


    iterator begin() const; // const тут сумнівний, безперечно! Але без нього GTest не компілював ряд тестів.
    const_iterator cbegin() const;

    iterator end() const; // const тут сумнівний, безперечно! Але без нього GTest не компілював ряд тестів.
    const_iterator cend()const ;

    void push_back(const T& value);
    template <class... Args>
    void emplace_back(Args&&... args);

    void pop_back();
    void shrink_to_fit();

    iterator insert(iterator pos, const T& value);
    template <typename InputIt>
    iterator insert(iterator pos, InputIt first, InputIt last);

    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);

    void swap(my_vector<T>& iOther);

public:
    bool operator==(const my_vector& other) const;
    bool operator!=(const my_vector& other) const;
    bool operator<(const my_vector& other) const;
    bool operator<=(const my_vector& other) const;
    bool operator>(const my_vector& other) const;
    bool operator>=(const my_vector& other) const;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

private:
    void reallocate(size_t new_capacity);

private:
    T* mData;
    size_t mSize;
    size_t mCapacity;
};

//! Таке рішення -- що порожній вектор містить нульовий вказівник замість блоку даних,
//! часто -- субоптимальне. Воно змушує завжди перевіряти, а чи цей вказівник не нульовий.
//! Виграш є при переміщенні, але програє решта коду.
template <typename T>
my_vector<T>::my_vector()
:mSize(0)
,mCapacity(0)
,mData(nullptr)
{
}

template <typename T>
my_vector<T>::my_vector(size_t size)
: mSize(size)
, mCapacity(size)
, mData(nullptr)
{
    mData = new T[mCapacity]();
}

template <typename T>
my_vector<T>::my_vector(size_t size, const T& value)
: mSize(size)
, mCapacity(size)
, mData(nullptr)
{
    mData = new T[mCapacity];
    for (size_t i = 0; i < size; ++i)
    {
        mData[i] = value;
    }
}



template <typename T>
my_vector<T>::my_vector(std::initializer_list<T> init)
: mSize(init.size())
, mCapacity(init.size())
{
    mData = new T[mCapacity];

    size_t  i = 0;
    for (const T& val : init)
    {
        mData[i++] = val;
    }
}

template <typename T>
my_vector<T>::my_vector(const my_vector &iOther)
: mSize(iOther.mSize)
, mCapacity(iOther.mCapacity)
{
    mData = new T[mCapacity];

    for (size_t i = 0; i < mSize; ++i)
        mData[i] = iOther.mData[i];
}

template <typename T>
my_vector<T>::my_vector(my_vector &&iOther) noexcept
: mSize(iOther.mSize)
, mCapacity(iOther.mCapacity)
{
    mData = iOther.mData;

    iOther.mData = nullptr;
    iOther.mSize = 0;
    iOther.mCapacity = 0;
}

template <typename T>
my_vector<T>& my_vector<T>::operator=(const my_vector<T> &iOther)
{
//! Немає захисту від присвоєння самому собі!
    clear();

    mSize = iOther.mSize;
    mCapacity = iOther.mCapacity;

    mData = new T[mCapacity];
    for (size_t i = 0; i < mSize; ++i)
    {
        mData[i] = iOther.mData[i];
    }
    return *this;
}

template <typename T>
my_vector<T>& my_vector<T>::operator=(my_vector<T> &&iOther) noexcept
{
    clear();

    mSize = iOther.mSize;
    mCapacity = iOther.mCapacity;
    mData = iOther.mData;

    iOther.mSize = iOther.mCapacity = 0;
    iOther.mData = nullptr;

    return *this;
}


template <typename T>
my_vector<T>::~my_vector()
{
    delete[] mData;
}

//methods

template <typename T>
bool my_vector<T>::is_empty() const
{
    return mSize == 0;
}

template <typename T>
size_t my_vector<T>::size() const
{
    return mSize;
}

template <typename T>
size_t my_vector<T>::max_size() const
{
    return MAX_SIZE;
}

template <typename T>
size_t my_vector<T>::capacity() const
{
    return mCapacity;
}

template <typename T>
void my_vector<T>::reserve(size_t new_capacity)
{
    if (new_capacity <= capacity())
        return;

    if (new_capacity > max_size())
        throw std::length_error("max size exceeds");

    reallocate(new_capacity);
}

template <typename T>
void my_vector<T>::clear()
{
    delete [] mData;
    mSize = mCapacity = 0;
    mData = nullptr; // Раз вже Ви вважаєте, що порожній об'єкт не має блоку пам'яті.
}

template <typename T>
void my_vector<T>::resize(size_t new_size)
{
    if (mSize == new_size)
        return;

    if (mSize < new_size)
    {
        if (mCapacity < new_size)
        {
            reserve(new_size);
        }
    }
    else
    {
        for (size_t i = new_size; i < mSize; ++i)
        {
            mData[i] = T(); // Хитро Ви викрутилися із знищенням об'єктів. :=)
        }
    }
    mSize = new_size;
}

template <typename T>
T& my_vector<T>::at(size_t index)
{
    if (index >= size())
        throw std::out_of_range("index is out of range");

    return operator[](index);
}

template <typename T>
const T& my_vector<T>::at(size_t index) const
{
    if (index >= size())
        throw std::out_of_range("index is out of range");

    return operator[](index);
}

template <typename T>
void my_vector<T>::push_back(const T &value)
{
    if (mSize < capacity()) {
        mData[mSize] = value;
        ++mSize;
        return;
    }


    T copyValue = value;

    reserve(2 * capacity() + 1);
    mData[mSize] = std::move(copyValue);
    ++mSize;

}

template<typename T>
template<class... Args>
void my_vector<T>::emplace_back(Args &&... args)
{
    if (mSize < capacity()) {
        mData[mSize] = std::move(T( std::forward<Args>(args) ... ));;
        ++mSize;
        return;
    }


    T copyValue = std::move(T( std::forward<Args>(args) ... ));;

    reserve(2 * capacity() + 1);
    mData[mSize] = std::move(copyValue);
    ++mSize;
}

template <typename T>
void my_vector<T>::pop_back()
{
    --mSize;

    //TODO: think about two times calling destructor
    //! Це все просто треба було по іншому робити. Створювати теж "вручну".
    // mData[mSize].~T();
    mData[mSize] = T(); // Вами ж запропонований ерзац
}

template <typename T>
void my_vector<T>::shrink_to_fit()
{
    if (mCapacity > mSize)
    {
        reallocate(mSize);
    }
}


template <typename T>
T & my_vector<T>::front()
{
    return *begin();
}

template <typename T>
const T & my_vector<T>::front() const
{
    return *cbegin();
}

template <typename T>
T & my_vector<T>::back()
{
    return mData[mSize - 1];
}

template <typename T>
const T & my_vector<T>::back() const
{
    return mData[mSize - 1];
}


template <typename T>
typename my_vector<T>::iterator my_vector<T>::insert(my_vector<T>::iterator pos, const T &value)
{
    size_t index_pos = pos - begin();

    if (size() == capacity())
    {
        reserve(2*capacity() + 1);
    }

    iterator iter_pos = begin() + index_pos;
    size_t move_size = size() - index_pos;

    for (size_t i = size();  i > index_pos; --i)
    {
        mData[i] = std::move(mData[i-1]);
    }

    *iter_pos = value;
    ++mSize;

    return iter_pos;
}

template<typename T>
template<typename InputIt>
typename my_vector<T>::iterator my_vector<T>::insert(my_vector<T>::iterator pos, InputIt first, InputIt last)
{
    size_t index_pos = pos - begin();
    size_t insert_size = last - first;

    if (insert_size == 0)
        return pos;

    if (size() + insert_size > capacity())
    {
        reserve(2 * (size() + insert_size) + 1);
    }

    iterator iter_pos = begin() + index_pos;

    for (size_t i = size() + insert_size - 1; i >= index_pos + insert_size; --i)
    {
        mData[i] = std::move(mData[i - insert_size]);
    }

    for (auto iter = iter_pos, input_iter = first; input_iter != last; ++iter, ++input_iter)
    {
        *iter = *input_iter;
    }

    mSize += insert_size;
    return iter_pos;
}




template<typename T>
typename my_vector<T>::iterator my_vector<T>::erase(iterator pos)
{
    size_t index_pos = pos - begin();

    for (size_t i = index_pos; i < size() - 1; ++i)
    {
        mData[i] = std::move(mData[i + 1]);
    }
    --mSize;

    return pos;
}


template<typename T>
typename my_vector<T>::iterator my_vector<T>::erase(iterator first, iterator last)
{
    size_t erase_size = last - first;
    if (erase_size == 0)
        return last;

    size_t first_pos = first - begin();

    for (size_t i = first_pos; i < size() - erase_size; ++i)
    {
        mData[i] = std::move(mData[i + erase_size]);
    }

    for (size_t i = size() - erase_size; i < size(); ++i)
    {
        //mData[i].~T(); // Ви зразу породжуєте проблему -- решта коду може вважати, що об'єкт існує.
        mData[i] = T();
    }


    mSize -= erase_size;

    return begin() + first_pos;
}


template <typename T>
void my_vector<T>::swap(my_vector<T> &iOther)
{
    size_t tmpSize = mSize;
    size_t tmpCapacity = mCapacity;
    T* tmpData = mData;

    mSize = iOther.mSize;
    mCapacity = iOther.mCapacity;
    mData = iOther.mData;


    iOther.mSize = tmpSize;
    iOther.mCapacity = tmpCapacity;
    iOther.mData = tmpData;
}

template <typename T>
typename my_vector<T>::iterator my_vector<T>::begin() const
{
    return mData;
}

template <typename T>
typename my_vector<T>::const_iterator my_vector<T>::cbegin() const
{
    return mData;
}

template <typename T>
typename my_vector<T>::iterator my_vector<T>::end() const
{
    return mData + mSize;
}

template <typename T>
typename my_vector<T>::const_iterator my_vector<T>::cend() const
{
    return mData + mSize;
}

//operators overload
template <typename T>
bool my_vector<T>::operator==(const my_vector<T> & other) const
{
    if (mSize != other.mSize)
        return false;

    for (size_t i = 0; i < mSize; ++i)
    {
        if (mData[i] != other.mData[i])
            return false;
    }

    return true;
}

template <typename T>
bool my_vector<T>::operator!=(const my_vector<T> &other) const
{
    return !operator==(other);
}

template <typename T>
T& my_vector<T>::operator[](size_t index)
{
    return mData[index];
}

template <typename T>
const T& my_vector<T>::operator[](size_t index) const
{
    return mData[index];
}


template<typename T>
bool my_vector<T>::operator<(const my_vector &other) const
{
    size_t min_size = std::min(size(), other.size());

    for (size_t i = 0; i < min_size; ++i)
    {
        if (mData[i] != other.mData[i])
            return mData[i] < other.mData[i];
    }
    return size() < other.size();
}

template<typename T>
bool my_vector<T>::operator<=(const my_vector &other) const
{
    size_t min_size = std::min(size(), other.size());

    for (size_t i = 0; i < min_size; ++i)
    {
        if (mData[i] != other.mData[i])
            return mData[i] < other.mData[i];
    }
    return size() <= other.size();}

template<typename T>
bool my_vector<T>::operator>(const my_vector &other) const
{
    size_t min_size = std::min(size(), other.size());

    for (size_t i = 0; i < min_size; ++i)
    {
        if (mData[i] != other.mData[i])
            return mData[i] > other.mData[i];
    }
    return size() > other.size();
}

template<typename T>
bool my_vector<T>::operator>=(const my_vector &other) const
{
    size_t min_size = std::min(size(), other.size());

    for (size_t i = 0; i < min_size; ++i)
    {
        if (mData[i] != other.mData[i])
            return mData[i] > other.mData[i];
    }
    return size() >= other.size();
}



//implementation private methods
template <typename T>
void my_vector<T>::reallocate(size_t new_capacity)
{
    mCapacity = new_capacity;

    T* newData = new T[mCapacity];
    for (size_t i = 0; i < mSize; ++i)
    {
        newData[i] = mData[i];
    }
    //memcpy(newData, mData, mSize* sizeof(T));
    delete []mData;

    mData = newData;

}


#endif //CUSTOMVECTOR_MY_VECTOR_H
