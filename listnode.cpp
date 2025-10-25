/**
 * @file listnode.cpp
 * @brief 链表节点类的实现文件
 * @author DS2025
 * @date 2025
 */

#include "listnode.h"

namespace MySTL {

// ListNode 实现
template <typename T>
ListNode<T>* ListNode<T>::insertAsSucc(T const& e) {
    ListNode<T>* x = new ListNode(e, this, succ);
    succ->pred = x; succ = x;
    return x;
}

template <typename T>
ListNode<T>* ListNode<T>::insertAsPred(T const& e) {
    ListNode<T>* x = new ListNode(e, pred, this);
    pred->succ = x; pred = x;
    return x;
}

// List 实现
template <typename T>
void List<T>::init() {
    header = new ListNode<T>;
    trailer = new ListNode<T>;
    header->succ = trailer; header->pred = nullptr;
    trailer->pred = header; trailer->succ = nullptr;
    _size = 0;
}

template <typename T>
List<T>::List(List<T> const& L) {
    init(); copyNodes(L.first(), L._size);
}

template <typename T>
List<T>::List(List<T> const& L, int r, int n) {
    init();
    ListNode<T>* p = L.first();
    while (0 < r--) p = p->succ;
    copyNodes(p, n);
}

template <typename T>
List<T>::List(ListNode<T>* p, int n) {
    init(); copyNodes(p, n);
}

template <typename T>
List<T>::~List() {
    clear(); delete header; delete trailer;
}

template <typename T>
int List<T>::clear() {
    int oldSize = _size;
    while (0 < _size) remove(header->succ);
    return oldSize;
}

template <typename T>
void List<T>::copyNodes(ListNode<T>* p, int n) {
    init();
    while (n--) {
        insertAsLast(p->data); p = p->succ;
    }
}

template <typename T>
T& List<T>::operator[](int r) const {
    ListNode<T>* p = first();
    while (0 < r--) p = p->succ;
    return p->data;
}

template <typename T>
ListNode<T>* List<T>::find(T const& e, int n, ListNode<T>* p) const {
    while (0 < n--)
        if (e == (p = p->pred)->data) return p;
    return nullptr;
}

template <typename T>
ListNode<T>* List<T>::search(T const& e, int n, ListNode<T>* p) const {
    while (0 <= n--)
        if (((p = p->pred)->data) <= e) break;
    return p;
}

template <typename T>
ListNode<T>* List<T>::insertAsFirst(T const& e) {
    _size++; return header->insertAsSucc(e);
}

template <typename T>
ListNode<T>* List<T>::insertAsLast(T const& e) {
    _size++; return trailer->insertAsPred(e);
}

template <typename T>
ListNode<T>* List<T>::insertBefore(ListNode<T>* p, T const& e) {
    _size++; return p->insertAsPred(e);
}

template <typename T>
ListNode<T>* List<T>::insertAfter(ListNode<T>* p, T const& e) {
    _size++; return p->insertAsSucc(e);
}

template <typename T>
T List<T>::remove(ListNode<T>* p) {
    T e = p->data;
    p->pred->succ = p->succ; p->succ->pred = p->pred;
    delete p; _size--;
    return e;
}

template <typename T>
void List<T>::merge(ListNode<T>*& p, int n, List<T>& L, ListNode<T>* q, int m) {
    ListNode<T>* pp = p->pred;
    while (0 < m)
        if ((0 < n) && (p->data <= q->data)) {
            if (q == (p = p->succ)) break; n--;
        } else {
            insertBefore(p, L.remove((q = q->succ)->pred)); m--;
        }
    p = pp->succ;
}

template <typename T>
void List<T>::mergeSort(ListNode<T>*& p, int n) {
    if (n < 2) return;
    int m = n >> 1;
    ListNode<T>* q = p; for (int i = 0; i < m; i++) q = q->succ;
    mergeSort(p, m); mergeSort(q, n - m);
    merge(p, m, *this, q, n - m);
}

template <typename T>
void List<T>::selectionSort(ListNode<T>* p, int n) {
    ListNode<T>* head = p->pred; ListNode<T>* tail = p;
    for (int i = 0; i < n; i++) tail = tail->succ;
    while (1 < n) {
        ListNode<T>* max = selectMax(head->succ, n);
        insertBefore(tail, remove(max));
        tail = tail->pred; n--;
    }
}

template <typename T>
void List<T>::insertionSort(ListNode<T>* p, int n) {
    for (int r = 0; r < n; r++) {
        insertAfter(search(p->data, r, p), p->data);
        p = p->succ; remove(p->pred);
    }
}

template <typename T>
void List<T>::sort(ListNode<T>* p, int n) {
    mergeSort(p, n);
}

template <typename T>
int List<T>::deduplicate() {
    if (_size < 2) return 0;
    int oldSize = _size;
    ListNode<T>* p = header; int r = 0;
    while (trailer != (p = p->succ)) {
        ListNode<T>* q = find(p->data, r, p);
        q ? remove(q) : r++;
    }
    return oldSize - _size;
}

template <typename T>
int List<T>::uniquify() {
    if (_size < 2) return 0;
    int oldSize = _size;
    ListNode<T>* p = first(); ListNode<T>* q;
    while (trailer != (q = p->succ))
        if (p->data != q->data) p = q;
        else remove(q);
    return oldSize - _size;
}

template <typename T>
void List<T>::reverse() {
    if (_size < 2) return;
    ListNode<T>* p; ListNode<T>* q;
    for (p = header, q = p->succ; p != trailer; p = q, q = p->succ)
        p->pred = q;
    trailer->pred = nullptr;
}

template <typename T>
void List<T>::traverse(void (*visit)(T&)) {
    for (ListNode<T>* p = header->succ; p != trailer; p = p->succ)
        visit(p->data);
}

template <typename T>
template <typename VST>
void List<T>::traverse(VST& visit) {
    for (ListNode<T>* p = header->succ; p != trailer; p = p->succ)
        visit(p->data);
}

template <typename T>
int List<T>::disordered() const {
    int n = 0;
    for (ListNode<T>* p = first(); trailer != p->succ; p = p->succ)
        if (p->data > p->succ->data) n++;
    return n;
}

template <typename T>
ListNode<T>* List<T>::selectMax(ListNode<T>* p, int n) {
    ListNode<T>* max = p;
    for (ListNode<T>* cur = p; 1 < n; n--)
        if (!((cur = cur->succ)->data < max->data))
            max = cur;
    return max;
}

} // namespace MySTL