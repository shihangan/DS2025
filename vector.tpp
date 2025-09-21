#ifndef MYSTL_VECTOR_TPP
#define MYSTL_VECTOR_TPP

namespace MySTL {

// 辅助函数实现
template<typename T, typename Allocator>
void vector<T, Allocator>::reallocate(size_type new_capacity) {
    if (new_capacity == 0) {
        destroy_elements();
        std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
        data_ = nullptr;
        capacity_ = 0;
        return;
    }

    pointer new_data = std::allocator_traits<Allocator>::allocate(alloc_, new_capacity);
    
    try {
        // 移动或复制现有元素
        for (size_type i = 0; i < size_; ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, new_data + i, std::move(data_[i]));
        }
    } catch (...) {
        // 如果构造失败，清理已分配的内存
        for (size_type i = 0; i < size_; ++i) {
            std::allocator_traits<Allocator>::destroy(alloc_, new_data + i);
        }
        std::allocator_traits<Allocator>::deallocate(alloc_, new_data, new_capacity);
        throw;
    }

    destroy_elements();
    if (data_) {
        std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
    }
    
    data_ = new_data;
    capacity_ = new_capacity;
}

template<typename T, typename Allocator>
void vector<T, Allocator>::destroy_elements() {
    for (size_type i = 0; i < size_; ++i) {
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
    }
    size_ = 0;
}

template<typename T, typename Allocator>
void vector<T, Allocator>::move_elements(pointer from, pointer to, size_type count) {
    if (to < from) {
        // 向前移动
        for (size_type i = 0; i < count; ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, to + i, std::move(from[i]));
            std::allocator_traits<Allocator>::destroy(alloc_, from + i);
        }
    } else if (to > from) {
        // 向后移动
        for (size_type i = count; i > 0; --i) {
            std::allocator_traits<Allocator>::construct(alloc_, to + i - 1, std::move(from[i - 1]));
            std::allocator_traits<Allocator>::destroy(alloc_, from + i - 1);
        }
    }
}

// 构造函数实现
template<typename T, typename Allocator>
vector<T, Allocator>::vector() noexcept(noexcept(Allocator()))
    : data_(nullptr), size_(0), capacity_(0), alloc_() {}

template<typename T, typename Allocator>
vector<T, Allocator>::vector(const Allocator& alloc) noexcept
    : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {}

template<typename T, typename Allocator>
vector<T, Allocator>::vector(size_type count, const T& value, const Allocator& alloc)
    : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
    if (count > 0) {
        data_ = std::allocator_traits<Allocator>::allocate(alloc_, count);
        capacity_ = count;
        
        try {
            for (size_type i = 0; i < count; ++i) {
                std::allocator_traits<Allocator>::construct(alloc_, data_ + i, value);
                ++size_;
            }
        } catch (...) {
            destroy_elements();
            std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
            throw;
        }
    }
}

template<typename T, typename Allocator>
template<typename InputIt>
vector<T, Allocator>::vector(InputIt first, InputIt last, const Allocator& alloc)
    : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
    for (auto it = first; it != last; ++it) {
        push_back(*it);
    }
}

template<typename T, typename Allocator>
vector<T, Allocator>::vector(const vector& other)
    : data_(nullptr), size_(0), capacity_(0), 
      alloc_(std::allocator_traits<Allocator>::select_on_container_copy_construction(other.alloc_)) {
    if (other.size_ > 0) {
        data_ = std::allocator_traits<Allocator>::allocate(alloc_, other.size_);
        capacity_ = other.size_;
        
        try {
            for (size_type i = 0; i < other.size_; ++i) {
                std::allocator_traits<Allocator>::construct(alloc_, data_ + i, other.data_[i]);
                ++size_;
            }
        } catch (...) {
            destroy_elements();
            std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
            throw;
        }
    }
}

template<typename T, typename Allocator>
vector<T, Allocator>::vector(const vector& other, const Allocator& alloc)
    : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
    if (other.size_ > 0) {
        data_ = std::allocator_traits<Allocator>::allocate(alloc_, other.size_);
        capacity_ = other.size_;
        
        try {
            for (size_type i = 0; i < other.size_; ++i) {
                std::allocator_traits<Allocator>::construct(alloc_, data_ + i, other.data_[i]);
                ++size_;
            }
        } catch (...) {
            destroy_elements();
            std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
            throw;
        }
    }
}

template<typename T, typename Allocator>
vector<T, Allocator>::vector(vector&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_), alloc_(std::move(other.alloc_)) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<typename T, typename Allocator>
vector<T, Allocator>::vector(vector&& other, const Allocator& alloc)
    : alloc_(alloc) {
    if (alloc_ == other.alloc_) {
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    } else {
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        if (other.size_ > 0) {
            data_ = std::allocator_traits<Allocator>::allocate(alloc_, other.size_);
            capacity_ = other.size_;
            
            try {
                for (size_type i = 0; i < other.size_; ++i) {
                    std::allocator_traits<Allocator>::construct(alloc_, data_ + i, std::move(other.data_[i]));
                    ++size_;
                }
            } catch (...) {
                destroy_elements();
                std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
                throw;
            }
        }
    }
}

template<typename T, typename Allocator>
vector<T, Allocator>::vector(std::initializer_list<T> init, const Allocator& alloc)
    : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
    if (init.size() > 0) {
        data_ = std::allocator_traits<Allocator>::allocate(alloc_, init.size());
        capacity_ = init.size();
        
        try {
            for (const auto& item : init) {
                std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, item);
                ++size_;
            }
        } catch (...) {
            destroy_elements();
            std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
            throw;
        }
    }
}

// 析构函数
template<typename T, typename Allocator>
vector<T, Allocator>::~vector() {
    destroy_elements();
    if (data_) {
        std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
    }
}

// 赋值操作符
template<typename T, typename Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(const vector& other) {
    if (this != &other) {
        if (std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value) {
            if (alloc_ != other.alloc_) {
                destroy_elements();
                if (data_) {
                    std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
                }
                data_ = nullptr;
                size_ = 0;
                capacity_ = 0;
            }
            alloc_ = other.alloc_;
        }
        
        if (other.size_ > capacity_) {
            destroy_elements();
            if (data_) {
                std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
            }
            data_ = std::allocator_traits<Allocator>::allocate(alloc_, other.size_);
            capacity_ = other.size_;
        } else {
            destroy_elements();
        }
        
        for (size_type i = 0; i < other.size_; ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + i, other.data_[i]);
            ++size_;
        }
    }
    return *this;
}

template<typename T, typename Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(vector&& other) noexcept {
    if (this != &other) {
        destroy_elements();
        if (data_) {
            std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
        }
        
        if (std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value) {
            alloc_ = std::move(other.alloc_);
        }
        
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

template<typename T, typename Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(std::initializer_list<T> ilist) {
    clear();
    if (ilist.size() > capacity_) {
        if (data_) {
            std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
        }
        data_ = std::allocator_traits<Allocator>::allocate(alloc_, ilist.size());
        capacity_ = ilist.size();
    }
    
    for (const auto& item : ilist) {
        std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, item);
        ++size_;
    }
    return *this;
}

// 分配器
template<typename T, typename Allocator>
typename vector<T, Allocator>::allocator_type vector<T, Allocator>::get_allocator() const noexcept {
    return alloc_;
}

// 元素访问
template<typename T, typename Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type pos) {
    if (pos >= size_) {
        throw std::out_of_range("vector::at: index out of range");
    }
    return data_[pos];
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type pos) const {
    if (pos >= size_) {
        throw std::out_of_range("vector::at: index out of range");
    }
    return data_[pos];
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](size_type pos) {
    return data_[pos];
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](size_type pos) const {
    return data_[pos];
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::front() {
    return data_[0];
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::front() const {
    return data_[0];
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::back() {
    return data_[size_ - 1];
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::back() const {
    return data_[size_ - 1];
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::pointer vector<T, Allocator>::data() noexcept {
    return data_;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_pointer vector<T, Allocator>::data() const noexcept {
    return data_;
}

// 迭代器
template<typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::begin() noexcept {
    return data_;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::begin() const noexcept {
    return data_;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cbegin() const noexcept {
    return data_;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::end() noexcept {
    return data_ + size_;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end() const noexcept {
    return data_ + size_;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cend() const noexcept {
    return data_ + size_;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rbegin() noexcept {
    return reverse_iterator(end());
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crbegin() const noexcept {
    return const_reverse_iterator(cend());
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::reverse_iterator vector<T, Allocator>::rend() noexcept {
    return reverse_iterator(begin());
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::const_reverse_iterator vector<T, Allocator>::crend() const noexcept {
    return const_reverse_iterator(cbegin());
}

// 容量
template<typename T, typename Allocator>
bool vector<T, Allocator>::empty() const noexcept {
    return size_ == 0;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::size() const noexcept {
    return size_;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::max_size() const noexcept {
    return std::allocator_traits<Allocator>::max_size(alloc_);
}

template<typename T, typename Allocator>
void vector<T, Allocator>::reserve(size_type new_cap) {
    if (new_cap > capacity_) {
        reallocate(new_cap);
    }
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity() const noexcept {
    return capacity_;
}

template<typename T, typename Allocator>
void vector<T, Allocator>::shrink_to_fit() {
    if (size_ < capacity_) {
        reallocate(size_);
    }
}

// 修改器
template<typename T, typename Allocator>
void vector<T, Allocator>::clear() noexcept {
    destroy_elements();
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, const T& value) {
    size_type index = pos - data_;
    
    if (size_ == capacity_) {
        size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
        reallocate(new_capacity);
    }
    
    // 移动元素为新元素腾出空间
    for (size_type i = size_; i > index; --i) {
        std::allocator_traits<Allocator>::construct(alloc_, data_ + i, std::move(data_[i - 1]));
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i - 1);
    }
    
    std::allocator_traits<Allocator>::construct(alloc_, data_ + index, value);
    ++size_;
    
    return data_ + index;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, T&& value) {
    size_type index = pos - data_;
    
    if (size_ == capacity_) {
        size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
        reallocate(new_capacity);
    }
    
    // 移动元素为新元素腾出空间
    for (size_type i = size_; i > index; --i) {
        std::allocator_traits<Allocator>::construct(alloc_, data_ + i, std::move(data_[i - 1]));
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i - 1);
    }
    
    std::allocator_traits<Allocator>::construct(alloc_, data_ + index, std::move(value));
    ++size_;
    
    return data_ + index;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, size_type count, const T& value) {
    if (count == 0) return const_cast<iterator>(pos);
    
    size_type index = pos - data_;
    
    if (size_ + count > capacity_) {
        size_type new_capacity = std::max(capacity_ * 2, size_ + count);
        reallocate(new_capacity);
    }
    
    // 移动现有元素
    for (size_type i = size_ + count - 1; i >= index + count; --i) {
        std::allocator_traits<Allocator>::construct(alloc_, data_ + i, std::move(data_[i - count]));
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i - count);
    }
    
    // 插入新元素
    for (size_type i = 0; i < count; ++i) {
        std::allocator_traits<Allocator>::construct(alloc_, data_ + index + i, value);
    }
    
    size_ += count;
    return data_ + index;
}

template<typename T, typename Allocator>
template<typename InputIt>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, InputIt first, InputIt last) {
    size_type index = pos - data_;
    size_type count = std::distance(first, last);
    
    if (count == 0) return const_cast<iterator>(pos);
    
    if (size_ + count > capacity_) {
        size_type new_capacity = std::max(capacity_ * 2, size_ + count);
        reallocate(new_capacity);
    }
    
    // 移动现有元素
    for (size_type i = size_ + count - 1; i >= index + count; --i) {
        std::allocator_traits<Allocator>::construct(alloc_, data_ + i, std::move(data_[i - count]));
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i - count);
    }
    
    // 插入新元素
    size_type i = 0;
    for (auto it = first; it != last; ++it, ++i) {
        std::allocator_traits<Allocator>::construct(alloc_, data_ + index + i, *it);
    }
    
    size_ += count;
    return data_ + index;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, std::initializer_list<T> ilist) {
    return insert(pos, ilist.begin(), ilist.end());
}

template<typename T, typename Allocator>
template<typename... Args>
typename vector<T, Allocator>::iterator vector<T, Allocator>::emplace(const_iterator pos, Args&&... args) {
    size_type index = pos - data_;
    
    if (size_ == capacity_) {
        size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
        reallocate(new_capacity);
    }
    
    // 移动元素为新元素腾出空间
    for (size_type i = size_; i > index; --i) {
        std::allocator_traits<Allocator>::construct(alloc_, data_ + i, std::move(data_[i - 1]));
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i - 1);
    }
    
    std::allocator_traits<Allocator>::construct(alloc_, data_ + index, std::forward<Args>(args)...);
    ++size_;
    
    return data_ + index;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator pos) {
    size_type index = pos - data_;
    
    std::allocator_traits<Allocator>::destroy(alloc_, data_ + index);
    
    // 移动后续元素
    for (size_type i = index; i < size_ - 1; ++i) {
        std::allocator_traits<Allocator>::construct(alloc_, data_ + i, std::move(data_[i + 1]));
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i + 1);
    }
    
    --size_;
    return data_ + index;
}

template<typename T, typename Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator first, const_iterator last) {
    size_type start_index = first - data_;
    size_type end_index = last - data_;
    size_type count = end_index - start_index;
    
    if (count == 0) return const_cast<iterator>(first);
    
    // 销毁要删除的元素
    for (size_type i = start_index; i < end_index; ++i) {
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
    }
    
    // 移动后续元素
    for (size_type i = start_index; i < size_ - count; ++i) {
        std::allocator_traits<Allocator>::construct(alloc_, data_ + i, std::move(data_[i + count]));
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i + count);
    }
    
    size_ -= count;
    return data_ + start_index;
}

template<typename T, typename Allocator>
void vector<T, Allocator>::push_back(const T& value) {
    if (size_ == capacity_) {
        size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
        reallocate(new_capacity);
    }
    
    std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, value);
    ++size_;
}

template<typename T, typename Allocator>
void vector<T, Allocator>::push_back(T&& value) {
    if (size_ == capacity_) {
        size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
        reallocate(new_capacity);
    }
    
    std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, std::move(value));
    ++size_;
}

template<typename T, typename Allocator>
template<typename... Args>
typename vector<T, Allocator>::reference vector<T, Allocator>::emplace_back(Args&&... args) {
    if (size_ == capacity_) {
        size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
        reallocate(new_capacity);
    }
    
    std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, std::forward<Args>(args)...);
    ++size_;
    return data_[size_ - 1];
}

template<typename T, typename Allocator>
void vector<T, Allocator>::pop_back() {
    if (size_ > 0) {
        --size_;
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + size_);
    }
}

template<typename T, typename Allocator>
void vector<T, Allocator>::resize(size_type count) {
    if (count > size_) {
        if (count > capacity_) {
            reallocate(count);
        }
        for (size_type i = size_; i < count; ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + i);
        }
    } else if (count < size_) {
        for (size_type i = count; i < size_; ++i) {
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
        }
    }
    size_ = count;
}

template<typename T, typename Allocator>
void vector<T, Allocator>::resize(size_type count, const value_type& value) {
    if (count > size_) {
        if (count > capacity_) {
            reallocate(count);
        }
        for (size_type i = size_; i < count; ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + i, value);
        }
    } else if (count < size_) {
        for (size_type i = count; i < size_; ++i) {
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
        }
    }
    size_ = count;
}

template<typename T, typename Allocator>
void vector<T, Allocator>::swap(vector& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    if (std::allocator_traits<Allocator>::propagate_on_container_swap::value) {
        std::swap(alloc_, other.alloc_);
    }
}

// 非成员函数
template<typename T, typename Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T, typename Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
    return !(lhs == rhs);
}

template<typename T, typename Alloc>
bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename T, typename Alloc>
bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
    return !(rhs < lhs);
}

template<typename T, typename Alloc>
bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
    return rhs < lhs;
}

template<typename T, typename Alloc>
bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
    return !(lhs < rhs);
}

template<typename T, typename Alloc>
void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace MySTL

#endif // MYSTL_VECTOR_TPP