#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

#include <memory>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>

namespace MySTL {

template<typename T, typename Allocator = std::allocator<T>>
class vector {
public:
    // 类型定义
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    pointer data_;          // 指向数据的指针
    size_type size_;        // 当前元素数量
    size_type capacity_;    // 当前容量
    allocator_type alloc_;  // 分配器

    // 辅助函数
    void reallocate(size_type new_capacity);
    void destroy_elements();
    void move_elements(pointer from, pointer to, size_type count);

public:
    // 构造函数
    vector() noexcept(noexcept(Allocator()));
    explicit vector(const Allocator& alloc) noexcept;
    explicit vector(size_type count, const T& value = T(), const Allocator& alloc = Allocator());
    template<typename InputIt>
    vector(InputIt first, InputIt last, const Allocator& alloc = Allocator());
    vector(const vector& other);
    vector(const vector& other, const Allocator& alloc);
    vector(vector&& other) noexcept;
    vector(vector&& other, const Allocator& alloc);
    vector(std::initializer_list<T> init, const Allocator& alloc = Allocator());

    // 析构函数
    ~vector();

    // 赋值操作符
    vector& operator=(const vector& other);
    vector& operator=(vector&& other) noexcept;
    vector& operator=(std::initializer_list<T> ilist);

    // 分配器
    allocator_type get_allocator() const noexcept;

    // 元素访问
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    pointer data() noexcept;
    const_pointer data() const noexcept;

    // 迭代器
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // 容量
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    void reserve(size_type new_cap);
    size_type capacity() const noexcept;
    void shrink_to_fit();

    // 修改器
    void clear() noexcept;
    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, T&& value);
    iterator insert(const_iterator pos, size_type count, const T& value);
    template<typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last);
    iterator insert(const_iterator pos, std::initializer_list<T> ilist);
    
    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args);
    
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    
    void push_back(const T& value);
    void push_back(T&& value);
    
    template<typename... Args>
    reference emplace_back(Args&&... args);
    
    void pop_back();
    void resize(size_type count);
    void resize(size_type count, const value_type& value);
    void swap(vector& other) noexcept;
};

// 非成员函数
template<typename T, typename Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template<typename T, typename Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template<typename T, typename Alloc>
bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template<typename T, typename Alloc>
bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template<typename T, typename Alloc>
bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template<typename T, typename Alloc>
bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template<typename T, typename Alloc>
void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs) noexcept;

} // namespace MySTL

#include "vector.tpp"  // 包含模板实现

#endif // MYSTL_VECTOR_H