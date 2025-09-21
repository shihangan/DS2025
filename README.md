# MySTL - 自定义STL库

这是一个简洁且具有鲁棒性的C++ STL库实现，目前包含了`vector`容器的完整实现。

## 特性

### MySTL::vector

- **完整的STL兼容接口**: 支持所有标准vector操作
- **异常安全**: 提供强异常安全保证
- **内存高效**: 使用标准分配器，支持自定义分配器
- **性能优化**: 采用指数增长策略，减少内存重分配
- **类型安全**: 完整的模板支持和类型检查

#### 支持的操作

**构造函数**:
- 默认构造
- 计数构造 `vector(size_t count, const T& value)`
- 范围构造 `vector(InputIt first, InputIt last)`
- 拷贝构造
- 移动构造
- 初始化列表构造 `vector{1, 2, 3, 4, 5}`

**元素访问**:
- `operator[]` - 无边界检查的访问
- `at()` - 带边界检查的访问
- `front()` / `back()` - 首尾元素访问
- `data()` - 获取底层数组指针

**迭代器**:
- `begin()` / `end()` - 正向迭代器
- `rbegin()` / `rend()` - 反向迭代器
- `cbegin()` / `cend()` - 常量迭代器
- 支持范围for循环

**容量管理**:
- `size()` / `empty()` - 大小查询
- `capacity()` - 容量查询
- `reserve()` - 预留容量
- `resize()` - 调整大小
- `shrink_to_fit()` - 收缩容量

**修改操作**:
- `push_back()` / `pop_back()` - 尾部插入/删除
- `insert()` - 任意位置插入
- `erase()` - 任意位置删除
- `emplace()` / `emplace_back()` - 原地构造
- `clear()` - 清空容器
- `swap()` - 交换容器

**比较操作**:
- `==`, `!=`, `<`, `<=`, `>`, `>=` - 全套比较操作

## 文件结构

```
MySTL/
├── vector.h        # vector类声明
├── vector.tpp      # vector类模板实现
├── test_vector.cpp # 单元测试
├── example.cpp     # 使用示例
└── README.md       # 项目说明
```

## 编译和使用

### 前置要求

**重要**: 本项目需要C++编译器才能编译运行。如果您的系统中没有安装编译器，请参考 [编译器安装指南](COMPILER_SETUP.md) 进行安装。

支持的编译器：
- Visual Studio 2017或更高版本（Windows推荐）
- GCC 4.8或更高版本
- Clang 3.3或更高版本

### 编译测试程序

**使用Visual Studio编译器（Windows）：**
```powershell
cl /EHsc /std:c++17 /Fe:test_vector.exe test_vector.cpp
```

**使用GCC/MinGW：**
```bash
g++ -std=c++11 -Wall -Wextra -O2 test_vector.cpp -o test_vector
```

### 编译示例程序

**使用Visual Studio编译器（Windows）：**
```powershell
cl /EHsc /std:c++17 /Fe:example.exe example.cpp
```

**使用GCC/MinGW：**
```bash
g++ -std=c++11 -Wall -Wextra -O2 example.cpp -o example
```

### 使用VS Code

如果您使用VS Code，可以：
1. 按 `Ctrl+Shift+P` 打开命令面板
2. 选择 "Tasks: Run Task"
3. 选择相应的构建任务（Build with MSVC 或 Build with MinGW）

### 使用Makefile
```bash
make          # 编译所有程序
make test     # 编译并运行测试
make example  # 编译并运行示例
make clean    # 清理生成的文件
```

### 在线编译（无需安装编译器）

如果暂时无法安装编译器，可以使用在线C++编译器：
- [Compiler Explorer](https://godbolt.org/)
- [OnlineGDB](https://www.onlinegdb.com/online_c++_compiler)
- [Replit](https://replit.com/)

详细说明请参考 [编译器安装指南](COMPILER_SETUP.md)。

### 基本使用

```cpp
#include "vector.h"

int main() {
    // 创建vector
    MySTL::vector<int> v{1, 2, 3, 4, 5};
    
    // 添加元素
    v.push_back(6);
    
    // 遍历
    for (const auto& val : v) {
        std::cout << val << " ";
    }
    
    return 0;
}
```

## 设计特点

### 1. 异常安全
- 所有操作都提供强异常安全保证
- 内存分配失败时自动清理已分配资源
- 构造函数异常时正确释放内存

### 2. 内存管理
- 使用标准分配器接口，支持自定义分配器
- 采用RAII原则，自动管理资源生命周期
- 指数增长策略，平摊时间复杂度为O(1)

### 3. 性能优化
- 移动语义支持，减少不必要的拷贝
- 原地构造(emplace)，避免临时对象
- 内存预分配，减少重分配次数

### 4. 标准兼容
- 完全兼容STL vector接口
- 支持标准算法库
- 类型别名和迭代器类型完整

## 测试

运行测试程序验证功能：

```bash
g++ -std=c++11 -O2 test_vector.cpp -o test_vector
./test_vector
```

测试覆盖：
- 所有构造函数
- 元素访问操作
- 迭代器功能
- 容量管理
- 修改操作
- 比较操作
- 异常处理

## 要求

- C++11或更高版本
- 支持标准库的编译器

## 许可证

本项目仅供学习和研究使用。