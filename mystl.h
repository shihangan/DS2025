/**
 * @file mystl.h
 * @brief MySTL 数据结构库的统一头文件
 * @author DS2025
 * @date 2025
 * 
 * 包含所有数据结构的声明
 */

#ifndef MYSTL_H
#define MYSTL_H

#include <stdexcept>
#include <iostream>
#include <vector>

// 为了保持兼容性，我们使用标准库的vector
namespace MySTL {
    template<typename T>
    using vector = std::vector<T>;
}

// 前向声明
template <typename T> class ListNode;
template <typename T> class List;
template <typename T> class Stack;

// Stack类声明
template <typename T>
class Stack {
private:
    MySTL::vector<T> _data;

public:
    Stack();
    Stack(const Stack<T>& other);
    ~Stack();
    Stack<T>& operator=(const Stack<T>& other);
    
    void push(const T& e);
    T pop();
    T& top();
    const T& top() const;
    bool empty() const;
    int size() const;
    void clear();
    
    template <typename VST>
    void traverse(VST& visit) {
        for (size_t i = 0; i < _data.size(); ++i) {
            visit(_data[i]);
        }
    }
    
    template <typename VST>
    void traverse(VST& visit) const {
        for (size_t i = 0; i < _data.size(); ++i) {
            visit(_data[i]);
        }
    }
};

// ListNode类声明
template <typename T>
class ListNode {
public:
    T data;
    ListNode<T>* pred;
    ListNode<T>* succ;
    
    ListNode() {}
    ListNode(T e, ListNode<T>* p = nullptr, ListNode<T>* s = nullptr);
    
    ListNode<T>* insertAsPred(T const& e);
    ListNode<T>* insertAsSucc(T const& e);
};

// List类声明
template <typename T>
class List {
private:
    int _size;
    ListNode<T>* header;
    ListNode<T>* trailer;
    
protected:
    void init();
    int clear();
    void copyNodes(ListNode<T>* p, int n);
    void merge(ListNode<T>*& p, int n, List<T>& L, ListNode<T>* q, int m);
    void mergeSort(ListNode<T>*& p, int n);
    void selectionSort(ListNode<T>* p, int n);
    void insertionSort(ListNode<T>* p, int n);
    
public:
    List() { init(); }
    List(List<T> const& L);
    List(List<T> const& L, int r, int n);
    List(ListNode<T>* p, int n);
    ~List() { clear(); delete header; delete trailer; }
    
    int size() const { return _size; }
    bool empty() const { return _size <= 0; }
    T& operator[](int r) const;
    ListNode<T>* first() const { return header->succ; }
    ListNode<T>* last() const { return trailer->pred; }
    bool valid(ListNode<T>* p) { return p && (trailer != p) && (header != p); }
    
    ListNode<T>* find(T const& e) const { return find(e, _size, trailer); }
    ListNode<T>* find(T const& e, int n, ListNode<T>* p) const;
    ListNode<T>* search(T const& e) const { return search(e, _size, trailer); }
    ListNode<T>* search(T const& e, int n, ListNode<T>* p) const;
    ListNode<T>* selectMax(ListNode<T>* p, int n);
    ListNode<T>* selectMax() { return selectMax(header->succ, _size); }
    
    ListNode<T>* insertAsFirst(T const& e);
    ListNode<T>* insertAsLast(T const& e);
    ListNode<T>* insertBefore(ListNode<T>* p, T const& e);
    ListNode<T>* insertAfter(ListNode<T>* p, T const& e);
    
    T remove(ListNode<T>* p);
    
    void merge(List<T>& L) { merge(first(), size(), L, L.first(), L._size); }
    void sort(ListNode<T>* p, int n);
    void sort() { sort(first(), _size); }
    int deduplicate();
    int uniquify();
    
    void reverse();
    
    template <typename VST>
    void traverse(VST& visit);
};

#endif // MYSTL_H