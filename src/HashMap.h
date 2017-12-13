#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iostream>


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
            _hashTable = new HashNode*[_tableSize+1]{nullptr};  //begin() i end() beda wskazywaly na ostatnia komorke tabeli
            _size = 0;
        }

        HashMap(std::initializer_list<value_type> list) {
            (void) list; // disables "unused argument" warning, can be removed when method is implemented.
            throw std::runtime_error("TODO");
        }

        HashMap(const HashMap &other) {
            (void) other;
            throw std::runtime_error("TODO");
        }

        HashMap(HashMap &&other) {
            (void) other;
            throw std::runtime_error("TODO");
        }

        HashMap &operator=(const HashMap &other) {
            (void) other;
            throw std::runtime_error("TODO");
        }

        HashMap &operator=(HashMap &&other) {
            (void) other;
            throw std::runtime_error("TODO");
        }

        bool isEmpty() const {
            return (_size==0);
        }

        mapped_type &operator[](const key_type &key) {
            /*(void) key;
            throw std::runtime_error("TODO");*/
            //size_type hashKey = hashFunction(key);
            HashNode *node= nullptr;
            if(_size == 0){
                node = new HashNode(key, mapped_type{});
                _hashTable[key]=node;
                ++_size;
            }
            return node->nodeElement.second;
        }

        const mapped_type &valueOf(const key_type &key) const {
            (void) key;
            throw std::runtime_error("TODO");
        }

        mapped_type &valueOf(const key_type &key) {
            (void) key;
            throw std::runtime_error("TODO");
        }

        const_iterator find(const key_type &key) const {
            (void) key;
            throw std::runtime_error("TODO");
        }

        iterator find(const key_type &key) {
            (void) key;
            throw std::runtime_error("TODO");
        }

        void remove(const key_type &key) {
            (void) key;
            throw std::runtime_error("TODO");
        }

        void remove(const const_iterator &it) {
            (void) it;
            throw std::runtime_error("TODO");
        }

        size_type getSize() const {
            return _size;
        }

        bool operator==(const HashMap &other) const {
            (void) other;
            throw std::runtime_error("TODO");
        }

        bool operator!=(const HashMap &other) const {
            return !(*this == other);
        }

        iterator begin() {
            //throw std::runtime_error("TODO");
            //iterator it(_hashTable);
            iterator it;
            if(_size==0) {
                it.ptr = _hashTable[_tableSize];
                it.index = _tableSize;
            }
            else{
                unsigned long i = 0;
                while(i<_tableSize && _hashTable[i] == nullptr){
                    ++i;
                }
                it.ptr = _hashTable[i];
                it.index=i;
            }
            return it;
        }

        iterator end() {
            //throw std::runtime_error("TODO");
            //iterator it(_hashTable);
            iterator it;
            it.ptr = _hashTable[_tableSize];
            it.index = _tableSize;
            return it;
        }

        const_iterator cbegin() const {
            //throw std::runtime_error("TODO");
            const_iterator it(_hashTable);
            if(_size==0) {
                it.ptr = _hashTable[_tableSize];
                it.index = _tableSize;
            }
            else{
                unsigned long i = 0;
                while(i<_tableSize && _hashTable[i]== nullptr){
                    ++i;
                }
                it.ptr = _hashTable[i];
                it.index=i;
            }
            return it;
        }

        const_iterator cend() const {
            //throw std::runtime_error("TODO");
            const_iterator it(_hashTable);
            it.ptr = _hashTable[_tableSize];
            it.index=_tableSize;
            return it;
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }
    };

    template<typename KeyType, typename ValueType>
    class HashMap<KeyType, ValueType>::ConstIterator {
    public:
        using reference = typename HashMap::const_reference;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename HashMap::value_type;
        using pointer = const typename HashMap::value_type *;

        HashNode *ptr;
        size_type index;
        HashNode **table;

        explicit ConstIterator() {
            ptr = nullptr;
            index=0;
        }
        ConstIterator(HashNode **hashTable):ConstIterator(){
            table = hashTable;

        }

        ConstIterator(const ConstIterator &other) {
            ptr = other.ptr;
            index= other.index;
        }

        ConstIterator &operator++() {
            //throw std::runtime_error("TODO");
            if(index == _tableSize)throw std::out_of_range("Can't increment end()");
            if(ptr->next != nullptr)
                ptr = ptr->next;
            else{
                while(index <= _tableSize && table[index]!=nullptr){
                    ++index;
                }
                ptr = table[index];
            }
            return *this;
        }

        ConstIterator operator++(int) {
            throw std::runtime_error("TODO");
        }

        ConstIterator &operator--() {
            throw std::runtime_error("TODO");
        }

        ConstIterator operator--(int) {
            throw std::runtime_error("TODO");
        }

        reference operator*() const {
            //throw std::runtime_error("TODO");
            return ptr->nodeElement;
        }

        pointer operator->() const {
            return &this->operator*();
        }

        bool operator==(const ConstIterator &other) const {
            /*(void) other;
            throw std::runtime_error("TODO");*/
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

        explicit Iterator() {}

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
