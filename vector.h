/*
 * ========================================
 * 自建向量STL实现 (Vector STL Implementation)
 * ========================================
 * 
 * 文件名: vector.h
 * 作者: DS2025课程项目
 * 描述: 完整的向量(Vector)数据结构声明，包含所有STL标准功能
 * 
 * 主要功能:
 * 1. 动态内存管理 - 自动扩容和缩容
 * 2. 多种构造方式 - 默认、拷贝、数组、区间构造
 * 3. 元素访问 - 下标操作符、安全访问
 * 4. 插入删除 - 任意位置插入删除元素
 * 5. 查找功能 - 无序查找、有序二分查找
 * 6. 排序算法 - 冒泡、选择、插入、归并、快速、堆排序等
 * 7. 去重功能 - 无序去重、有序去重
 * 8. 遍历功能 - 函数指针、函数对象遍历
 * 9. 实用工具 - 置乱、判断有序性等
 * 
 * 时间复杂度:
 * - 访问: O(1)
 * - 插入/删除: O(n) 
 * - 查找: O(n) 无序, O(log n) 有序
 * - 排序: O(n log n) 平均情况
 * 
 * 空间复杂度: O(n)
 * ========================================
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

namespace MySTL {

typedef int Rank; // 秩类型定义，用于表示元素在向量中的位置
#define DEFAULT_CAPACITY 10 // 默认初始容量

template<typename T>
class Vector { // 向量模板类
protected:
    Rank _size;      // 规模（当前元素个数）
    int _capacity;   // 容量（最大可容纳元素个数）
    T* _elem;        // 数据区指针

    // 私有辅助函数
    void copyFrom(const T* A, Rank lo, Rank hi); // 复制数组区间A[lo,hi)
    void expand();                                // 扩展容量
    void shrink();                               // 压缩容量
    bool bubble(Rank lo, Rank hi);               // 扫描交换（冒泡排序的一趟）
    void bubbleSort(Rank lo, Rank hi);           // 冒泡排序
    Rank max(Rank lo, Rank hi);                  // 选取最大元素
    void merge(Rank lo, Rank mi, Rank hi);       // 归并算法
    void mergeSort(Rank lo, Rank hi);            // 归并排序
    void heapSort(Rank lo, Rank hi);             // 堆排序
    void quickSort(Rank lo, Rank hi);            // 快速排序
    void selectionSort(Rank lo, Rank hi);        // 选择排序
    void insertionSort(Rank lo, Rank hi);        // 插入排序
    void shellSort(Rank lo, Rank hi);            // 希尔排序
    Rank partition(Rank lo, Rank hi);            // 轴点构造算法

public:
    // 构造函数
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = T()); // 容量为c、规模为s、所有元素初始为v
    Vector(T const* A, Rank n);                    // 数组整体复制
    Vector(T const* A, Rank lo, Rank hi);        // 区间复制
    Vector(Vector<T> const& V);         // 向量整体复制
    Vector(Vector<T> const& V, Rank lo, Rank hi); // 区间复制

    // 析构函数
    ~Vector(); // 释放内部空间

    // 只读访问接口
    Rank size() const; // 规模
    bool empty() const; // 判空
    int disordered() const; // 判断向量是否已排序
    Rank find(T const& e) const; // 无序向量整体查找
    Rank find(T const& e, Rank lo, Rank hi) const; // 无序向量区间查找
    Rank search(T const& e) const; // 有序向量整体查找
    Rank search(T const& e, Rank lo, Rank hi) const; // 有序向量区间查找

    // 可写访问接口
    T& operator[](Rank r) const; // 重载下标操作符
    Vector<T>& operator=(Vector<T> const& V); // 重载赋值操作符
    T remove(Rank r); // 删除秩为r的元素
    int remove(Rank lo, Rank hi); // 删除秩在区间[lo,hi)之内的元素
    Rank insert(Rank r, T const& e); // 插入元素
    Rank insert(T const& e); // 默认作为末元素插入
    void sort(Rank lo, Rank hi); // 对[lo, hi)排序
    void sort(); // 整体排序
    void unsort(Rank lo, Rank hi); // 对[lo, hi)置乱
    void unsort(); // 整体置乱
    int deduplicate(); // 无序去重
    int uniquify(); // 有序去重

    // 遍历
    void traverse(void (*)(T&)); // 遍历（使用函数指针）
    template <typename VST> void traverse(VST&); // 遍历（使用函数对象）
};

} // namespace MySTL

#ifndef VECTOR_IMPLEMENTATION_INCLUDED
#define VECTOR_IMPLEMENTATION_INCLUDED
#include "vector.cpp" // 包含实现文件
#endif

#endif // VECTOR_H