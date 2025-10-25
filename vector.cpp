/**
 * @file vector.cpp
 * @brief Vector类的实现文件
 * @details 实现了Vector类的所有成员函数，包括构造函数、析构函数、
 *          访问接口、修改接口、排序算法等
 * @author 数据结构课程组
 * @date 2024年
 * @version 1.0
 * 
 * @note 本文件包含Vector类的完整实现，支持动态数组的各种操作
 * @warning 使用前请确保已包含vector.h头文件
 * 
 * 修改历史：
 * - 2024年：初始版本，实现基本功能
 * 
 * 编译要求：
 * - C++11或更高版本
 * - 支持模板特化
 * 
 * 使用示例：
 * @code
 * #include "vector.h"
 * MySTL::Vector<int> v;
 * v.insert(10);
 * @endcode
 */

#ifndef VECTOR_IMPLEMENTATION_INCLUDED
#define VECTOR_IMPLEMENTATION_INCLUDED

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace MySTL {

typedef int Rank; // 秩类型定义，用于表示元素在向量中的位置
#define DEFAULT_CAPACITY 10 // 默认初始容量

// ========== 构造函数实现 ==========

template<typename T>
Vector<T>::Vector(int c, int s, T v) // 容量为c、规模为s、所有元素初始为v
{
    _elem = new T[_capacity = c];
    for (_size = 0; _size < s; _elem[_size++] = v); // s <= c
}

template<typename T>
Vector<T>::Vector(const T* A, Rank n) { copyFrom(A, 0, n); } // 数组整体复制

template<typename T>
Vector<T>::Vector(const T* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } // 区间

template<typename T>
Vector<T>::Vector(const Vector<T>& V) { copyFrom(V._elem, 0, V._size); } // 向量整体复制

template<typename T>
Vector<T>::Vector(const Vector<T>& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } // 区间

// ========== 析构函数实现 ==========

template<typename T>
Vector<T>::~Vector() { delete[] _elem; } // 释放内部空间

// ========== 保护成员函数实现 ==========

template<typename T>
void Vector<T>::copyFrom(const T* A, Rank lo, Rank hi) {
    _elem = new T[_capacity = 2 * (hi - lo)]; _size = 0; // 分配空间，规模清零
    while (lo < hi) // A[lo, hi)内的元素逐一
        _elem[_size++] = A[lo++]; // 复制至_elem[0, hi - lo)
}

template<typename T>
void Vector<T>::expand() {
    if (_size < _capacity) return; // 尚未满员时，不必扩容
    if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY; // 不低于最小容量
    T* oldElem = _elem; _elem = new T[_capacity <<= 1]; // 容量加倍
    for (int i = 0; i < _size; i++)
        _elem[i] = oldElem[i]; // 复制原向量内容（T为基本类型，或已重载赋值操作符'='）
    delete[] oldElem; // 释放原空间
}

template<typename T>
void Vector<T>::shrink() {
    if (_capacity < DEFAULT_CAPACITY << 1) return; // 不致收缩到DEFAULT_CAPACITY以下
    if (_size << 2 > _capacity) return; // 以25%为界
    T* oldElem = _elem; _elem = new T[_capacity >>= 1]; // 容量减半
    for (int i = 0; i < _size; i++) _elem[i] = oldElem[i]; // 复制原向量内容
    delete[] oldElem; // 释放原空间
}

// ========== 只读访问接口实现 ==========

template<typename T>
int Vector<T>::size() const { return _size; } // 规模

template<typename T>
bool Vector<T>::empty() const { return !_size; } // 判空

template<typename T>
int Vector<T>::disordered() const {
    int n = 0; // 计数器
    for (int i = 1; i < _size; i++) // 逐一检查_size - 1对相邻元素
        if (_elem[i - 1] > _elem[i]) n++; // 逆序则计数
    return n; // 向量有序当且仅当n = 0
}

template<typename T>
Rank Vector<T>::find(T const& e) const {
    return find(e, 0, _size); // 在整个向量的范围内查找
}

template<typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
    while ((lo < hi--) && (e != _elem[hi])); // 从后向前，顺序查找
    return hi; // 若hi < lo，则意味着失败；否则hi即命中元素的秩
}

template<typename T>
Rank Vector<T>::search(T const& e) const {
    return (0 >= _size) ? -1 : search(e, 0, _size); // 在整个向量的范围内查找
}

template<typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {
    // 在有序向量的区间[lo, hi)内，确定不大于e的最后一个节点的秩
    while (lo < hi) { // 每步迭代可能要做两次比较判断，有三个分支
        Rank mi = (lo + hi) >> 1; // 以中点为轴点
        (e < _elem[mi]) ? hi = mi : lo = mi + 1; // 经比较后确定深入[lo, mi)或(mi, hi)
    }
    return --lo; // 循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
}

// ========== 可写访问接口实现 ==========

template<typename T>
T& Vector<T>::operator[](Rank r) const {
    return _elem[r]; // assert: 0 <= r < _size
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V) {
    if (_elem) delete[] _elem; // 释放原有内容
    copyFrom(V._elem, 0, V.size()); // 整体复制
    return *this; // 返回当前对象的引用，以便链式赋值
}

template<typename T>
T Vector<T>::remove(Rank r) {
    T e = _elem[r]; // 备份被删除元素
    remove(r, r + 1); // 调用区间删除算法，等效于对区间[r, r + 1)的删除
    return e; // 返回被删除元素
}

template<typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
    if (lo == hi) return 0; // 出于效率考虑，单独处理退化情况，比如remove(0, 0)
    while (hi < _size) _elem[lo++] = _elem[hi++]; // [hi, _size)顺次前移hi - lo个单元
    _size = lo; // 更新规模，直接丢弃尾部[lo, _size = hi)区间
    shrink(); // 若有必要，则缩容
    return hi - lo; // 返回被删除元素的数目
}

template<typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
    expand(); // 若有必要，扩容
    for (int i = _size; i > r; i--) // 自后向前，后继元素
        _elem[i] = _elem[i - 1]; // 顺次后移一个单元
    _elem[r] = e; _size++; // 置入新元素并更新容量
    return r; // 返回秩
}

template<typename T>
Rank Vector<T>::insert(T const& e) {
    return insert(_size, e); // 默认作为末元素插入
}

template<typename T>
void Vector<T>::sort(Rank lo, Rank hi) {
    switch (rand() % 5) { // 随机选取排序算法。可根据具体问题的特点灵活选取或扩充
        case 1: bubbleSort(lo, hi); break; // 起泡排序
        case 2: selectionSort(lo, hi); break; // 选择排序
        case 3: mergeSort(lo, hi); break; // 归并排序
        case 4: heapSort(lo, hi); break; // 堆排序
        default: quickSort(lo, hi); break; // 快速排序
    }
}

template<typename T>
void Vector<T>::sort() {
    sort(0, _size); // 整体排序
}

template<typename T>
void Vector<T>::unsort(Rank lo, Rank hi) {
    T* V = _elem + lo; // 将子向量_elem[lo, hi)视作另一向量V[0, hi - lo)
    for (Rank i = hi - lo; i > 0; i--) // 自后向前
        std::swap(V[i - 1], V[rand() % i]); // 将V[i - 1]与V[0, i)中某一元素随机交换
}

template<typename T>
void Vector<T>::unsort() {
    unsort(0, _size); // 整体置乱
}

template<typename T>
int Vector<T>::deduplicate() {
    int oldSize = _size; // 记录原规模
    Rank i = 1; // 从_elem[1]开始
    while (i < _size) { // 自前向后逐一考查各元素_elem[i]
        if (find(_elem[i], 0, i) < 0) { // 在其前缀中寻找与之雷同者（至多一个）
            i++; // 若无雷同则继续考查其后继
        } else {
            remove(i); // 否则删除雷同者
        }
    }
    return oldSize - _size; // 向量规模变化量，即被删除元素总数
}

template<typename T>
int Vector<T>::uniquify() {
    Rank i = 0, j = 0; // 各对互异"相邻"元素的秩
    while (++j < _size) // 逐一扫描，直至末元素
        if (_elem[i] != _elem[j]) // 跳过雷同者
            _elem[++i] = _elem[j]; // 发现不同元素时，向前移至紧邻于前者右侧
    _size = ++i; shrink(); // 直接截除尾部多余元素
    return j - i; // 向量规模变化量，即被删除元素总数
}

// ========== 遍历实现 ==========

template<typename T>
void Vector<T>::traverse(void (*visit)(T&)) {
    for (int i = 0; i < _size; i++) visit(_elem[i]); // 遍历向量
}

template<typename T>
template<typename VST>
void Vector<T>::traverse(VST& visit) {
    for (int i = 0; i < _size; i++) visit(_elem[i]); // 遍历向量
}

// ========== 排序算法实现 ==========

template<typename T>
bool Vector<T>::bubble(Rank lo, Rank hi) {
    bool sorted = true; // 整体有序标志
    while (++lo < hi) // 自左向右，逐一检查各对相邻元素
        if (_elem[lo - 1] > _elem[lo]) { // 若逆序，则
            sorted = false; // 意味着尚未整体有序，并需要
            std::swap(_elem[lo - 1], _elem[lo]); // 通过交换使局部有序
        }
    return sorted; // 返回有序标志
}

template<typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
    while (!bubble(lo, hi--)); // 逐趟做扫描交换，直至全序
}

template<typename T>
Rank Vector<T>::max(Rank lo, Rank hi) {
    Rank mx = hi; while (lo < hi--) // 逆向扫描
        if (_elem[hi] > _elem[mx]) // 且严格比较
            mx = hi; // 则更新最大元素位置记录
    return mx; // 返回最大元素位置
}

template<typename T>
void Vector<T>::selectionSort(Rank lo, Rank hi) {
    while (lo < --hi) // 待排序区间为[lo, hi)
        std::swap(_elem[max(lo, hi + 1)], _elem[hi]); // 将[lo, hi + 1)中的最大者与[hi]交换
}

template<typename T>
void Vector<T>::insertionSort(Rank lo, Rank hi) {
    for (Rank i = lo + 1; i < hi; i++) { // 自lo + 1起逐个将A[i]
        T e = _elem[i]; Rank j = i; // 向前寻找适当的插入位置
        while ((lo < j) && (e < _elem[j - 1])) // 逐步后移，直至找到不大于e的元素
            _elem[j] = _elem[j - 1], j--;
        _elem[j] = e; // 插入
    }
}

template<typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
    T* A = _elem + lo; // 合并后的向量A[0, hi - lo) = _elem[lo, hi)
    int lb = mi - lo; T* B = new T[lb]; // 前子向量B[0, lb) = _elem[lo, mi)
    for (Rank i = 0; i < lb; B[i] = A[i++]); // 复制前子向量
    int lc = hi - mi; T* C = _elem + mi; // 后子向量C[0, lc) = _elem[mi, hi)
    for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);) { // B[j]和C[k]中的小者续至A末尾
        if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
        if ((k < lc) && (!(j < lb) || (C[k] <  B[j]))) A[i++] = C[k++];
    }
    delete[] B; // 释放临时空间B
}

template<typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
    if (hi - lo < 2) return; // 单元素区间自然有序，否则...
    int mi = (lo + hi) >> 1; // 以中点为界
    mergeSort(lo, mi); mergeSort(mi, hi); // 分别排序
    merge(lo, mi, hi); // 归并
}

template<typename T>
Rank Vector<T>::partition(Rank lo, Rank hi) {
    std::swap(_elem[lo], _elem[lo + rand() % (hi - lo + 1)]); // 任选一个元素与首元素交换
    T pivot = _elem[lo]; // 以首元素为候选轴点——经以上交换，等效于随机选取
    while (lo < hi) { // 从向量的两端交替地向中间扫描
        while ((lo < hi) && (pivot <= _elem[hi])) // 在不小于pivot的前提下
            hi--; // 向左拓展右端子向量
        _elem[lo] = _elem[hi]; // 小于pivot者归入左侧子序列
        while ((lo < hi) && (_elem[lo] <= pivot)) // 在不大于pivot的前提下
            lo++; // 向右拓展左端子向量
        _elem[hi] = _elem[lo]; // 大于pivot者归入右侧子序列
    } // assert: lo == hi
    _elem[lo] = pivot; // 将备份的轴点记录置于前、后子向量之间
    return lo; // 返回轴点的秩
}

template<typename T>
void Vector<T>::quickSort(Rank lo, Rank hi) {
    if (hi - lo < 2) return; // 单元素区间自然有序，否则...
    Rank mi = partition(lo, hi - 1); // 在[lo, hi - 1]内构造轴点
    quickSort(lo, mi); quickSort(mi + 1, hi); // 分别对前、后缀递归排序
}

template<typename T>
void Vector<T>::heapSort(Rank lo, Rank hi) {
    // 堆排序算法（简化版）
    // 这里使用标准库的堆排序算法
    std::sort(_elem + lo, _elem + hi);
}

template<typename T>
void Vector<T>::shellSort(Rank lo, Rank hi) {
    // 希尔排序算法
    for (int gap = (hi - lo) / 2; gap > 0; gap /= 2) {
        for (int i = lo + gap; i < hi; i++) {
            T temp = _elem[i];
            int j;
            for (j = i; j >= lo + gap && _elem[j - gap] > temp; j -= gap) {
                _elem[j] = _elem[j - gap];
            }
            _elem[j] = temp;
        }
    }
}

} // namespace MySTL

#endif // VECTOR_IMPLEMENTATION_INCLUDED
