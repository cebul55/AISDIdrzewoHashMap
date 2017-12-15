#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iostream>
#include <boost/concept_check.hpp>


namespace aisdi {

    template<typename KeyType, typename ValueType>
    class HashMap {
    public:
        using key_type = KeyType;
        using mapped_type = ValueType;
        using value_type = std::pair<const key_type, mapped_type>;
        using size_type = std::size_t;
        using reference = value_type &;
        using const_reference = const value_type &;

        class ConstIterator;
        class Iterator;

        using iterator = Iterator;
        using const_iterator = ConstIterator;

    private:
        static const size_type _tableSize = 64081;
    struct HashNode
    {
        value_type nodeElement;
        HashNode *prev, *next;

        HashNode(){
            prev =nullptr;
            next = nullptr;
        }

        HashNode(const key_type key, mapped_type val):
            nodeElement(std::make_pair(key,val)),
            prev(nullptr),
            next(nullptr)
        {};

        ~HashNode(){
            //(&nodeElement)->~value_type();
        }
    };
    public:
        HashNode **_hashTable;
        size_type _size;

    public:

        HashMap() {
            _hashTable = new HashNode* [_tableSize+1]{nullptr};  //begin() i end() beda wskazywaly na ostatnia komorke tabeli
            _size = 0;
        }

        HashMap(std::initializer_list<value_type> list): HashMap()
        {
            for(auto& a: list)valueOf(a.first)=a.second;
        }

        HashMap(const HashMap &other):HashMap() {
            for (auto& a : other) valueOf(a.first) = a.second;
        }

        HashMap(HashMap &&other) {
            _hashTable = other._hashTable;
            _size = other._size;
            other._hashTable = nullptr;
            other._size=0;
        }

        HashMap &operator=(const HashMap &other) {
            /*(void) other;
            throw std::runtime_error("TODO");*/
            if(*this == other)return *this;
            //if(_size!=0)
                _clear();
            _size=0;
            for(auto& a: other)valueOf(a.first) = a.second;
            return *this;
        }

        HashMap &operator=(HashMap &&other) {
            //_clear();
            //if(_size!=0)
                _clear();
            delete[] _hashTable;
            _hashTable = other._hashTable;
            _size = other._size;
            other._hashTable = nullptr;
            other._size = 0;
            return *this;
        }
        ~HashMap(){
            //if(_size!=0)
                _clear();
            delete[] _hashTable;
        }

        bool isEmpty() const {
            return (_size==0);
        }

        mapped_type &operator[](const key_type &key) {
            return valueOf(key);
        }

        const mapped_type &valueOf(const key_type &key) const {
            /*(void) key;
            throw std::runtime_error("TODO");*/
            const_iterator it = find(key);
            if(it == cend())throw std::out_of_range("no such key");

            return it->second;
        }

        mapped_type &valueOf(const key_type &key) {
            /*(void) key;
            throw std::runtime_error("TODO");*/
            size_type hashKey = stupidHashFunction(key);
            if(_size == 0){
                HashNode *node = new HashNode(key,mapped_type{});
                _hashTable[hashKey]=node;
                ++_size;
                return node->nodeElement.second;
            }
            else{
                if(_hashTable[hashKey]== nullptr){
                    HashNode *node = new HashNode(key,mapped_type{});
                    _hashTable[hashKey]=node;
                    ++_size;
                    return node->nodeElement.second;
                }
                else{
                    HashNode *tmp = _hashTable[hashKey];
                    while(tmp != nullptr ){
                        if(tmp->nodeElement.first == key) {
                            //if (tmp->nodeElement.second != mapped_type{})
                                //tmp->nodeElement.second = mapped_type{};
                            return tmp->nodeElement.second;
                        }
                        else {
                            if (tmp->next != nullptr)
                                tmp = tmp->next;
                            else break;
                        }

                    }
                    /*if (tmp->nodeElement.second != mapped_type{}) {
                        tmp->nodeElement.second = mapped_type{};
                        return tmp->nodeElement.second;
                    }
                    else {*/
                        HashNode *node = new HashNode(key, mapped_type{});
                        tmp->next = node;
                        node->prev = tmp;
                        node->next = nullptr;
                        ++_size;
                        return node->nodeElement.second;
                    //}
                }
            }
        }

        const_iterator _find(const key_type& key) const
        {
            size_type hashKey = stupidHashFunction(key);
            if(_hashTable[hashKey]!= nullptr){
                HashNode *tmp = _hashTable[hashKey];
                while(tmp!= nullptr){
                    if(tmp->nodeElement.first==key)
                        return const_iterator(this,tmp,hashKey);
                    tmp=tmp->next;
                }
            }
            return const_iterator (this);
        }

        const_iterator find(const key_type &key) const {
            return _find(key);
        }

        iterator find(const key_type &key) {
            const_iterator it = _find(key);
            return iterator(it);
        }

        void remove(const key_type &key) {
            return remove(find(key));
        }

        void remove(const const_iterator &it) {
            if(it == cend())throw std::out_of_range("No such node");

            HashNode *tmp = it.ptr;
            if(tmp->next == nullptr && tmp->prev == nullptr){
                _hashTable[it.index] = nullptr;
                delete tmp;
                --_size;
            }
            else if(tmp->prev== nullptr){
                _hashTable[it.index] = tmp->next;
                tmp->next->prev= nullptr;
                tmp->next = nullptr;
                delete tmp;
                --_size;
            }
            else if(tmp->next == nullptr){
                tmp->prev->next= nullptr;
                tmp->prev= nullptr;
                delete tmp;
                --_size;
            }
            else{
                HashNode *prevN = tmp->prev;
                HashNode *nextN = tmp->next;
                prevN->next=tmp->next;
                nextN->prev=tmp->prev;
                tmp->prev = tmp->next = nullptr;
                delete tmp;
                --_size;
            }
        }

        void _clear(){
            if (_size == 0) return;
            size_type index = 0;
            HashNode *tmp, *deleted;
            while(index < _tableSize){
                if(_hashTable[index]!= nullptr){
                    tmp = _hashTable[index];
                    _hashTable[index]= nullptr;
                    while(tmp!= nullptr){
                        deleted = tmp;
                        tmp= tmp->next;

                        if(tmp != nullptr)
                            tmp->prev = nullptr;
                        deleted->next= nullptr;
                        delete deleted;
                        --_size;
                    }
                }
                ++index;
            }
        }

        size_type getSize() const {
            return _size;
        }

        bool operator==(const HashMap &other) const {
            if(_size != other._size)return false;
            ConstIterator a = cbegin();
            ConstIterator b = other.cbegin();
            while(a != cend() && b != other.cend()){
                if(*a++ != *b++)return false;
            }

            return true;
        }

        bool operator!=(const HashMap &other) const {
            return !(*this == other);
        }

        iterator begin() {
            if(_size==0) {
                return iterator(this);
            }
            else{
                unsigned long i = 0;
                while(i<_tableSize && _hashTable[i] == nullptr){
                    ++i;
                }
                return iterator(this,_hashTable[i],i);
            }
        }

        iterator end() {
            iterator it(this,_hashTable[_tableSize],_tableSize);
            return it;
        }

        const_iterator cbegin() const {
            if(_size==0) {
                return const_iterator(this);
            }
            else{
                unsigned long i = 0;
                while(i<_tableSize && _hashTable[i]== nullptr){
                    ++i;
                }
                return const_iterator (this,_hashTable[i],i);
            }
        }

        const_iterator cend() const {
            const_iterator it(this,_hashTable[_tableSize],_tableSize);
            return it;
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }

        size_type stupidHashFunction(const key_type& key) const
        {
            return key%_tableSize;
        }
    };

    template<typename KeyType, typename ValueType>
    class HashMap<KeyType, ValueType>::ConstIterator {
    public:
        using reference = typename HashMap::const_reference;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename HashMap::value_type;
        using pointer = const typename HashMap::value_type *;

        const HashMap *myMap;
        HashNode *ptr;
        size_type index;


        explicit ConstIterator(const HashMap *table= nullptr , HashNode *ptr = nullptr, size_type index = 0):
        myMap(table),
        ptr(ptr),
        index(index)
        {
            if(table!= nullptr && ptr == nullptr)
                this->index = table->_tableSize;
        };

        ConstIterator(const ConstIterator &other):ConstIterator(other.myMap, other.ptr, other.index)
        {}

        ConstIterator &operator++() {
            //throw std::runtime_error("TODO");
            if(index == _tableSize)throw std::out_of_range("Can't increment end()");
            if(ptr->next != nullptr)
                ptr = ptr->next;
            else{
                ++index;
                while(index < myMap->_tableSize && myMap->_hashTable[index] == nullptr){
                    ++index;
                }
                if(index >= myMap->_tableSize)
                    ptr = nullptr;
                else
                    ptr = myMap->_hashTable[index];
            }
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        ConstIterator &operator--() {
            if(myMap == nullptr)throw std::out_of_range("can't decrement iterator");
            else if(ptr == nullptr || ptr == myMap->_hashTable[index]) {
                --index;
                while (index > 0 && myMap->_hashTable[index] == nullptr)
                    --index;
                if (index == myMap->_tableSize && myMap->_size == 0)
                    throw std::out_of_range("can't decrement iterator");

                if (index == 0 && myMap->_hashTable[index] == nullptr)
                    throw std::out_of_range("can't decrement iterator");

                ptr = myMap->_hashTable[index];
                while (ptr->next != nullptr)
                    ptr = ptr->next;
            }
            else{
                ptr = ptr->prev;
            }
            return *this;
        }

        ConstIterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        reference operator*() const {
            //throw std::runtime_error("TODO");
            if(index == myMap->_tableSize)throw std::out_of_range("can't dereference end()");
            return ptr->nodeElement;
        }

        pointer operator->() const {
            return &this->operator*();
        }

        bool operator==(const ConstIterator &other) const {
            return(ptr == other.ptr && index==other.index);
        }

        bool operator!=(const ConstIterator &other) const {
            return !(*this == other);
        }
    };

    template<typename KeyType, typename ValueType>
    class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator {
    public:
        using reference = typename HashMap::reference;
        using pointer = typename HashMap::value_type *;

        explicit Iterator(const HashMap *table= nullptr , HashNode *ptr = nullptr, size_type index = 0):ConstIterator(table,ptr,index)
        {}

        Iterator(const ConstIterator &other)
                : ConstIterator(other) {}

        Iterator &operator++() {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int) {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator &operator--() {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int) {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        pointer operator->() const {
            return &this->operator*();
        }

        reference operator*() const {
            // ugly cast, yet reduces code duplication.
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

}

#endif /* AISDI_MAPS_HASHMAP_H */
