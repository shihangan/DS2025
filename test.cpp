/**
 * @file test.cpp
 * @brief MySTL 通用测试文件
 * @author DS2025
 * @date 2025
 * 
 * 用于测试各种数据结构的实现
 */

#include <iostream>
#include <string>
#include "mystl.h"

using namespace std;

// 测试函数声明
void testStack();
void testStackBasicOperations();
void testStackCopyAndAssignment();
void testStackTraversal();

// 辅助函数：打印分隔线
void printSeparator(const string& title) {
    cout << "\n========== " << title << " ==========" << endl;
}

// 访问函数用于遍历
void printInt(const int& x) {
    cout << x << " ";
}

void printString(const string& s) {
    cout << s << " ";
}

int main() {
    cout << "MySTL 数据结构测试程序" << endl;
    cout << "========================" << endl;
    
    testStack();
    
    cout << "\n所有测试完成！" << endl;
    return 0;
}

/**
 * @brief 栈的综合测试
 */
void testStack() {
    printSeparator("栈（Stack）测试");
    
    testStackBasicOperations();
    testStackCopyAndAssignment();
    testStackTraversal();
}

/**
 * @brief 测试栈的基本操作
 */
void testStackBasicOperations() {
    cout << "\n--- 基本操作测试 ---" << endl;
    
    // 创建整数栈
    Stack<int> intStack;
    
    // 测试空栈
    cout << "新建栈是否为空: " << (intStack.empty() ? "是" : "否") << endl;
    cout << "栈大小: " << intStack.size() << endl;
    
    // 测试push操作
    cout << "\n压入元素: 1, 2, 3, 4, 5" << endl;
    for (int i = 1; i <= 5; i++) {
        intStack.push(i);
        cout << "压入 " << i << ", 栈大小: " << intStack.size() << ", 栈顶: " << intStack.top() << endl;
    }
    
    // 测试栈状态
    cout << "\n当前栈是否为空: " << (intStack.empty() ? "是" : "否") << endl;
    cout << "栈大小: " << intStack.size() << endl;
    cout << "栈顶元素: " << intStack.top() << endl;
    
    // 测试pop操作
    cout << "\n弹出所有元素:" << endl;
    while (!intStack.empty()) {
        int top = intStack.top();
        int popped = intStack.pop();
        cout << "弹出: " << popped << ", 剩余大小: " << intStack.size();
        if (!intStack.empty()) {
            cout << ", 新栈顶: " << intStack.top();
        }
        cout << endl;
    }
    
    cout << "栈是否为空: " << (intStack.empty() ? "是" : "否") << endl;
}

/**
 * @brief 测试栈的拷贝和赋值
 */
void testStackCopyAndAssignment() {
    cout << "\n--- 拷贝和赋值测试 ---" << endl;
    
    // 创建字符串栈
    Stack<string> originalStack;
    originalStack.push("Hello");
    originalStack.push("World");
    originalStack.push("Stack");
    
    cout << "原始栈大小: " << originalStack.size() << endl;
    cout << "原始栈顶: " << originalStack.top() << endl;
    
    // 测试拷贝构造函数
    Stack<string> copiedStack(originalStack);
    cout << "\n拷贝构造后:" << endl;
    cout << "拷贝栈大小: " << copiedStack.size() << endl;
    cout << "拷贝栈顶: " << copiedStack.top() << endl;
    
    // 测试赋值操作符
    Stack<string> assignedStack;
    assignedStack = originalStack;
    cout << "\n赋值操作后:" << endl;
    cout << "赋值栈大小: " << assignedStack.size() << endl;
    cout << "赋值栈顶: " << assignedStack.top() << endl;
    
    // 验证独立性
    originalStack.pop();
    cout << "\n修改原始栈后:" << endl;
    cout << "原始栈大小: " << originalStack.size() << ", 栈顶: " << originalStack.top() << endl;
    cout << "拷贝栈大小: " << copiedStack.size() << ", 栈顶: " << copiedStack.top() << endl;
    cout << "赋值栈大小: " << assignedStack.size() << ", 栈顶: " << assignedStack.top() << endl;
}

/**
 * @brief 测试栈的遍历功能
 */
void testStackTraversal() {
    cout << "\n--- 遍历测试 ---" << endl;
    
    // 创建整数栈
    Stack<int> intStack;
    cout << "压入元素: ";
    for (int i = 10; i <= 50; i += 10) {
        intStack.push(i);
        cout << i << " ";
    }
    cout << endl;
    
    cout << "栈中元素（从栈底到栈顶）: ";
    intStack.traverse(printInt);
    cout << endl;
    
    // 创建字符串栈
    Stack<string> stringStack;
    stringStack.push("First");
    stringStack.push("Second");
    stringStack.push("Third");
    
    cout << "\n字符串栈中元素（从栈底到栈顶）: ";
    stringStack.traverse(printString);
    cout << endl;
    
    // 测试clear操作
    cout << "\n清空栈前大小: " << intStack.size() << endl;
    intStack.clear();
    cout << "清空栈后大小: " << intStack.size() << endl;
    cout << "栈是否为空: " << (intStack.empty() ? "是" : "否") << endl;
}