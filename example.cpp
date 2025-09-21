#include "vector.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "=== MySTL::vector Example ===" << std::endl;
    
    // 1. 基本构造和初始化
    std::cout << "\n1. 构造和初始化:" << std::endl;
    MySTL::vector<int> v1;                    // 默认构造
    MySTL::vector<int> v2(5, 10);             // 5个元素，值为10
    MySTL::vector<int> v3{1, 2, 3, 4, 5};     // 初始化列表
    
    std::cout << "v1 size: " << v1.size() << std::endl;
    std::cout << "v2 size: " << v2.size() << ", first element: " << v2[0] << std::endl;
    std::cout << "v3 elements: ";
    for (const auto& val : v3) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 2. 添加和删除元素
    std::cout << "\n2. 添加和删除元素:" << std::endl;
    v1.push_back(100);
    v1.push_back(200);
    v1.push_back(300);
    
    std::cout << "v1 after push_back: ";
    for (size_t i = 0; i < v1.size(); ++i) {
        std::cout << v1[i] << " ";
    }
    std::cout << std::endl;
    
    v1.pop_back();
    std::cout << "v1 after pop_back: ";
    for (const auto& val : v1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 3. 插入和删除
    std::cout << "\n3. 插入和删除:" << std::endl;
    v1.insert(v1.begin() + 1, 150);
    std::cout << "v1 after insert 150 at position 1: ";
    for (const auto& val : v1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    v1.erase(v1.begin());
    std::cout << "v1 after erase first element: ";
    for (const auto& val : v1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 4. 容量管理
    std::cout << "\n4. 容量管理:" << std::endl;
    std::cout << "v1 size: " << v1.size() << ", capacity: " << v1.capacity() << std::endl;
    
    v1.reserve(20);
    std::cout << "v1 after reserve(20) - size: " << v1.size() << ", capacity: " << v1.capacity() << std::endl;
    
    v1.resize(5, 999);
    std::cout << "v1 after resize(5, 999): ";
    for (const auto& val : v1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 5. 使用字符串
    std::cout << "\n5. 字符串vector示例:" << std::endl;
    MySTL::vector<std::string> words;
    words.emplace_back("Hello");
    words.emplace_back("World");
    words.push_back("from");
    words.push_back("MySTL");
    
    std::cout << "Words: ";
    for (const auto& word : words) {
        std::cout << word << " ";
    }
    std::cout << std::endl;
    
    // 6. 迭代器使用
    std::cout << "\n6. 迭代器使用:" << std::endl;
    std::cout << "Forward iteration: ";
    for (auto it = words.begin(); it != words.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Reverse iteration: ";
    for (auto it = words.rbegin(); it != words.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // 7. 比较操作
    std::cout << "\n7. 比较操作:" << std::endl;
    MySTL::vector<int> a{1, 2, 3};
    MySTL::vector<int> b{1, 2, 3};
    MySTL::vector<int> c{1, 2, 4};
    
    std::cout << "a == b: " << (a == b ? "true" : "false") << std::endl;
    std::cout << "a == c: " << (a == c ? "true" : "false") << std::endl;
    std::cout << "a < c: " << (a < c ? "true" : "false") << std::endl;
    
    // 8. 交换
    std::cout << "\n8. 交换操作:" << std::endl;
    MySTL::vector<int> x{10, 20};
    MySTL::vector<int> y{30, 40, 50};
    
    std::cout << "Before swap - x: ";
    for (const auto& val : x) std::cout << val << " ";
    std::cout << ", y: ";
    for (const auto& val : y) std::cout << val << " ";
    std::cout << std::endl;
    
    x.swap(y);
    
    std::cout << "After swap - x: ";
    for (const auto& val : x) std::cout << val << " ";
    std::cout << ", y: ";
    for (const auto& val : y) std::cout << val << " ";
    std::cout << std::endl;
    
    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    
    return 0;
}