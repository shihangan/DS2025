#ifndef __LISTNODE_H__
#define __LISTNODE_H__

#include <iostream>
#include <string>
#include <algorithm>

namespace MySTL {

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
    ListNode();                   // 针对header和trailer的构造
    ListNode(T e, ListNode<T>* p = nullptr, ListNode<T>* s = nullptr);  // 默认构造器

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
    List();

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
    ListNode<T>* find(T const& e) const;

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
    ListNode<T>* search(T const& e) const;

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
    ListNode<T>* selectMax();

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
    void sort();

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

} // namespace MySTL

#endif // __LISTNODE_H__