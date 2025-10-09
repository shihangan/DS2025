# ListNode 双向链表实现

## 概述

本项目实现了一个完整的双向链表数据结构，包含 `ListNode` 节点类和 `List` 链表类。该实现提供了丰富的ADT接口，支持各种链表操作。

## 文件结构

- `listnode.h` - 头文件，包含类声明和接口定义
- `listnode.tpp` - 模板实现文件，包含所有方法的具体实现
- `simple_listnode_test.cpp` - 测试程序
- `test_listnode.cpp` - 完整测试程序（包含中文注释）

## 主要特性

### ListNode 节点类
- **数据成员**：
  - `T data` - 数据域
  - `ListNode<T>* pred` - 前驱指针
  - `ListNode<T>* succ` - 后继指针

- **操作接口**：
  - `insertAsSucc(e)` - 作为后继插入新节点
  - `insertAsPred(e)` - 作为前驱插入新节点

### List 链表类

#### 1. 构造与析构
- `List()` - 默认构造函数
- `List(const List& L)` - 复制构造函数
- `List(const List& L, int r, int n)` - 部分复制构造函数
- `List(ListNode<T>* p, int n)` - 从指定位置复制构造
- `~List()` - 析构函数

#### 2. 基本信息
- `size()` - 获取链表规模
- `empty()` - 判断是否为空
- `first()` - 获取首节点
- `last()` - 获取末节点
- `valid(p)` - 判断位置是否合法

#### 3. 元素访问
- `operator[](r)` - 支持下标访问（效率较低，O(r)）

#### 4. 查找操作
- `find(e)` - 无序查找
- `find(e, n, p)` - 在指定范围内无序查找
- `search(e)` - 有序查找
- `search(e, n, p)` - 在指定范围内有序查找

#### 5. 插入操作
- `insertAsFirst(e)` - 作为首节点插入
- `insertAsLast(e)` - 作为末节点插入
- `insertBefore(p, e)` - 在指定位置前插入
- `insertAfter(p, e)` - 在指定位置后插入

#### 6. 删除操作
- `remove(p)` - 删除指定位置的节点
- `clear()` - 清空整个链表

#### 7. 排序算法
- `sort()` - 链表排序（使用归并排序）
- `mergeSort(p, n)` - 归并排序
- `selectionSort(p, n)` - 选择排序
- `insertionSort(p, n)` - 插入排序

#### 8. 去重操作
- `deduplicate()` - 无序去重
- `uniquify()` - 有序去重

#### 9. 其他操作
- `reverse()` - 链表倒置
- `merge(L)` - 与另一个有序链表归并

#### 10. 遍历操作
- `traverse(visit)` - 使用函数指针遍历
- `traverse(visit)` - 使用函数对象遍历

## 时间复杂度

| 操作 | 时间复杂度 | 说明 |
|------|------------|------|
| 插入/删除 | O(1) | 在已知位置进行操作 |
| 查找 | O(n) | 线性查找 |
| 排序 | O(n log n) | 归并排序 |
| 去重（有序） | O(n) | 一次遍历 |
| 去重（无序） | O(n²) | 需要重复查找 |
| 遍历 | O(n) | 访问所有节点 |

## 空间复杂度

- 节点存储：O(n)
- 排序辅助空间：O(log n)（归并排序的递归栈）

## 使用示例

```cpp
#include "listnode.h"
#include <iostream>

int main() {
    // 创建链表
    List<int> list;
    
    // 插入元素
    list.insertAsLast(10);
    list.insertAsLast(20);
    list.insertAsLast(30);
    
    // 查找元素
    ListNode<int>* found = list.find(20);
    if (found) {
        std::cout << "Found: " << found->data << std::endl;
    }
    
    // 排序
    list.sort();
    
    // 遍历
    list.traverse([](int& x) { std::cout << x << " "; });
    
    return 0;
}
```

## 编译和运行

```bash
# 编译测试程序
g++ -o simple_listnode_test.exe simple_listnode_test.cpp -std=c++11

# 运行测试
./simple_listnode_test.exe
```

## 测试结果

测试程序验证了以下功能：
- ✅ 基本构造和插入操作
- ✅ 元素访问
- ✅ 查找操作
- ✅ 排序功能
- ✅ 所有操作均正常工作

## 注意事项

1. 该实现使用了哨兵节点（header 和 trailer）来简化边界处理
2. 支持任意类型的模板参数
3. 提供了完整的异常安全保证
4. 所有动态分配的内存都会在析构时正确释放

## 扩展功能

该实现还支持：
- 函数对象遍历，支持全局性修改
- 部分区间操作
- 链表归并
- 多种排序算法选择