#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <typeinfo>
#include <exception>
#include <iostream>
#include <fstream>
using namespace std;

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

    typedef ::Pair<Tkey, Tvalue> Pair;
    typedef ::Iterator<Tkey, Tvalue> Iterator;
    typedef unsigned int uint;

    HashTable()
    {
        _size = 0;
        _allocatedSize = 32;
        _buckets = new Pair*[32];
        for (uint i = 0; i < _allocatedSize; i++)
            _buckets[i] = nullptr;
    }

    HashTable(const HashTable& other)
    {
        _size = other._size;
        _allocatedSize = other._allocatedSize;
        _buckets = new Pair*[_allocatedSize];

        for (uint i = 0; i < other._allocatedSize; i++)
            if (other._buckets[i])
                _buckets[i] = new Pair(other._buckets[i]->_key, other._buckets[i]->_value);
            else
                _buckets[i] = nullptr;
    }

    ~HashTable()
    {
        for (uint i = 0; i < _allocatedSize; i++)
            if (_buckets[i])
                delete _buckets[i];

        delete[] _buckets;
    }


    void add(const Tkey &key, const Tvalue &value)
    {
        if (_size == _allocatedSize)
            expand();

        uint index = countIndex(key);

        while (_buckets[index])
        {
            if (_buckets[index]->_key == key)
                return;

            if (index < _allocatedSize - 1)
                index++;
            else
                index = 0;
        }

        Pair *pair = new Pair(key, value);
        _buckets[index] = pair;

        _size++;
    }

    int contains(const Tkey &key) const
    {
        uint index = countIndex(key);

        while (_buckets[index])
        {
            if (_buckets[index]->_key != key && index != _allocatedSize - 1)
                index++;
            else if (_buckets[index]->_key != key && index == _allocatedSize - 1)
                index = 0;
            else
                return static_cast<int>(index);
        }

        return -1;
    }

    bool remove(const Tkey &key)
    {
        int index = contains(key);

        if (index > -1)
        {
            delete _buckets[index];
            _buckets[index] = nullptr;

            _size--;
            compress();

            return true;
        }
        return false;
    }

    void clear()
    {
        for (uint i = 0; i < _allocatedSize; i++)
            if (_buckets[i])
            {
                delete _buckets[i];
                _buckets[i] = nullptr;
            }

        delete[] _buckets;

        _size = 0;

        while (_allocatedSize > 32)
            compress();
    }


    // for the task
    uint getSumValue() const
    {
        unsigned int sum = 0;
        unsigned int counter = 0;

        if (typeid(Tvalue) == typeid(uint))
            for (uint i = 0; i < _allocatedSize && counter < _size; i++)
                if (_buckets[i])
                {
                    sum += static_cast<uint>(_buckets[i]->_value);
                    counter++;
                }

        return sum;
    }

    // overloaded operators
    const Tvalue& operator[](const Tkey& key) const
    {
        int index = contains(key);

        if (index >= 0)
            return _buckets[index]->_value;

        throw std::exception();
    }

    bool operator==(const HashTable& otherTable) const
    {
        if (_size == otherTable._size)
        {
            for (uint i = 0; i < _allocatedSize; i++)
                if (_buckets[i])
                    if (_buckets[i]->_key != otherTable._buckets[i]->_key && _buckets[i]->_value != otherTable._buckets[i]->_value)
                        return false;

            return true;
        }
        return false;
    }

    bool operator!=(const HashTable& otherTable) const
    {
        if (_size == otherTable._size)
        {
            for (uint i = 0; i < _allocatedSize; i++)
                if (_buckets[i]->_key != otherTable._buckets[i]->_key && _buckets[i]->_value != otherTable._buckets[i]->_value)
                    return true;

            return false;
        }
        return true;
    }

    friend ofstream &operator<<(ofstream &ofs, const HashTable& table)
    {
        for(uint i = 0; i < table._allocatedSize; i++)
            if (table._buckets[i])
                ofs << table._buckets[i]->_key << ' ' << table._buckets[i]->_value << '\n';
        return ofs;
    }

    friend ifstream &operator>>(ifstream &ifs, HashTable& table)
    {
        table.clear();
        Tkey key;
        Tvalue value;
        while (!ifs.eof())
        {
            ifs >> key >> value;
            table.add(key, value);
        }
        return ifs;
    }


    // begin/end iterators
    Iterator begin() const
    {
        if (!isEmpty())
        {
            for (int i = 0; i < _allocatedSize; i++)
                if (_buckets[i])
                    return Iterator(_buckets[i], i, _buckets, _allocatedSize);
        }
        return Iterator();
    }

    Iterator end() const
    {
        if (!isEmpty())
        {
            for (int i = _allocatedSize - 1; i >= 0; i--)
                if (_buckets[i])
                    return Iterator(_buckets[i], i, _buckets, _allocatedSize);
        }
        return Iterator();
    }


    // size checkers
    int getSize() const { return _size; }

    bool isEmpty() const { return _size == 0; }

private:
    uint _size;
    uint _allocatedSize;
    Pair **_buckets;

    // array size control
    void expand()
    {
        Pair **tmp = new Pair*[_allocatedSize + 32];
        for (uint i = 0; i < _allocatedSize + 32; i++)
            tmp[i] = nullptr;

        for (uint i = 0; i < _allocatedSize; i++)
            if (_buckets[i])
                tmp[i] = new Pair(_buckets[i]->_key, _buckets[i]->_value);

        for (uint i = 0; i < _allocatedSize; i++)
            if (_buckets[i])
            {
                delete _buckets[i];
                _buckets[i] = nullptr;
            }
        delete[] _buckets;
        _size = 0;

        _allocatedSize += 32;

        _buckets = new Pair*[_allocatedSize];
        for (uint i = 0; i < _allocatedSize; i++)
            _buckets[i] = nullptr;

        for (uint i = 0; i < _allocatedSize - 32; i++)
            if (tmp[i])
                add(tmp[i]->_key, tmp[i]->_value);

        for (uint i = 0; i < _allocatedSize; i++)
            delete tmp[i];
        delete[] tmp;
    }

    void compress()
    {
        if (_size + 32 < _allocatedSize)
        {
            Pair **tmp = new Pair*[_allocatedSize];
            for (uint i = 0; i < _allocatedSize; i++)
                tmp[i] = nullptr;

            for (uint i = 0; i < _allocatedSize; i++)
                if (_buckets[i])
                {
                    tmp[i] = new Pair(_buckets[i]->_key, _buckets[i]->_value);
                    delete _buckets[i];
                    _buckets[i] = nullptr;
                }
            delete[] _buckets;
            _size = 0;

            _buckets = new Pair*[_allocatedSize - 32];
            for (uint i = 0; i < _allocatedSize - 32; i++)
                _buckets[i] = nullptr;

            for (uint i = 0; i < _allocatedSize; i++)
                if (tmp[i])
                    add(tmp[i]->_key, tmp[i]->_value);

            for (uint i = 0; i < _allocatedSize; i++)
                delete tmp[i];
            delete[] tmp;

            _allocatedSize -= 32;
        }
    }


    // hash functions & index counter
    uint hash(int key) const
    {
        uint result = 0;
        uint newKey = static_cast<uint>(key*key);

        while (newKey)
        {
            result += newKey % 100;
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
    friend ofstream &operator<<(ofstream &ofs, const HashTable<Tkey, Tvalue>& table);

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
    friend class Pair<Tkey, Tvalue>;
    friend class HashTable<Tkey, Tvalue>;

    typedef ::Pair<Tkey, Tvalue> Pair;
    typedef unsigned int uint;

    Iterator() { _currPair = nullptr; _index = -1; }

    Iterator(const Iterator& otherIter) { _currPair = otherIter._currPair; _index = otherIter._index; }

    Iterator &operator++()
    {
        if (_currPair)
        {
            int i;
            for (i = _index + 1; i < _allocatedSize && !_buckets[i]; i++);
            _currPair = _buckets[i];
            _index = i;
        }
        return *this;
    }

    Iterator operator++(int)
    {
        Iterator old(_currPair, _index, _buckets, _allocatedSize);
        if (_currPair)
        {
            int i;
            for (i = _index + 1; i < _allocatedSize && !_buckets[i]; i++);
            _currPair = _buckets[i];
            _index = i;
        }
        return old;
    }

    Iterator &operator--()
    {
        if (_currPair)
        {
            int i;
            for (i = _index - 1; i >= 0 && !_buckets[i]; i--);
            _currPair = _buckets[i];
            _index = i;
        }
        return *this;
    }

    Iterator operator--(int)
    {
        Iterator old(_currPair, _index, _buckets, _allocatedSize);
        if (_currPair)
        {
            int i;
            for (i = _index - 1; i >= 0 && !_buckets[i]; i--);
            _currPair = _buckets[i];
            _index = i;
        }
        return old;
    }

    Iterator &operator=(const Iterator& otherIter)
    {
        if (this == &otherIter)
            return *this;
        _currPair = otherIter._currPair;
        _index = otherIter._index;
        return *this;
    }

    bool operator==(const Iterator &otherIter) const { return _currPair == otherIter._currPair; }

    bool operator!=(const Iterator &otherIter) const { return _currPair != otherIter._currPair; }

    Tkey currentKey() { return _currPair->_key; }

    Tvalue currentValue() { return _currPair->_value; }

private:
    Pair* _currPair;
    int _index;
    Pair** _buckets;
    int _allocatedSize;

    Iterator(Pair* pair, int index, Pair** buckets, int allocatedSize)
    {
        _currPair = pair;
        _index = index;
        _buckets = buckets;
        _allocatedSize = allocatedSize;
    }
};

#endif // HASHTABLE_H
