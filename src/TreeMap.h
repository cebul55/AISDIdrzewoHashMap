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

 ~TreeMap(){
        if(_root!=nullptr && _root->left!= nullptr ) {
            _clear(_root->left);
            delete _root;
            _root = nullptr;
            _nodeCount = 0;
        }
  }

  TreeMap(std::initializer_list<value_type> list): TreeMap()
  {
    for(auto& a : list) valueOf(a.first) = a.second;
  }

  TreeMap(const TreeMap& other) : TreeMap()
  {
    for (auto& a : other) valueOf(a.first) = a.second;
  }

  TreeMap(TreeMap&& other)
  {
    _nodeCount = other._nodeCount;
    _root = other._root;
    other._nodeCount = 0;
    other._root = nullptr;
  }

  TreeMap& operator=(const TreeMap& other)
  {
    if(this == &other) return *this;
    _clear(_root->left);
    _nodeCount=0;
    for (auto& a : other) valueOf(a.first) = a.second;
    return *this;
  }

  TreeMap& operator=(TreeMap&& other)
  {
    _clear(_root->left);
    _nodeCount = other._nodeCount;
    _root = other._root;
    other._root = nullptr;
    other._nodeCount = 0;

    return *this;
  }

  bool isEmpty() const
  {
    if(_nodeCount == 0)return true;
    return false;
  }

  mapped_type& operator[](const key_type& key)
  {

    return valueOf(key);

  }

  const mapped_type& valueOf(const key_type& key) const
  {
      const_iterator it = find(key);
      if(it == cend())throw std::out_of_range("no such key");

      return it->second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    if(_nodeCount == 0) {
      Node *node = new Node(key, mapped_type{});
      _root->left = node;
      node->parent = _root;
      _nodeCount++;

      return node->nodeElement.second;
    }
    else{
      Node *pos = _root->left;
      while (true){
        key_type k = pos->nodeElement.first;

        if(k == key)
          return pos->nodeElement.second;

        Node *tmp;
        if(k < key)
          tmp = pos->right;
        else
          tmp = pos->left;

        if(tmp == nullptr){
          Node *node = new Node(key, mapped_type{});
          node->parent = pos;
          if(k<key)
            pos->right = node;
          else
            pos->left = node;

          _nodeCount++;
          return node->nodeElement.second;
        }

        pos = tmp;
      }
    }
  }

  const_iterator _find(const key_type& key) const
  {
    Node *tmp = _root->left;
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

  const_iterator find(const key_type& key) const
    {
      return _find(key);
    }

  iterator find(const key_type& key)
  {
    ConstIterator it = _find(key);
      return iterator(it);
  }

  void _clear(Node *root){
    if(root != nullptr){
      _clear(root->right);
      _clear(root->left);
      remove(root->nodeElement.first);
    }
  }

  void remove(const key_type& key)
  {
    remove(find(key));
  }

  void remove(const const_iterator& it)
  {
    if(it == cend())throw std::out_of_range("No such node");

    Node *toDestroy = it.ptr;
    Node *parent = it.ptr->parent;
    Node *left = it.ptr->left;
    Node *right = it.ptr->right;

    if(left == nullptr && right == nullptr){
      if(parent->right == toDestroy)
        parent->right = nullptr;
      else
        parent->left = nullptr;

      /*toDestroy->parent= nullptr;
      (&toDestroy->nodeElement).~value_type();*/
      delete toDestroy;
    }
    else if(left != nullptr && right != nullptr){
      Node *tmp; /*wskaznik na node ktory bedziemy przepinac*/
      if(parent->left == toDestroy){
        tmp = toDestroy->right;
        while(tmp->left != nullptr)
          tmp = tmp->left;

        //mamy juz odpowiednie wskazniki ustawione teraz nastepuje przepinanie
        parent->left = right;
        right->parent = parent;
        tmp->left = left;
        left->parent = tmp;

        /*toDestroy->parent = toDestroy->left = toDestroy->right = nullptr;
        (&toDestroy->nodeElement).~value_type();*/
        delete toDestroy;
      }
      else {
        tmp = toDestroy->left;
        while(tmp->right != nullptr)
          tmp = tmp->right;

        parent->right = left;
        left->parent = parent;
        tmp->right = right;
        right->parent = tmp;

        /*toDestroy->parent = toDestroy->left = toDestroy->right = nullptr;
        (&toDestroy->nodeElement).~value_type();*/
        delete toDestroy;
      }
    }
    else{
      //istnieje 1 dziecko
      if(parent->right == toDestroy){
        if(right!= nullptr){
          parent->right = right;
          right->parent = parent;
        }
        else{
          parent->right = left;
          left->parent = parent;
        }
      }
      else{
        if(right!= nullptr){
          parent->left = right;
          right->parent = parent;
        }
        else{
          parent->left=left;
          left->parent = parent;
        }
      }

      /*toDestroy->parent = toDestroy->left = toDestroy->right = nullptr;
      (&toDestroy->nodeElement).~value_type();*/
      delete toDestroy;
    }
    _nodeCount--;
  }

  size_type getSize() const
  {
    return _nodeCount;
  }

  bool operator==(const TreeMap& other) const
  {
    if(getSize() != other.getSize())return false;

    ConstIterator a = cbegin();
    ConstIterator b = other.cbegin();
    while(a != cend() && b != other.cend()){
      if(*a++ != *b++)return false;
    }

    return true;
  }

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
      iterator it;
    Node *tmp = _root;
    while(tmp->left != nullptr)
      tmp = tmp->left;
    it.ptr = tmp;
    return it;
  }

  iterator end()
  {
      iterator it;
      it.ptr = _root;
    return it;
  }

  const_iterator cbegin() const
  {
    //ConstIterator *it = new ConstIterator;
      const_iterator it;

      Node *tmp = _root;
    while(tmp->left != nullptr)
      tmp = tmp->left;
    it.ptr = tmp;
    return it;
  }

  const_iterator cend() const
  {
      const_iterator it;

      it.ptr = _root;
    return it;
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

    ~Node(){
      (&nodeElement)->~value_type();
    }

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
