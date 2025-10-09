/**
 * @file stack.cpp
 * @brief 栈（Stack）数据结构的完整实现文件
 * @author DS2025
 * @date 2025
 * 
 * 基于向量（Vector）实现的栈数据结构，支持泛型编程
 * 提供标准的栈操作：push、pop、top、empty、size等
 * 
 * @tparam T 栈中存储的元素类型
 */

#include <stdexcept>
#include <iostream>
#include <vector>  // 使用标准库的vector

// 为了保持兼容性，我们使用标准库的vector
namespace MySTL {
    template<typename T>
    using vector = std::vector<T>;
}

/**
 * @brief 栈模板类
 * @tparam T 栈中存储的元素类型
 * 
 * 栈是一种后进先出（LIFO, Last In First Out）的数据结构
 * 只能在栈顶进行插入和删除操作
 * 
 * @time 各操作的时间复杂度：
 *       - push: O(1) 摊销时间复杂度
 *       - pop: O(1)
 *       - top: O(1)
 *       - empty: O(1)
 *       - size: O(1)
 * 
 * @space 空间复杂度：O(n)，其中n为栈中元素个数
 */
template <typename T>
class Stack {
private:
    MySTL::vector<T> _data;  ///< 底层存储容器，使用向量实现

public:
    // 构造函数和析构函数
    /**
     * @brief 默认构造函数
     * @time O(1)
     */
    Stack() : _data() {
        // Vector的默认构造函数会自动初始化
    }

    /**
     * @brief 拷贝构造函数
     * @param other 要拷贝的栈
     * @time O(n)
     */
    Stack(const Stack<T>& other) : _data(other._data) {
        // Vector的拷贝构造函数会处理深拷贝
    }

    /**
     * @brief 析构函数
     * @time O(n)
     */
    ~Stack() {
        // Vector的析构函数会自动清理资源
    }

    // 赋值操作符
    /**
     * @brief 赋值操作符重载
     * @param other 要赋值的栈
     * @return Stack<T>& 返回当前栈的引用
     * @time O(n)
     */
    Stack<T>& operator=(const Stack<T>& other) {
        if (this != &other) {
            _data = other._data;  // 使用Vector的赋值运算符
        }
        return *this;
    }

    // 栈的基本操作
    /**
     * @brief 将元素压入栈顶
     * @param elem 要压入的元素
     * @time O(1) 摊销时间复杂度
     */
    void push(const T& elem) {
        _data.push_back(elem);  // 在末尾插入元素
    }

    /**
     * @brief 弹出栈顶元素
     * @return T 被弹出的栈顶元素
     * @time O(1)
     * @note 调用前需确保栈非空，否则行为未定义
     */
    T pop() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        
        T elem = _data.back();  // 获取栈顶元素
        _data.pop_back();       // 删除栈顶元素
        return elem;
    }

    /**
     * @brief 获取栈顶元素的引用（可修改）
     * @return T& 栈顶元素的引用
     * @time O(1)
     * @note 调用前需确保栈非空，否则行为未定义
     */
    T& top() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return _data.back();
    }

    /**
     * @brief 获取栈顶元素的常量引用（只读）
     * @return const T& 栈顶元素的常量引用
     * @time O(1)
     * @note 调用前需确保栈非空，否则行为未定义
     */
    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return _data.back();
    }

    // 状态查询
    /**
     * @brief 判断栈是否为空
     * @return bool 栈为空返回true，否则返回false
     * @time O(1)
     */
    bool empty() const {
        return _data.empty();
    }

    /**
     * @brief 获取栈中元素个数
     * @return int 栈中元素的个数
     * @time O(1)
     */
    int size() const {
        return static_cast<int>(_data.size());
    }

    /**
     * @brief 清空栈中所有元素
     * @time O(n)
     */
    void clear() {
        _data.clear();
    }

    // 调试和遍历
    /**
     * @brief 遍历栈中所有元素（从栈底到栈顶）
     * @param visit 访问函数，接受一个T类型的参数
     * @time O(n)
     */
    template <typename VST>
    void traverse(VST& visit) {
        for (size_t i = 0; i < _data.size(); ++i) {
            visit(_data[i]);
        }
    }

    /**
     * @brief 遍历栈中所有元素（从栈底到栈顶，常量版本）
     * @param visit 访问函数，接受一个const T&类型的参数
     * @time O(n)
     */
    template <typename VST>
    void traverse(VST& visit) const {
        for (size_t i = 0; i < _data.size(); ++i) {
            visit(_data[i]);
        }
    }
};

// 显式实例化常用类型
template class Stack<int>;
template class Stack<double>;
template class Stack<char>;
template class Stack<std::string>;