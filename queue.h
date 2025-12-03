#ifndef MYSTL_QUEUE_H
#define MYSTL_QUEUE_H

#include "vector.h"

namespace MySTL {

template <typename T>
class Queue {
private:
    Vector<T> c;
public:
    Queue() = default;
    bool empty() const { return c.empty(); }
    int size() const { return c.size(); }
    void push(const T& e) { c.insert(c.size(), e); }
    T pop() { T e = c[0]; c.remove(0); return e; }
    T& front() { return c[0]; }
    const T& front() const { return c[0]; }
    void clear() { if (!c.empty()) c.remove(0, c.size()); }
};

}

#endif