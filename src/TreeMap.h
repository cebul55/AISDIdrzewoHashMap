#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iostream>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  class Node;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

           /*empty node , which left child is treeroot*/
  Node *_root;
  size_type _nodeCount;



  TreeMap()           /*creating empty tree with 2 Nodes ( _korzen and _guard,which is on the most right side of tree)*/
  {
    Node *node = new Node;
    _root = node;
    _nodeCount = 0;
  }

  TreeMap(std::initializer_list<value_type> list)
  {
    (void)list; // disables "unused argument" warning, can be removed when method is implemented.
    throw std::runtime_error("TODO");
  }

  TreeMap(const TreeMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  TreeMap(TreeMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  TreeMap& operator=(const TreeMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  TreeMap& operator=(TreeMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool isEmpty() const
  {
    if(_nodeCount == 0)return true;
    return false;
  }

  mapped_type& operator[](const key_type& key)
  {

    //TODO adding item to map
   // if(_nodeCount == 0) {
      Node *node = new Node(key, mapped_type{});
      _root->left = node;
      node->parent = _root;
      _nodeCount++;

      return node->nodeElement.second;
/*    }
    else{
      const_iterator it = find(key);
      if(it != cend()){
        return it->second;
      }
      else{
        Node *tmp = _root;
        while()
      }
    }*/
  }

  const mapped_type& valueOf(const key_type& key) const
  {
      (void)key;
      throw std::runtime_error("TODO");
  }

  mapped_type& valueOf(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  const_iterator find(const key_type& key) const
  {
    Node *tmp = _root;
    while(tmp != nullptr){
      if(tmp->nodeElement.first == key)
        return ConstIterator(tmp);
      if(key > tmp->nodeElement.first)
        tmp= tmp->right;
      else
        tmp = tmp->left;

    }
    return cend();
  }

  iterator _find(const key_type& key)
  {
    ConstIterator it = find(key);
      return iterator(it);
  }

  void remove(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  void remove(const const_iterator& it)
  {
    (void)it;
    throw std::runtime_error("TODO");
  }

  size_type getSize() const
  {
    return _nodeCount;
  }

  bool operator==(const TreeMap& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    Iterator *it = new Iterator;
    Node *tmp = _root;
    while(tmp->left != nullptr)
      tmp = tmp->left;
    it->ptr = tmp;
    return *it;
  }

  iterator end()
  {
    Iterator *it = new Iterator;
    it->ptr = _root;
    return *it;
  }

  const_iterator cbegin() const
  {
    ConstIterator *it = new ConstIterator;
    Node *tmp = _root;
    while(tmp->left != nullptr)
      tmp = tmp->left;
    it->ptr = tmp;
    return *it;
  }

  const_iterator cend() const
  {
    ConstIterator *it = new ConstIterator;
    it->ptr = _root;
    return *it;
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template<typename KeyType, typename ValueType>
class TreeMap<KeyType , ValueType>::Node
{
public:
    Node *parent;                  /*ptr to parent*/
    Node *left;                    /*ptr to left child*/
    Node *right;                   /*ptr to right child*/
    value_type nodeElement;        /* pair<const key_type, mapped_type>*/
    int hight;                     /*hight of highest subtree + 1*/

    Node()
    {
      parent = nullptr;
      left = nullptr;
      right = nullptr;
      hight = 0;
    }

    Node(const key_type key, mapped_type value):
            nodeElement(std::make_pair(key,value))
    {
      parent = nullptr;
      left = nullptr;
      right = nullptr;
      hight = 0;
    };

};


template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;

  Node *ptr;

  explicit ConstIterator()
  {
    ptr = nullptr;
  }

  ConstIterator(Node *node){
    ptr = node;
  }

  ConstIterator(const ConstIterator& other)
  {
    *this= other;
  }

  ConstIterator& operator++() /*preinkrementacja*/
  {
    if(this->ptr->parent == nullptr)throw std::out_of_range("Can't increment iterator end()");
    Node *tmp = this->ptr;
    Node *tmpParent = this->ptr->parent;
    if(tmp->right != nullptr){
      tmp = tmp->right;
      while(tmp->left != nullptr)
        tmp = tmp->left;
    }
    else if(tmpParent->left == tmp)
      tmp = tmp->parent;
    else if(tmpParent->right == tmp){
      while(tmpParent->right == tmp){
        tmp = tmpParent;
        tmpParent = tmpParent->parent;
      }
      tmp = tmp->parent;
    }
    this->ptr = tmp;
    return *this;
  }

  ConstIterator operator++(int)
  {
    const_iterator copy = *this;
    ++(*this);
    return copy;
  }

  ConstIterator& operator--() /*predekrementacja*/
  {
    //sprawdz czy nie wywolujesz --begin()
    Node *tmp = this->ptr;
    if(tmp->left != nullptr){
        tmp = tmp->left;
        while(tmp->right != nullptr)
            tmp= tmp->right;
    }
    else if(tmp->parent == nullptr)throw std::out_of_range("can't decrement begin()");
    else if (tmp->parent->right == tmp){
        tmp = tmp->parent;
    }
    else if(tmp->parent->left == tmp){
        Node *tmpCopy = tmp;
        while(tmpCopy->parent != nullptr && tmpCopy->parent->left == tmpCopy ){
            tmpCopy = tmpCopy->parent;
        }
        if(tmpCopy == nullptr)throw std::out_of_range("can't decrement begin()");
        while(tmp->parent->left == tmp)
            tmp = tmp->parent;
        tmp = tmp->parent;
    }
      this->ptr = tmp;
      return *this;
  }

  ConstIterator operator--(int)
  {
      const_iterator copy = *this;
      --(*this);
      return copy;
  }


  reference operator*() const
  {
    if( ptr->parent == nullptr)throw std::out_of_range("Getting to nullptr");
    return ptr->nodeElement;
  }

  pointer operator->() const
  {
      return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    if(ptr == other.ptr)
      return true;
    return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_MAP_H */
