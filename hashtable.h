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
        uint index = countIndex(key);

        if (_size == _allocatedSize)
            expand();

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

    uint contains(const Tkey &key) const
    {
        uint index = countIndex(key);

        while (_buckets[index])
        {
            if (_buckets[index]->_key != key && index != _allocatedSize - 1)
                index++;
            else if (_buckets[index]->_key != key && index == _allocatedSize - 1)
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
            _buckets[index] = nullptr;

            _size--;

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
        compress();
    }

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

    const Tvalue& operator[](const Tkey& key) const
    {
        uint index = contains(key);

        if (index)
            return _buckets[index]->_value;

        throw std::exception();
    }

    const Tvalue& getValue(const Tkey& key) const
    {
        uint index = contains(key);

        if (index)
            return _buckets[index]->_value;

        throw std::exception();
    }

    bool operator==(const HashTable& otherTable) const
    {
        if (_size == otherTable._size)
        {
            for (uint i = 0; i < _allocatedSize; i++)
                if (_buckets[i])
                    if (_buckets[i]->_key != otherTable._buckets[i]->_key)
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
                if (_buckets[i]->_key != otherTable._buckets[i]->_key)
                    return true;

            return false;
        }
        return true;
    }

    int getSize() const { return _size; }

    bool isEmpty() const { return _size == 0; }

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


    Iterator* begin() const
    {
        Iterator *iter;
        if (!isEmpty())
        {
            for (auto i = 0; i < _allocatedSize; i++)
                if (_buckets[i])
                {
                    iter = new Iterator(_buckets[i]);
                    break;
                }
        }
        else
            iter = new Iterator();
        return iter;
    }

    Iterator* end() const
    {
        Iterator *iter;
        if (!isEmpty())
        {
            for (auto i = _allocatedSize - 1; i > 0; i--)
                if (_buckets[i])
                {
                    iter = new Iterator(_buckets[i]);
                    break;
                }
        }
        else
            iter = new Iterator();
        return iter;
    }

private:
    uint _size;
    uint _allocatedSize;
    Pair **_buckets;

    void expand()
    {
        Pair **tmp = new Pair*[_allocatedSize + 32];

        for (uint i = 0; i < _size; i++)
            tmp[i] = _buckets[i];

        delete[] _buckets;
        _buckets = tmp;
    }

    void compress()
    {
        if (_size + 32 < _allocatedSize)
        {
            Pair **tmp = new Pair*[_allocatedSize];

            for (uint i = 0; i < _allocatedSize; i++)
                if (_buckets[i])
                {
                    tmp[i] = _buckets[i];
                    delete _buckets[i];
                }

            delete[] _buckets;

            _buckets = new Pair*[_allocatedSize - 32];

            for (auto i = 0; i < _allocatedSize; i++)
                if (tmp[i]->_key)
                    add(tmp[i]->_key, tmp[i]->_value);
        }
    }

//    unsigned int hash(Tkey key) const {}

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

    typedef ::Pair<Tkey, Tvalue> Pair;
    typedef unsigned int uint;

    Iterator() { _currPair = nullptr; }

    Iterator(Pair* pair) { _currPair = pair; }

    Iterator(const Iterator& otherIter) { _currPair = otherIter._currPair; }


    Iterator &operator++()
    {
        // TODO
        uint index = this->countIndex(_currPair);

        while (this->_buckets[index])
        {
            if (this->_buckets[index]->_key != _currPair && index != this->_allocatedSize - 1)
                index++;
            else if (this->_buckets[index]->_key != _currPair && index == this->_allocatedSize - 1)
                index = 0;
            else
                return this->_buckets[index++];
        }
    }

    Iterator operator++(int)
    {
        // TODO
    }

    Iterator &operator--()
    {
        // TODO
    }

    Iterator operator--(int)
    {
        // TODO
    }

    Iterator &operator=(const Iterator& otherIter)
    {
        if (this == &otherIter)
            return *this;
        _currPair = otherIter._currPair;
        return *this;
    }

    Tkey currentKey() { return _currPair->_key; }

    Tvalue currentValue() { return _currPair->_value; }

private:
    Pair* _currPair;
};

#endif // HASHTABLE_H
