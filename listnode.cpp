/**
 * @file listnode.cpp
 * @brief 链表节点类的完整实现文件
 * @author DS2025
 * @date 2025
 * 
 * 定义并实现了ListNode和List类的ADT接口
 * 
 * 功能描述：
 * 1. 双向链表节点的基本数据结构
 * 2. 链表的构造、析构、复制功能
 * 3. 元素的插入、删除、查找操作
 * 4. 多种排序算法 - 归并排序、选择排序、插入排序
 * 5. 去重功能 - 无序去重、有序去重
 * 6. 遍历功能 - 函数指针、函数对象遍历
 * 7. 实用工具 - 倒置、归并等
 * 
 * 时间复杂度：
 * - 插入/删除：O(1)
 * - 查找：O(n)
 * - 排序：O(nlogn) - 归并排序
 * - 去重（有序）：O(n)
 * - 去重（无序）：O(n²)
 * 
 * 空间复杂度：O(1) - 除排序递归栈外
 */

#include <iostream>
#include <string>
#include <algorithm>

// 前向声明
template <typename T> class List;

/**
 * @brief 列表节点模板类（以双向链表形式实现）
 * @tparam T 数据类型
 * 
 * 数据成员：
 * - data: 数值
 * - pred: 前驱节点指针
 * - succ: 后继节点指针
 * 
 * 核心操作：
 * - insertAsSucc(): 作为当前节点的后继插入新节点
 * - insertAsPred(): 作为当前节点的前驱插入新节点
 */
template <typename T>
class ListNode {
    // List类需要访问ListNode的私有成员
    friend class List<T>;

private:
    T data;                         // 数值
    ListNode<T>* pred;              // 前驱
    ListNode<T>* succ;              // 后继

public:
    // 构造函数
    ListNode() {}                   // 针对header和trailer的构造
    ListNode(T e, ListNode<T>* p = nullptr, ListNode<T>* s = nullptr)
        : data(e), pred(p), succ(s) {}  // 默认构造器

    // 操作接口
    /**
     * @brief 作为当前节点的后继插入新节点
     * @param e 要插入的数据
     * @return 返回新插入的节点指针
     * @time O(1)
     */
    ListNode<T>* insertAsSucc(T const& e);

    /**
     * @brief 作为当前节点的前驱插入新节点
     * @param e 要插入的数据
     * @return 返回新插入的节点指针
     * @time O(1)
     */
    ListNode<T>* insertAsPred(T const& e);
};

/**
 * @brief 列表模板类（以双向链表形式实现）
 * @tparam T 数据类型
 * 
 * 数据成员：
 * - _size: 规模
 * - header: 头哨兵
 * - trailer: 尾哨兵
 * 
 * 核心特性：
 * 1. 使用头尾哨兵节点简化边界处理
 * 2. 支持泛型数据类型
 * 3. 提供丰富的操作接口
 * 4. 实现多种排序算法
 * 5. 支持函数式编程风格的遍历
 */
template <typename T>
class List {
private:
    int _size;                      // 规模
    ListNode<T>* header;            // 头哨兵
    ListNode<T>* trailer;           // 尾哨兵

protected:
    /**
     * @brief 列表创建时的初始化
     * @time O(1)
     */
    void init();

    /**
     * @brief 清除所有节点
     * @return 被清除节点的数目
     * @time O(n)
     */
    int clear();

    /**
     * @brief 复制列表中自位置p起的n项
     * @param p 起始位置
     * @param n 项数
     * @time O(n)
     */
    void copyNodes(ListNode<T>* p, int n);

    /**
     * @brief 归并操作：当前列表中自p起的n个元素，与列表L中自q起的m个元素归并
     * @param p 当前列表起始位置
     * @param n 当前列表元素个数
     * @param L 另一个列表
     * @param q 另一个列表起始位置
     * @param m 另一个列表元素个数
     * @time O(n+m)
     */
    void merge(ListNode<T>*& p, int n, List<T>& L, ListNode<T>* q, int m);

    /**
     * @brief 列表的归并排序算法：对起始于位置p的n个元素排序
     * @param p 起始位置
     * @param n 元素个数
     * @time O(nlogn)
     * @space O(logn) - 递归栈空间
     */
    void mergeSort(ListNode<T>*& p, int n);

    /**
     * @brief 列表的选择排序算法：对起始于位置p的n个元素排序
     * @param p 起始位置
     * @param n 元素个数
     * @time O(n^2)
     */
    void selectionSort(ListNode<T>* p, int n);

    /**
     * @brief 列表的插入排序算法：对起始于位置p的n个元素排序
     * @param p 起始位置
     * @param n 元素个数
     * @time O(n^2)
     */
    void insertionSort(ListNode<T>* p, int n);

public:
    // 构造函数
    /**
     * @brief 默认构造函数
     * @time O(1)
     */
    List() { init(); }

    /**
     * @brief 复制构造函数
     * @param L 要复制的列表
     * @time O(n)
     */
    List(List<T> const& L);

    /**
     * @brief 复制列表L中自第r项起的n项
     * @param L 源列表
     * @param r 起始位置
     * @param n 复制项数
     * @time O(n)
     */
    List(List<T> const& L, int r, int n);

    /**
     * @brief 复制列表中自位置p起的n项
     * @param p 起始节点
     * @param n 复制项数
     * @time O(n)
     */
    List(ListNode<T>* p, int n);

    /**
     * @brief 析构函数
     * @time O(n)
     */
    ~List();

    // 只读访问接口
    /**
     * @brief 获取列表规模
     * @return 列表中元素的个数
     * @time O(1)
     */
    int size() const { return _size; }

    /**
     * @brief 判断列表是否为空
     * @return 空返回true，否则返回false
     * @time O(1)
     */
    bool empty() const { return _size <= 0; }

    /**
     * @brief 重载下标操作符，支持循秩访问（效率低）
     * @param r 秩
     * @return 对应元素的引用
     * @time O(r)
     */
    T& operator[](int r) const;

    /**
     * @brief 获取首节点位置
     * @return 首节点指针
     * @time O(1)
     */
    ListNode<T>* first() const { return header->succ; }

    /**
     * @brief 获取末节点位置
     * @return 末节点指针
     * @time O(1)
     */
    ListNode<T>* last() const { return trailer->pred; }

    /**
     * @brief 判断位置p是否对外合法
     * @param p 节点指针
     * @return 合法返回true，否则返回false
     * @time O(1)
     */
    bool valid(ListNode<T>* p) const { return p && (trailer != p) && (header != p); }

    /**
     * @brief 判断列表是否已排序
     * @return 已排序返回true，否则返回false
     * @time O(n)
     */
    int disordered() const;

    /**
     * @brief 在无序列表内节点p（可能是trailer）的n个（真）前驱中，找到等于e的最后者
     * @param e 目标元素
     * @param n 查找范围
     * @param p 起始位置
     * @return 找到的节点指针，失败时返回nullptr
     * @time O(n)
     */
    ListNode<T>* find(T const& e, int n, ListNode<T>* p) const;

    /**
     * @brief 在无序列表中查找元素e
     * @param e 目标元素
     * @return 找到的节点指针，失败时返回nullptr
     * @time O(n)
     */
    ListNode<T>* find(T const& e) const { return find(e, _size, trailer); }

    /**
     * @brief 在有序列表内节点p（可能是trailer）的n个（真）前驱中，找到不大于e的最后者
     * @param e 目标元素
     * @param n 查找范围
     * @param p 起始位置
     * @return 找到的节点指针
     * @time O(n)
     */
    ListNode<T>* search(T const& e, int n, ListNode<T>* p) const;

    /**
     * @brief 在有序列表中查找元素e
     * @param e 目标元素
     * @return 找到的节点指针
     * @time O(n)
     */
    ListNode<T>* search(T const& e) const { return search(e, _size, trailer); }

    /**
     * @brief 在p及其n-1个后继中选出最大者
     * @param p 起始位置
     * @param n 范围
     * @return 最大元素的节点指针
     * @time O(n)
     */
    ListNode<T>* selectMax(ListNode<T>* p, int n);

    /**
     * @brief 整体最大者
     * @return 最大元素的节点指针
     * @time O(n)
     */
    ListNode<T>* selectMax() { return selectMax(header->succ, _size); }

    // 可写访问接口
    /**
     * @brief 将e当作首节点插入
     * @param e 要插入的元素
     * @return 新插入的节点指针
     * @time O(1)
     */
    ListNode<T>* insertAsFirst(T const& e);

    /**
     * @brief 将e当作末节点插入
     * @param e 要插入的元素
     * @return 新插入的节点指针
     * @time O(1)
     */
    ListNode<T>* insertAsLast(T const& e);

    /**
     * @brief 将e当作p的前驱插入
     * @param p 目标位置
     * @param e 要插入的元素
     * @return 新插入的节点指针
     * @time O(1)
     */
    ListNode<T>* insertBefore(ListNode<T>* p, T const& e);

    /**
     * @brief 将e当作p的后继插入
     * @param p 目标位置
     * @param e 要插入的元素
     * @return 新插入的节点指针
     * @time O(1)
     */
    ListNode<T>* insertAfter(ListNode<T>* p, T const& e);

    /**
     * @brief 删除合法位置p处的节点，返回被删除节点
     * @param p 要删除的节点
     * @return 被删除节点的数据
     * @time O(1)
     */
    T remove(ListNode<T>* p);

    /**
     * @brief 对列表区间排序
     * @param p 起始位置
     * @param n 元素个数
     * @time O(nlogn)
     */
    void sort(ListNode<T>* p, int n);

    /**
     * @brief 整体排序
     * @time O(nlogn)
     */
    void sort() { sort(first(), _size); }

    /**
     * @brief 无序去重：删除无序列表中重复节点（效率低）
     * @return 被删除节点总数
     * @time O(n^2)
     */
    int deduplicate();

    /**
     * @brief 有序去重：删除有序列表中重复节点
     * @return 被删除节点总数
     * @time O(n)
     */
    int uniquify();

    /**
     * @brief 前后倒置
     * @time O(n)
     */
    void reverse();

    // 遍历
    /**
     * @brief 遍历（使用函数指针）
     * @param visit 访问函数
     * @time O(n)
     */
    void traverse(void (*visit)(T&));

    /**
     * @brief 遍历（使用函数对象）
     * @param visit 访问函数对象
     * @time O(n)
     */
    template <typename VST>
    void traverse(VST& visit);
};

// ==================== ListNode类实现 ====================

/**
 * @brief 作为当前节点的后继插入新节点
 * @param e 要插入的数据
 * @return 返回新插入的节点指针
 * @time O(1)
 * 
 * 算法思路：
 * 1. 创建新节点，设置其前驱为当前节点，后继为当前节点的后继
 * 2. 更新当前节点后继的前驱指针指向新节点
 * 3. 更新当前节点的后继指针指向新节点
 */
template <typename T>
ListNode<T>* ListNode<T>::insertAsSucc(T const& e) {
    ListNode<T>* x = new ListNode(e, this, succ);  // 创建新节点
    succ->pred = x;                                 // 设置后继的前驱
    succ = x;                                       // 设置当前节点的后继
    return x;                                       // 返回新节点
}

/**
 * @brief 作为当前节点的前驱插入新节点
 * @param e 要插入的数据
 * @return 返回新插入的节点指针
 * @time O(1)
 */
template <typename T>
ListNode<T>* ListNode<T>::insertAsPred(T const& e) {
    ListNode<T>* x = new ListNode(e, pred, this);  // 创建新节点
    pred->succ = x;                                 // 设置前驱的后继
    pred = x;                                       // 设置当前节点的前驱
    return x;                                       // 返回新节点
}

// ==================== List类实现 ====================

/**
 * @brief 列表创建时的初始化
 * @time O(1)
 * 
 * 算法思路：
 * 1. 创建头哨兵和尾哨兵节点
 * 2. 设置头尾哨兵的连接关系
 * 3. 初始化规模为0
 */
template <typename T>
void List<T>::init() {
    header = new ListNode<T>;                       // 创建头哨兵节点
    trailer = new ListNode<T>;                      // 创建尾哨兵节点
    header->succ = trailer; header->pred = nullptr; // 向前链接
    trailer->pred = header; trailer->succ = nullptr; // 向后链接
    _size = 0;                                      // 记录规模
}

/**
 * @brief 复制构造函数
 * @param L 要复制的列表
 * @time O(n)
 */
template <typename T>
List<T>::List(List<T> const& L) {
    init();                                         // 初始化
    copyNodes(L.first(), L._size);                  // 复制L的所有节点
}

/**
 * @brief 复制列表L中自第r项起的n项
 * @param L 源列表
 * @param r 起始位置
 * @param n 复制项数
 * @time O(n)
 */
template <typename T>
List<T>::List(List<T> const& L, int r, int n) {
    init();                                         // 初始化
    // 获取第r个节点的指针
    ListNode<T>* p = L.first();                     // 从首节点出发
    while (0 < r--) p = p->succ;                    // 顺数第r个节点
    copyNodes(p, n);                                // 复制L中自第r项起的n项
}

/**
 * @brief 复制列表中自位置p起的n项
 * @param p 起始节点
 * @param n 复制项数
 * @time O(n)
 */
template <typename T>
List<T>::List(ListNode<T>* p, int n) {
    init();                                         // 初始化
    copyNodes(p, n);                                // 复制自p起的n项
}

/**
 * @brief 析构函数
 * @time O(n)
 */
template <typename T>
List<T>::~List() {
    clear();                                        // 清空列表
    delete header;                                  // 释放头哨兵
    delete trailer;                                 // 释放尾哨兵
}

/**
 * @brief 清除所有节点
 * @return 被清除节点的数目
 * @time O(n)
 */
template <typename T>
int List<T>::clear() {
    int oldSize = _size;
    while (0 < _size) remove(header->succ);        // 反复删除首节点，直至列表变空
    return oldSize;
}

/**
 * @brief 复制列表中自位置p起的n项
 * @param p 起始位置
 * @param n 项数
 * @time O(n)
 */
template <typename T>
void List<T>::copyNodes(ListNode<T>* p, int n) {
    init();                                         // 创建头、尾哨兵节点并做初始化
    while (n--) {                                   // 将起自p的n项依次作为末节点插入
        insertAsLast(p->data);
        p = p->succ;
    }
}

/**
 * @brief 重载下标操作符，支持循秩访问（效率低）
 * @param r 秩
 * @return 对应元素的引用
 * @time O(r)
 */
template <typename T>
T& List<T>::operator[](int r) const {
    ListNode<T>* p = first();                       // 从首节点出发
    while (0 < r--) p = p->succ;                    // 顺数第r个节点即是
    return p->data;                                 // 目标节点，返回其中所存元素
}

/**
 * @brief 在无序列表内节点p（可能是trailer）的n个（真）前驱中，找到等于e的最后者
 * @param e 目标元素
 * @param n 查找范围
 * @param p 起始位置
 * @return 找到的节点指针，失败时返回nullptr
 * @time O(n)
 */
template <typename T>
ListNode<T>* List<T>::find(T const& e, int n, ListNode<T>* p) const {
    while (0 < n--)                                 // 对于p的最近的n个前驱，从右向左
        if (e == (p = p->pred)->data) return p;     // 逐个比对，直至命中或范围越界
    return nullptr;                                 // p越出左边界意味着区间内不含e，查找失败
}

/**
 * @brief 在有序列表内节点p（可能是trailer）的n个（真）前驱中，找到不大于e的最后者
 * @param e 目标元素
 * @param n 查找范围
 * @param p 起始位置
 * @return 找到的节点指针
 * @time O(n)
 */
template <typename T>
ListNode<T>* List<T>::search(T const& e, int n, ListNode<T>* p) const {
    while (0 <= n--)                                // 对于p的最近的n个前驱，从右向左逐个比较
        if (((p = p->pred)->data) <= e) break;      // 直至命中、数值越界或范围越界
    return p;                                       // 返回查找终止的位置
}

/**
 * @brief 将e当作首节点插入
 * @param e 要插入的元素
 * @return 新插入的节点指针
 * @time O(1)
 */
template <typename T>
ListNode<T>* List<T>::insertAsFirst(T const& e) {
    _size++; return header->insertAsSucc(e);        // e当作首节点插入
}

/**
 * @brief 将e当作末节点插入
 * @param e 要插入的元素
 * @return 新插入的节点指针
 * @time O(1)
 */
template <typename T>
ListNode<T>* List<T>::insertAsLast(T const& e) {
    _size++; return trailer->insertAsPred(e);       // e当作末节点插入
}

/**
 * @brief 将e当作p的前驱插入
 * @param p 目标位置
 * @param e 要插入的元素
 * @return 新插入的节点指针
 * @time O(1)
 */
template <typename T>
ListNode<T>* List<T>::insertBefore(ListNode<T>* p, T const& e) {
    _size++; return p->insertAsPred(e);             // e当作p的前驱插入
}

/**
 * @brief 将e当作p的后继插入
 * @param p 目标位置
 * @param e 要插入的元素
 * @return 新插入的节点指针
 * @time O(1)
 */
template <typename T>
ListNode<T>* List<T>::insertAfter(ListNode<T>* p, T const& e) {
    _size++; return p->insertAsSucc(e);             // e当作p的后继插入
}

/**
 * @brief 删除合法位置p处的节点，返回被删除节点
 * @param p 要删除的节点
 * @return 被删除节点的数据
 * @time O(1)
 * 
 * 算法思路：
 * 1. 备份待删除节点的数据
 * 2. 将p的前驱与后继相互链接
 * 3. 释放节点p
 * 4. 更新规模，返回备份的数据
 */
template <typename T>
T List<T>::remove(ListNode<T>* p) {
    T e = p->data;                                  // 备份待删除节点的数值（假定T类型可直接赋值）
    p->pred->succ = p->succ; p->succ->pred = p->pred; // 后继、前驱
    delete p; _size--;                              // 释放节点，更新规模
    return e;                                       // 返回备份的数值
}

/**
 * @brief 归并操作：当前列表中自p起的n个元素，与列表L中自q起的m个元素归并
 * @param p 当前列表起始位置
 * @param n 当前列表元素个数
 * @param L 另一个列表
 * @param q 另一个列表起始位置
 * @param m 另一个列表元素个数
 * @time O(n+m)
 */
template <typename T>
void List<T>::merge(ListNode<T>*& p, int n, List<T>& L, ListNode<T>* q, int m) {
    ListNode<T>* pp = p->pred;                      // 借助前驱（可能是header），以便返回前...
    while (0 < m)                                   // 在q尚未移出区间之前
        if ((0 < n) && (p->data <= q->data)) {      // 若p仍在区间内且v(p) <= v(q)，则
            if (q == (p = p->succ)) break;          // p归入合并的列表，并替换为其直接后继
            n--;
        } else {                                    // 若p已超出右界或v(q) < v(p)，则
            insertBefore(p, L.remove((q = q->succ)->pred)); // 将q转移至p之前
            m--;
        }
    p = pp->succ;                                   // 确定归并后区间的（新）起点
}

/**
 * @brief 列表的归并排序算法：对起始于位置p的n个元素排序
 * @param p 起始位置
 * @param n 元素个数
 * @time O(nlogn)
 * @space O(logn) - 递归栈空间
 */
template <typename T>
void List<T>::mergeSort(ListNode<T>*& p, int n) {
    if (n < 2) return;                              // 若待排序范围已足够小，则直接返回；否则...
    int m = n >> 1;                                 // 以中点为界
    ListNode<T>* q = p; for (int i = 0; i < m; i++) q = q->succ; // 均分列表
    mergeSort(p, m); mergeSort(q, n - m);           // 对前、后子列表分别排序
    merge(p, m, *this, q, n - m);                   // 归并
}

/**
 * @brief 列表的选择排序算法：对起始于位置p的n个元素排序
 * @param p 起始位置
 * @param n 元素个数
 * @time O(n^2)
 */
template <typename T>
void List<T>::selectionSort(ListNode<T>* p, int n) {
    ListNode<T>* head = p->pred; ListNode<T>* tail = p;
    for (int i = 0; i < n; i++) tail = tail->succ; // 待排序区间为(head, tail)
    while (1 < n) {                                 // 在至少还剩两个节点之前，在待排序区间内
        ListNode<T>* max = selectMax(head->succ, n); // 找出最大者（歧义时后者优先）
        insertBefore(tail, remove(max));            // 将其移至无序区间末尾（作为有序区间新的首元素）
        tail = tail->pred; n--;
    }
}

/**
 * @brief 列表的插入排序算法：对起始于位置p的n个元素排序
 * @param p 起始位置
 * @param n 元素个数
 * @time O(n^2)
 */
template <typename T>
void List<T>::insertionSort(ListNode<T>* p, int n) {
    for (int r = 0; r < n; r++) {                   // 逐一为各节点
        insertAfter(search(p->data, r, p), p->data); // 查找适当的位置并插入
        p = p->succ; remove(p->pred);               // 转向下一节点
    }
}

/**
 * @brief 对列表区间排序
 * @param p 起始位置
 * @param n 元素个数
 * @time O(nlogn)
 */
template <typename T>
void List<T>::sort(ListNode<T>* p, int n) {
    mergeSort(p, n);                                // 使用归并排序算法
}

/**
 * @brief 无序去重：删除无序列表中重复节点（效率低）
 * @return 被删除节点总数
 * @time O(n^2)
 */
template <typename T>
int List<T>::deduplicate() {
    if (_size < 2) return 0;                        // 平凡列表自然无重复
    int oldSize = _size;                            // 记录原规模
    ListNode<T>* p = header; int r = 0;             // p从首节点开始
    while (trailer != (p = p->succ)) {              // 依次直到末节点
        ListNode<T>* q = find(p->data, r, p);       // 在p的r个（真）前驱中查找雷同者
        if (q) remove(q); else r++;                 // 若的确存在，则删除之；否则秩加一
    }
    return oldSize - _size;                         // 返回删除元素总数
}

/**
 * @brief 有序去重：删除有序列表中重复节点
 * @return 被删除节点总数
 * @time O(n)
 */
template <typename T>
int List<T>::uniquify() {
    if (_size < 2) return 0;                        // 平凡列表自然无重复
    int oldSize = _size;                            // 记录原规模
    ListNode<T>* p = first(); ListNode<T>* q;       // p为各区段起点，q为其后继
    while (trailer != (q = p->succ))               // 反复考查紧邻的节点对(p, q)
        if (p->data != q->data) p = q;              // 若互异，则转向下一区段
        else remove(q);                             // 否则（雷同），删除后者
    return oldSize - _size;                         // 返回删除元素总数
}

/**
 * @brief 前后倒置
 * @time O(n)
 */
template <typename T>
void List<T>::reverse() {
    ListNode<T>* p = header; ListNode<T>* q = trailer; // 头、尾节点
    for (int i = 1; i < _size; i += 2)              // （从首、末节点开始）由外而内，捉对地
        std::swap((p = p->succ)->data, (q = q->pred)->data); // 交换对称节点的数据项
}

/**
 * @brief 遍历（使用函数指针）
 * @param visit 访问函数
 * @time O(n)
 */
template <typename T>
void List<T>::traverse(void (*visit)(T&)) {
    for (ListNode<T>* p = header->succ; p != trailer; p = p->succ)
        visit(p->data);
}

/**
 * @brief 遍历（使用函数对象）
 * @param visit 访问函数对象
 * @time O(n)
 */
template <typename T>
template <typename VST>
void List<T>::traverse(VST& visit) {
    for (ListNode<T>* p = header->succ; p != trailer; p = p->succ)
        visit(p->data);
}

/**
 * @brief 判断列表是否已排序
 * @return 已排序返回true，否则返回false
 * @time O(n)
 */
template <typename T>
int List<T>::disordered() const {
    int n = 0;                                      // 计数器
    for (ListNode<T>* p = first(); p != trailer; p = p->succ) // 逐一检查各对相邻节点
        if (p->succ != trailer && p->data > p->succ->data) n++; // 逆序则计数
    return n;                                       // 返回逆序相邻对总数
}

/**
 * @brief 在p及其n-1个后继中选出最大者
 * @param p 起始位置
 * @param n 范围
 * @return 最大元素的节点指针
 * @time O(n)
 */
template <typename T>
ListNode<T>* List<T>::selectMax(ListNode<T>* p, int n) {
    ListNode<T>* max = p;                           // 最大者暂定为首节点p
    for (ListNode<T>* cur = p; 1 < n; n--)          // 从首节点p出发，将后续节点逐一与max比较
        if (!((cur = cur->succ)->data < max->data)) // 若当前元素不小于max，则
            max = cur;                              // 更新最大元素位置记录
    return max;                                     // 返回最大节点位置
}

// 显式实例化常用类型
template class ListNode<int>;
template class ListNode<double>;
template class ListNode<char>;
template class ListNode<std::string>;

template class List<int>;
template class List<double>;
template class List<char>;
template class List<std::string>;