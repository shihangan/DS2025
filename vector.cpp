// MySTL Vector implementation definitions
// This file is included by vector.h and must ONLY contain template member definitions.
// To avoid multiple definition across translation units, we wrap with an include guard.

#ifndef VECTOR_IMPLEMENTATION_INCLUDED
#define VECTOR_IMPLEMENTATION_INCLUDED

#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace MySTL {

// NOTE: Rank typedef and DEFAULT_CAPACITY are declared in vector.h

// --- Constructors & Destructor ---
template<typename T>
Vector<T>::Vector(int c, int s, T v) {
    if (c < DEFAULT_CAPACITY) c = DEFAULT_CAPACITY;
    _capacity = c;
    _size = 0;
    _elem = new T[_capacity];
    for (; _size < s; ++_size) _elem[_size] = v;
}

template<typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
    Rank n = hi - lo;
    _capacity = std::max(DEFAULT_CAPACITY, n * 2);
    _elem = new T[_capacity];
    _size = 0;
    for (; lo < hi; ++lo) _elem[_size++] = A[lo];
}

template<typename T>
Vector<T>::Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }

template<typename T>
Vector<T>::Vector(T const* A, Rank n) { copyFrom(A, 0, n); }

template<typename T>
Vector<T>::Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }

template<typename T>
Vector<T>::Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }

template<typename T>
Vector<T>::~Vector() { if (_elem) { delete[] _elem; _elem = nullptr; } _size = 0; _capacity = 0; }

// --- Basic queries ---
template<typename T>
inline Rank Vector<T>::size() const { return _size; }

template<typename T>
inline bool Vector<T>::empty() const { return _size <= 0; }

template<typename T>
inline int Vector<T>::disordered() const {
    for (Rank i = 1; i < _size; ++i) {
        if (_elem[i - 1] > _elem[i]) return true;
    }
    return false;
}

// --- Internal helpers ---
template<typename T>
void Vector<T>::expand() {
    if (_size < _capacity) return;
    _capacity = std::max(DEFAULT_CAPACITY, _capacity * 2);
    T* old = _elem;
    _elem = new T[_capacity];
    for (Rank i = 0; i < _size; ++i) _elem[i] = old[i];
    delete[] old;
}

template<typename T>
void Vector<T>::shrink() {
    if (_capacity <= DEFAULT_CAPACITY) return;
    if (_size * 4 > _capacity) return; // avoid too frequent shrink
    Rank newCap = std::max(DEFAULT_CAPACITY, _capacity / 2);
    T* old = _elem;
    _elem = new T[newCap];
    for (Rank i = 0; i < _size; ++i) _elem[i] = old[i];
    _capacity = newCap;
    delete[] old;
}

// --- Searching ---
template<typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
    for (Rank i = hi; i > lo; --i) {
        if (_elem[i - 1] == e) return i - 1;
    }
    return -1;
}

template<typename T>
Rank Vector<T>::find(T const& e) const {
    return find(e, 0, _size);
}

// binary search on sorted Vector
template<typename T>
static Rank __binSearch(const T* A, const Rank lo, const Rank hi, const T& e) {
    Rank l = lo, r = hi;
    while (l < r) {
        Rank m = (l + r) >> 1;
        if (e < A[m]) r = m;
        else if (A[m] < e) l = m + 1;
        else return m;
    }
    return l - 1; // not found: return last smaller
}

template<typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {
    if (lo >= hi) return lo - 1;
    return __binSearch(_elem, lo, hi, e);
}

template<typename T>
Rank Vector<T>::search(T const& e) const {
    return (_size <= 0) ? -1 : search(e, 0, _size);
}

// --- Element access ---
template<typename T>
T& Vector<T>::operator[](Rank r) const { return const_cast<T&>(_elem[r]); }

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V) {
    if (this == &V) return *this;
    delete[] _elem;
    copyFrom(V._elem, 0, V._size);
    return *this;
}

// --- Insertion & Deletion ---
template<typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
    if (r < 0) r = 0;
    if (r > _size) r = _size;
    expand();
    for (Rank i = _size; i > r; --i) _elem[i] = _elem[i - 1];
    _elem[r] = e;
    ++_size;
    return r;
}

template<typename T>
Rank Vector<T>::insert(T const& e) { return insert(_size, e); }

template<typename T>
T Vector<T>::remove(Rank r) {
    if (r < 0 || r >= _size) return T();
    T e = _elem[r];
    for (Rank i = r + 1; i < _size; ++i) _elem[i - 1] = _elem[i];
    --_size;
    shrink();
    return e;
}

template<typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
    if (lo < 0) lo = 0;
    if (hi > _size) hi = _size;
    if (lo >= hi) return 0;
    Rank n = hi - lo;
    for (Rank i = hi; i < _size; ++i) _elem[i - n] = _elem[i];
    _size -= n;
    shrink();
    return n;
}

// --- Sorting & Shuffling ---
template<typename T>
void Vector<T>::sort(Rank lo, Rank hi) {
    if (lo < 0) lo = 0; if (hi > _size) hi = _size; if (lo >= hi) return;
    std::sort(_elem + lo, _elem + hi);
}

template<typename T>
void Vector<T>::sort() { sort(0, _size); }

template<typename T>
void Vector<T>::unsort(Rank lo, Rank hi) {
    if (lo < 0) lo = 0; if (hi > _size) hi = _size; if (lo >= hi) return;
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    for (Rank i = hi - 1; i > lo; --i) {
        Rank j = lo + std::rand() % (i - lo + 1);
        std::swap(_elem[i], _elem[j]);
    }
}

template<typename T>
void Vector<T>::unsort() { unsort(0, _size); }

// --- Deduplicate & Uniquify ---
template<typename T>
int Vector<T>::deduplicate() {
    if (_size < 2) return 0;
    int oldSize = _size;
    Rank i = 1;
    while (i < _size) {
        // if current element exists in prefix [0, i)
        if (find(_elem[i], 0, i) >= 0) {
            remove(i);
        } else {
            ++i;
        }
    }
    return oldSize - _size;
}

template<typename T>
int Vector<T>::uniquify() {
    if (_size < 2) return 0;
    Rank i = 0, j = 0;
    while (++j < _size) {
        if (_elem[i] != _elem[j]) _elem[++i] = _elem[j];
    }
    _size = i + 1;
    shrink();
    return j - i - 1; // number of removed elements
}

// --- Traverse ---
template<typename T>
void Vector<T>::traverse(void (*visit)(T&)) {
    for (Rank i = 0; i < _size; ++i) visit(_elem[i]);
}

template<typename T>
template<typename VST>
void Vector<T>::traverse(VST& visit) {
    for (Rank i = 0; i < _size; ++i) visit(_elem[i]);
}

} // namespace MySTL

#endif // VECTOR_IMPLEMENTATION_INCLUDED