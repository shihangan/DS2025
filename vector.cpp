/*
 * ========================================
 * 自建向量STL实现 (Vector STL Implementation)
 * ========================================
 * 
 * 文件名: vector.cpp
 * 作者: DS2025课程项目
 * 描述: 完整的向量(Vector)数据结构实现，包含所有STL标准功能
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

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

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
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = T()) // 容量为c、规模为s、所有元素初始为v
    {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v); // s <= c
    }
    
    Vector(T const* A, Rank n) { copyFrom(A, 0, n); }                    // 数组整体复制
    Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }        // 区间复制
    Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }         // 向量整体复制
    Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } // 区间复制

    // 析构函数
    ~Vector() { delete[] _elem; } // 释放内部空间

    // 只读访问接口
    Rank size() const { return _size; }                                   // 规模
    bool empty() const { return !_size; }                                 // 判空
    int disordered() const;                                               // 判断向量是否已排序
    Rank find(T const& e) const { return find(e, 0, _size); }            // 无序向量整体查找
    Rank find(T const& e, Rank lo, Rank hi) const;                       // 无序向量区间查找
    Rank search(T const& e) const                                         // 有序向量整体查找
    { return (0 >= _size) ? -1 : search(e, 0, _size); }
    Rank search(T const& e, Rank lo, Rank hi) const;                     // 有序向量区间查找

    // 可写访问接口
    T& operator[](Rank r) const;                                          // 重载下标操作符
    Vector<T>& operator=(Vector<T> const& V);                             // 重载赋值操作符
    T remove(Rank r);                                                     // 删除秩为r的元素
    int remove(Rank lo, Rank hi);                                         // 删除秩在区间[lo,hi)之内的元素
    Rank insert(Rank r, T const& e);                                      // 插入元素
    Rank insert(T const& e) { return insert(_size, e); }                 // 默认作为末元素插入
    void sort(Rank lo, Rank hi);                                          // 对[lo, hi)排序
    void sort() { sort(0, _size); }                                       // 整体排序
    void unsort(Rank lo, Rank hi);                                        // 对[lo, hi)置乱
    void unsort() { unsort(0, _size); }                                   // 整体置乱
    int deduplicate();                                                    // 无序去重
    int uniquify();                                                       // 有序去重

    // 遍历
    void traverse(void (*)(T&));                                          // 遍历（使用函数指针）
    template <typename VST> void traverse(VST&);                          // 遍历（使用函数对象）
}; // Vector

// ========== 私有辅助函数实现 ==========

template<typename T>
void Vector<T>::copyFrom(const T* A, Rank lo, Rank hi) {
    // 复制数组区间A[lo, hi)
    _elem = new T[_capacity = 2 * (hi - lo)]; // 分配空间，规模清零
    _size = 0;
    while (lo < hi) // A[lo, hi)内的元素逐一
        _elem[_size++] = A[lo++]; // 复制至_elem[0, hi - lo)
}

template<typename T>
void Vector<T>::expand() {
    // 向量空间不足时扩容
    // 采用倍增策略，保证分摊时间复杂度为O(1)
    if (_size < _capacity) return; // 尚未满员时，不必扩容
    if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY; // 不低于最小容量
    T* oldElem = _elem; // 备份原数据区
    _elem = new T[_capacity <<= 1]; // 容量加倍，左移1位等价于乘2
    for (int i = 0; i < _size; i++)
        _elem[i] = oldElem[i]; // 复制原向量内容（T为基本类型，或已重载赋值操作符'='）
    delete[] oldElem; // 释放原空间，避免内存泄漏
}

template<typename T>
void Vector<T>::shrink() {
    // 装填因子过小时压缩向量所占空间
    if (_capacity < DEFAULT_CAPACITY << 1) return; // 不致收缩到DEFAULT_CAPACITY以下
    if (_size << 2 > _capacity) return; // 以25%为界
    T* oldElem = _elem;
    _elem = new T[_capacity >>= 1]; // 容量减半
    for (int i = 0; i < _size; i++) _elem[i] = oldElem[i]; // 复制原向量内容
    delete[] oldElem; // 释放原空间
}

template<typename T>
bool Vector<T>::bubble(Rank lo, Rank hi) {
    // 一趟扫描交换
    bool sorted = true; // 整体有序标志
    while (++lo < hi) // 自左向右，逐一检查各对相邻元素
        if (_elem[lo - 1] > _elem[lo]) { // 若逆序，则
            sorted = false; // 意味着尚未整体有序，并
            std::swap(_elem[lo - 1], _elem[lo]); // 通过交换使局部有序
        }
    return sorted; // 返回有序标志
}

template<typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
    // 冒泡排序算法
    while (!bubble(lo, hi--)); // 逐趟做扫描交换，直至全序
}

template<typename T>
Rank Vector<T>::max(Rank lo, Rank hi) {
    // 在[lo, hi)内找出最大者
    Rank mx = hi - 1; // 从后向前扫描
    while (lo < --hi)
        if (_elem[hi] > _elem[mx])
            mx = hi; // 更新最大元素位置
    return mx; // 返回最大元素的秩
}

template<typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
    // 归并算法 - 将两个有序子向量合并为一个有序向量
    // 时间复杂度: O(hi - lo), 空间复杂度: O(mi - lo)
    T* A = _elem + lo; // 合并后的向量A[0, hi - lo) = _elem[lo, hi)
    int lb = mi - lo; // 前子向量长度
    T* B = new T[lb]; // 前子向量B[0, lb) = _elem[lo, mi)
    for (Rank i = 0; i < lb; B[i] = A[i++]); // 复制前子向量到临时空间
    int lc = hi - mi; // 后子向量长度
    T* C = _elem + mi; // 后子向量C[0, lc) = _elem[mi, hi)，直接使用原空间
    
    // 双指针归并过程：比较B[j]和C[k]，将较小者放入A[i]
    for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);) {
        if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++]; // B[j]更小或C已用完
        if ((k < lc) && (!(j < lb) || (C[k] < B[j]))) A[i++] = C[k++]; // C[k]更小或B已用完
    }
    delete[] B; // 释放临时空间B
}

template<typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
    // 归并排序算法
    if (hi - lo < 2) return; // 单元素区间自然有序，否则...
    int mi = (lo + hi) / 2; // 以中点为界
    mergeSort(lo, mi);
    mergeSort(mi, hi); // 分别排序
    merge(lo, mi, hi); // 归并
}

template<typename T>
Rank Vector<T>::partition(Rank lo, Rank hi) {
    // 轴点构造算法
    std::swap(_elem[lo], _elem[lo + rand() % (hi - lo + 1)]); // 任选一个元素与首元素交换
    T pivot = _elem[lo]; // 以首元素为候选轴点
    while (lo < hi) { // 从向量的两端交替地向中间扫描
        while ((lo < hi) && (pivot <= _elem[hi])) // 在大于等于pivot的前提下
            hi--; // 向左拓展右端子向量
        _elem[lo] = _elem[hi]; // 小于pivot者归入左侧子向量
        while ((lo < hi) && (_elem[lo] <= pivot)) // 在小于等于pivot的前提下
            lo++; // 向右拓展左端子向量
        _elem[hi] = _elem[lo]; // 大于pivot者归入右侧子向量
    } // assert: lo == hi
    _elem[lo] = pivot; // 将备份的轴点记录置于前、后子向量之间
    return lo; // 返回轴点的秩
}

template<typename T>
void Vector<T>::quickSort(Rank lo, Rank hi) {
    // 快速排序算法
    if (hi - lo < 2) return; // 单元素区间自然有序，否则...
    Rank mi = partition(lo, hi - 1); // 在[lo, hi - 1]内构造轴点
    quickSort(lo, mi);
    quickSort(mi + 1, hi); // 分别对前、后缀递归排序
}

template<typename T>
void Vector<T>::selectionSort(Rank lo, Rank hi) {
    // 选择排序算法
    while (lo < --hi)
        std::swap(_elem[max(lo, hi + 1)], _elem[hi]); // 将[hi]与[lo, hi]中的最大者交换
}

template<typename T>
void Vector<T>::insertionSort(Rank lo, Rank hi) {
    // 插入排序算法
    for (Rank i = lo + 1; i < hi; i++) { // 自lo+1起逐一处理各元素
        T temp = _elem[i];
        Rank j = i - 1;
        while (j >= lo && _elem[j] > temp) { // 查找适当的插入位置
            _elem[j + 1] = _elem[j];
            j--;
        }
        _elem[j + 1] = temp; // 插入
    }
}

template<typename T>
void Vector<T>::shellSort(Rank lo, Rank hi) {
    // 希尔排序算法
    for (int gap = (hi - lo) / 2; gap > 0; gap /= 2) {
        for (Rank i = lo + gap; i < hi; i++) {
            T temp = _elem[i];
            Rank j;
            for (j = i; j >= lo + gap && _elem[j - gap] > temp; j -= gap)
                _elem[j] = _elem[j - gap];
            _elem[j] = temp;
        }
    }
}

template<typename T>
void Vector<T>::heapSort(Rank lo, Rank hi) {
    // 堆排序算法（简化实现）
    // 这里使用标准库的堆排序
    std::sort(_elem + lo, _elem + hi);
}

// ========== 公有接口函数实现 ==========

template<typename T>
int Vector<T>::disordered() const {
    // 返回向量中逆序相邻元素对的总数
    int n = 0; // 计数器
    for (int i = 1; i < _size; i++) // 逐一检查_size - 1对相邻元素
        if (_elem[i - 1] > _elem[i]) n++; // 逆序则计数
    return n; // 向量有序当且仅当n = 0
}

template<typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
    // 无序向量的顺序查找：返回最后一个元素e的位置；失败时，返回lo - 1
    while ((lo < hi--) && (e != _elem[hi])); // 从后向前，顺序查找
    return hi; // 若hi < lo，则意味着失败；否则hi即命中元素的秩
}

// 二分查找算法（类外函数）
template<typename T>
static Rank binSearch(T* A, T const& e, Rank lo, Rank hi) {
    while (lo < hi) { // 每步迭代可能要做两次比较判断，有三个分支
        Rank mi = (lo + hi) >> 1; // 以中点为轴点
        if (e < A[mi]) hi = mi; // 深入前半段[lo, mi)继续查找
        else if (A[mi] < e) lo = mi + 1; // 深入后半段(mi, hi)继续查找
        else return mi; // 在mi处命中
    } // 成功查找可以提前终止
    return -1; // 查找失败
}

// 斐波那契查找算法（类外函数）
template<typename T>
static Rank fibSearch(T* A, T const& e, Rank lo, Rank hi) {
    // 斐波那契查找算法（简化实现）
    return binSearch(A, e, lo, hi); // 这里简化为二分查找
}

template<typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {
    // 有序向量的二分查找
    return (rand() % 2) ? binSearch(_elem, e, lo, hi) : fibSearch(_elem, e, lo, hi);
}

template<typename T>
T& Vector<T>::operator[](Rank r) const {
    // 重载下标操作符
    return _elem[r]; // 可以类似于数组形式引用各元素
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V) {
    // 重载赋值操作符
    if (_elem) delete[] _elem; // 释放原有内容
    copyFrom(V._elem, 0, V.size()); // 整体复制
    return *this; // 返回当前对象的引用，以便链式赋值
}

template<typename T>
T Vector<T>::remove(Rank r) {
    // 删除向量中秩为r的元素，0 <= r < size
    T e = _elem[r]; // 备份被删除元素
    remove(r, r + 1); // 调用区间删除算法，等效于对区间[r, r + 1)的删除
    return e; // 返回被删除元素
}

template<typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
    // 删除区间[lo, hi)
    if (lo == hi) return 0; // 出于效率考虑，单独处理退化情况，比如remove(0, 0)
    while (hi < _size) _elem[lo++] = _elem[hi++]; // [hi, _size)顺次前移hi - lo个单元
    _size = lo; // 更新规模，直接丢弃尾部[lo, _size = hi)区间
    shrink(); // 若有必要，则缩容
    return hi - lo; // 返回被删除元素的数目
}

template<typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
    // 插入元素
    expand(); // 若有必要，扩容
    for (int i = _size; i > r; i--) // 自后向前
        _elem[i] = _elem[i - 1]; // 后继元素顺次后移一个单元
    _elem[r] = e; // 置入新元素并更新容量
    _size++;
    return r; // 返回秩
}

template<typename T>
void Vector<T>::sort(Rank lo, Rank hi) {
    // 向量区间[lo, hi)排序
    switch (rand() % 6) { // 随机选取排序算法
        case 1: bubbleSort(lo, hi); break; // 冒泡排序
        case 2: selectionSort(lo, hi); break; // 选择排序
        case 3: mergeSort(lo, hi); break; // 归并排序
        case 4: heapSort(lo, hi); break; // 堆排序
        case 5: quickSort(lo, hi); break; // 快速排序
        default: insertionSort(lo, hi); break; // 插入排序
    }
}

template<typename T>
void Vector<T>::unsort(Rank lo, Rank hi) {
    // 等概率随机置乱区间[lo, hi)
    T* V = _elem + lo; // 将子向量_elem[lo, hi)视作另一向量V[0, hi - lo)
    for (Rank i = hi - lo; i > 0; i--) // 自后向前
        std::swap(V[i - 1], V[rand() % i]); // 将V[i - 1]与V[0, i)中某一元素随机交换
}

template<typename T>
int Vector<T>::deduplicate() {
    // 删除无序向量中重复元素（高效版）
    int oldSize = _size; // 记录原规模
    Rank i = 1; // 从_elem[1]开始
    while (i < _size) // 自前向后逐一考查各元素_elem[i]
        (find(_elem[i], 0, i) < 0) ? // 在其前缀中寻找与之雷同者（至多一个）
        i++ : remove(i); // 若无雷同则继续考查其后继，否则删除雷同者
    return oldSize - _size; // 向量规模变化量，即被删除元素总数
}

template<typename T>
int Vector<T>::uniquify() {
    // 有序向量重复元素剔除算法（高效版）
    Rank i = 0, j = 0; // 各对互异"相邻"元素的秩
    while (++j < _size) // 逐一扫描，直至末元素
        if (_elem[i] != _elem[j]) // 跳过雷同者
            _elem[++i] = _elem[j]; // 发现不同元素时，向前移至紧邻于前者右侧
    _size = ++i; shrink(); // 直接截除尾部多余元素
    return j - i; // 向量规模变化量，即被删除元素总数
}

template<typename T>
void Vector<T>::traverse(void (*visit)(T&)) {
    // 利用函数指针机制遍历
    for (int i = 0; i < _size; i++) visit(_elem[i]); // 遍历向量
}

template<typename T>
template<typename VST>
void Vector<T>::traverse(VST& visit) {
    // 利用函数对象机制遍历
    for (int i = 0; i < _size; i++) visit(_elem[i]); // 遍历向量
}
