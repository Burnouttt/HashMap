#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <typeinfo>

template<typename Tkey, typename Tvalue>
class Pair;

template<typename Tkey, typename Tvalue>
class Iterator;

template<typename Tkey, typename Tvalue>
class HashTable
{
public:
    friend class Pair<Tkey, Tvalue>;
    friend class Iterator<Tkey, Tvalue>;

    typedef Pair<Tkey, Tvalue> Pair;
    typedef Iterator<Tkey, Tvalue> Iterator;
    typedef unsigned int uint;

    HashTable()
    {
        _size = 0;
        _allocatedSize = 32;
        _buckets = new Pair*[32];
    }

    HashTable(const HashTable& other)
    {
        _size = other._size;
        _allocatedSize = other._allocatedSize;
        _buckets = new Pair*[_allocatedSize];

        for (auto i = 0; i < other._allocatedSize; i++)
            if (other._buckets[i])
                _buckets[i] = other._buckets[i];
    }

    ~HashTable()
    {
        for (auto i = 0; i < _allocatedSize; i++)
            if (_buckets[i])
                delete _buckets[i];

        delete[] _buckets;
    }


    void add(const Tkey &key, const Tvalue &value)
    {
        uint index = countIndex(key);

        if (_size == _allocatedSize)
            expand();

        while (_buckets[index])
            if (index < _allocatedSize - 1)
                index++;
            else
                index = 0;

        Pair *pair = new Pair(key, value);
        _buckets[index] = pair;

        _size++;
    }

    uint contains(const Tkey &key) const
    {
        uint index = countIndex(key);

        while (_buckets[index])
        {
            if (_buckets[index]._key != key && index != _allocatedSize - 1)
                index++;
            else if (_buckets[index]._key != key && index == _allocatedSize - 1)
                index = 0;
            else
                return index;
        }

        return 0;
    }

    bool remove(const Tkey &key)
    {
        uint index = contains(key);

        if (index)
        {
            delete _buckets[index];

            _size--;
        }
    }

    void clear()
    {
        for (auto i = 0; i < _allocatedSize; i++)
            if (_buckets[i])
                delete _buckets[i];

        delete[] _buckets;

        _size = 0;
        compress();
    }

    uint getSumValue() const
    {
        unsigned int sum = 0;

        if (typeid(Tvalue) == typeid(uint))
            for (auto i = 0; i < _allocatedSize; i++)
                if (_buckets[i])
                    sum += static_cast<uint>(_buckets[i]->_value); // check

        return sum;
    }

    const Tvalue& operator[](const Tkey& key) const
    {
        // TODO
    }

    bool operator==(const HashTable& otherTable) const
    {
        // TODO
    }

    bool operator!=(const HashTable& otherTable) const
    {
        // TODO
    }

    int getSize() const { return _size; }

    bool isEmpty() const { return _size == 0; }

private:
    uint _size;
    uint _allocatedSize;
    Pair **_buckets;

    void expand()
    {
        Pair **tmp = new Pair*[_allocatedSize + 32];

        for (auto i = 0; i < _size; i++)
            tmp[i] = _buckets[i];

        delete[] _buckets;
        _buckets = tmp;
    }

    void compress()
    {
        if (_size + 32 < _allocatedSize)
        {
            Pair **tmp = new Pair*[_allocatedSize];

            for (auto i = 0; i < _allocatedSize; i++)
                if (_buckets[i])
                {
                    tmp[i] = _buckets[i];
                    delete _buckets[i];
                }

            delete[] _buckets;

            _buckets = new Pair[_allocatedSize - 32];

            for (auto i = 0; i < _allocatedSize; i++)
                if (tmp[i]._key)
                    add(tmp[i]._key, tmp[i]._value);
        }
    }

//    unsigned int hash(Tkey key) const {}

    uint hash(int key) const
    {
        uint result = 0;
        uint newKey = static_cast<uint>(key*key);

        while (newKey)
        {
            result = newKey % 100;
            newKey /= 100;
        }

        return result;
    }

    uint countIndex(Tkey key) const { return hash(key) % _allocatedSize; }
};


template<typename Tkey, typename Tvalue>
class Pair
{
public:
    friend class HashTable<Tkey, Tvalue>;
    friend class Iterator<Tkey, Tvalue>;


    Pair(Tkey key, Tvalue value)
    {
        if (key && value)
        {
            _key = key;
            _value = value;
        }
    }

private:
    Tkey _key;
    Tvalue _value;
};


template<typename Tkey, typename Tvalue>
class Iterator
{
public:
    friend class HashTable<Tkey, Tvalue>;
    friend class Pair<Tkey, Tvalue>;
    typedef HashTable<Tkey, Tvalue> HashTable;
    typedef Pair<Tkey, Tvalue> Pair;


    Iterator() { _currPair = nullptr; }

    Iterator(Pair& pair) { _currPair = pair; }


    Iterator &operator++()
    {

    }

    Iterator operator++(int)
    {

    }

    Iterator &operator--()
    {

    }

    Iterator operator--(int)
    {

    }

    Iterator &operator=(const Iterator& otherIter)
    {

    }

    // TODO

private:
    Pair* _currPair;
};

#endif // HASHTABLE_H
