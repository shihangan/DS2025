#ifndef MYSTL_STACK_H
#define MYSTL_STACK_H

#include "vector.h" // 使用自建的vector作为底层容器

namespace MySTL {

template <typename T, typename Container = Vector<T>>
class Stack {
protected:
    Container c;

public:
    // 构造函数
    Stack() = default;

    // 判断栈是否为空
    bool empty() const { return c.empty(); }

    // 返回栈中元素的数量
    int size() const { return c.size(); }

    // 返回栈顶元素的引用
    T& top() { return c[c.size() - 1]; }
    const T& top() const { return c[c.size() - 1]; }

    // 将元素压入栈顶
    void push(const T& value) { c.insert(c.size(), value); }

    // 移除栈顶元素并返回被移除的元素
    T pop() { T e = c[c.size() - 1]; c.remove(c.size() - 1); return e; }

    // 清空栈
    void clear() { if (!c.empty()) c.remove(0, c.size()); }

    // 遍历（从栈底到栈顶）
    template <typename VST>
    void traverse(VST& visit) {
        for (int i = 0; i < c.size(); ++i) visit(c[i]);
    }

    template <typename VST>
    void traverse(VST& visit) const {
        for (int i = 0; i < c.size(); ++i) visit(c[i]);
    }
};

} // namespace MySTL

#endif // MYSTL_STACK_H