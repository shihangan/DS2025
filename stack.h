#ifndef MYSTL_STACK_H
#define MYSTL_STACK_H

#include "vector.h" // 使用自建的vector作为底层容器

namespace MySTL {

template <typename T, typename Container = MySTL::Vector<T>>
class Stack {
public:
    // 类型定义
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;

protected:
    Container c;

public:
    // 构造函数
    Stack() = default;

    // 判断栈是否为空
    bool empty() const { return c.empty(); }

    // 返回栈中元素的数量
    size_type size() const { return c.size(); }

    // 返回栈顶元素的引用
    reference top() { return c.back(); }
    const_reference top() const { return c.back(); }

    // 将元素压入栈顶
    void push(const value_type& value) { c.push_back(value); }
    void push(value_type&& value) { c.push_back(std::move(value)); }

    // 移除栈顶元素
    void pop() { c.pop_back(); }

    // 清空栈
    void clear() { c.clear(); }
};

} // namespace MySTL

#endif // MYSTL_STACK_H