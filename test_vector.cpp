#include "vector.h"
#include <iostream>
#include <string>
#include <cassert>

void test_constructors() {
    std::cout << "Testing constructors..." << std::endl;
    
    // 默认构造函数
    MySTL::vector<int> v1;
    assert(v1.empty());
    assert(v1.size() == 0);
    assert(v1.capacity() == 0);
    
    // 带计数的构造函数
    MySTL::vector<int> v2(5, 42);
    assert(v2.size() == 5);
    assert(v2.capacity() >= 5);
    for (size_t i = 0; i < v2.size(); ++i) {
        assert(v2[i] == 42);
    }
    
    // 拷贝构造函数
    MySTL::vector<int> v3(v2);
    assert(v3.size() == v2.size());
    for (size_t i = 0; i < v3.size(); ++i) {
        assert(v3[i] == v2[i]);
    }
    
    // 移动构造函数
    MySTL::vector<int> v4(std::move(v3));
    assert(v4.size() == 5);
    assert(v3.empty());
    
    // 初始化列表构造函数
    MySTL::vector<int> v5{1, 2, 3, 4, 5};
    assert(v5.size() == 5);
    for (size_t i = 0; i < v5.size(); ++i) {
        assert(v5[i] == static_cast<int>(i + 1));
    }
    
    std::cout << "Constructors test passed!" << std::endl;
}

void test_element_access() {
    std::cout << "Testing element access..." << std::endl;
    
    MySTL::vector<int> v{10, 20, 30, 40, 50};
    
    // operator[]
    assert(v[0] == 10);
    assert(v[4] == 50);
    
    // at()
    assert(v.at(0) == 10);
    assert(v.at(4) == 50);
    
    // 测试at()的边界检查
    try {
        v.at(10);
        assert(false); // 不应该到达这里
    } catch (const std::out_of_range&) {
        // 预期的异常
    }
    
    // front() 和 back()
    assert(v.front() == 10);
    assert(v.back() == 50);
    
    // data()
    assert(*v.data() == 10);
    
    std::cout << "Element access test passed!" << std::endl;
}

void test_iterators() {
    std::cout << "Testing iterators..." << std::endl;
    
    MySTL::vector<int> v{1, 2, 3, 4, 5};
    
    // 正向迭代器
    int expected = 1;
    for (auto it = v.begin(); it != v.end(); ++it) {
        assert(*it == expected++);
    }
    
    // const迭代器
    const MySTL::vector<int>& cv = v;
    expected = 1;
    for (auto it = cv.cbegin(); it != cv.cend(); ++it) {
        assert(*it == expected++);
    }
    
    // 反向迭代器
    expected = 5;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        assert(*it == expected--);
    }
    
    // 范围for循环
    expected = 1;
    for (const auto& val : v) {
        assert(val == expected++);
    }
    
    std::cout << "Iterators test passed!" << std::endl;
}

void test_capacity() {
    std::cout << "Testing capacity..." << std::endl;
    
    MySTL::vector<int> v;
    
    // empty()
    assert(v.empty());
    
    v.push_back(1);
    assert(!v.empty());
    assert(v.size() == 1);
    
    // reserve()
    v.reserve(100);
    assert(v.capacity() >= 100);
    assert(v.size() == 1);
    
    // resize()
    v.resize(5, 42);
    assert(v.size() == 5);
    assert(v[1] == 42);
    assert(v[4] == 42);
    
    v.resize(3);
    assert(v.size() == 3);
    
    std::cout << "Capacity test passed!" << std::endl;
}

void test_modifiers() {
    std::cout << "Testing modifiers..." << std::endl;
    
    MySTL::vector<int> v;
    
    // push_back()
    for (int i = 1; i <= 5; ++i) {
        v.push_back(i);
    }
    assert(v.size() == 5);
    
    // pop_back()
    v.pop_back();
    assert(v.size() == 4);
    assert(v.back() == 4);
    
    // insert()
    auto it = v.insert(v.begin() + 2, 99);
    assert(*it == 99);
    assert(v[2] == 99);
    assert(v.size() == 5);
    
    // insert multiple
    v.insert(v.begin(), 3, 88);
    assert(v.size() == 8);
    assert(v[0] == 88);
    assert(v[1] == 88);
    assert(v[2] == 88);
    
    // erase()
    it = v.erase(v.begin());
    assert(*it == 88);
    assert(v.size() == 7);
    
    // erase range
    it = v.erase(v.begin(), v.begin() + 2);
    assert(v.size() == 5);
    
    // clear()
    v.clear();
    assert(v.empty());
    assert(v.size() == 0);
    
    std::cout << "Modifiers test passed!" << std::endl;
}

void test_emplace() {
    std::cout << "Testing emplace..." << std::endl;
    
    MySTL::vector<std::string> v;
    
    // emplace_back()
    v.emplace_back("Hello");
    v.emplace_back(5, 'A'); // 构造字符串 "AAAAA"
    
    assert(v.size() == 2);
    assert(v[0] == "Hello");
    assert(v[1] == "AAAAA");
    
    // emplace()
    auto it = v.emplace(v.begin() + 1, "World");
    assert(*it == "World");
    assert(v[1] == "World");
    assert(v.size() == 3);
    
    std::cout << "Emplace test passed!" << std::endl;
}

void test_assignment() {
    std::cout << "Testing assignment..." << std::endl;
    
    MySTL::vector<int> v1{1, 2, 3, 4, 5};
    MySTL::vector<int> v2;
    
    // 拷贝赋值
    v2 = v1;
    assert(v2.size() == v1.size());
    for (size_t i = 0; i < v2.size(); ++i) {
        assert(v2[i] == v1[i]);
    }
    
    // 移动赋值
    MySTL::vector<int> v3;
    v3 = std::move(v2);
    assert(v3.size() == 5);
    assert(v2.empty());
    
    // 初始化列表赋值
    v3 = {10, 20, 30};
    assert(v3.size() == 3);
    assert(v3[0] == 10);
    assert(v3[1] == 20);
    assert(v3[2] == 30);
    
    std::cout << "Assignment test passed!" << std::endl;
}

void test_comparison() {
    std::cout << "Testing comparison..." << std::endl;
    
    MySTL::vector<int> v1{1, 2, 3};
    MySTL::vector<int> v2{1, 2, 3};
    MySTL::vector<int> v3{1, 2, 4};
    MySTL::vector<int> v4{1, 2};
    
    // 相等比较
    assert(v1 == v2);
    assert(!(v1 == v3));
    assert(v1 != v3);
    
    // 字典序比较
    assert(v1 < v3);
    assert(v4 < v1);
    assert(v3 > v1);
    assert(v1 > v4);
    assert(v1 <= v2);
    assert(v1 >= v2);
    
    std::cout << "Comparison test passed!" << std::endl;
}

void test_swap() {
    std::cout << "Testing swap..." << std::endl;
    
    MySTL::vector<int> v1{1, 2, 3};
    MySTL::vector<int> v2{4, 5, 6, 7};
    
    auto v1_size = v1.size();
    auto v2_size = v2.size();
    
    v1.swap(v2);
    
    assert(v1.size() == v2_size);
    assert(v2.size() == v1_size);
    assert(v1[0] == 4);
    assert(v2[0] == 1);
    
    // 测试非成员swap
    MySTL::swap(v1, v2);
    assert(v1.size() == v1_size);
    assert(v2.size() == v2_size);
    assert(v1[0] == 1);
    assert(v2[0] == 4);
    
    std::cout << "Swap test passed!" << std::endl;
}

int main() {
    try {
        test_constructors();
        test_element_access();
        test_iterators();
        test_capacity();
        test_modifiers();
        test_emplace();
        test_assignment();
        test_comparison();
        test_swap();
        
        std::cout << "\n=== All tests passed! ===" << std::endl;
        std::cout << "MySTL::vector implementation is working correctly!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Test failed with unknown exception!" << std::endl;
        return 1;
    }
    
    return 0;
}