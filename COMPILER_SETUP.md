# C++ 编译器安装指南

由于您的系统中没有检测到C++编译器，请按照以下步骤安装编译器来编译和运行MySTL项目。

## 推荐选项

### 选项1：安装Visual Studio Community（推荐）

1. 访问 [Visual Studio官网](https://visualstudio.microsoft.com/zh-hans/vs/community/)
2. 下载并安装Visual Studio Community（免费）
3. 在安装时选择"使用C++的桌面开发"工作负载
4. 安装完成后重启VS Code

### 选项2：安装MinGW-w64

1. 访问 [MSYS2官网](https://www.msys2.org/)
2. 下载并安装MSYS2
3. 打开MSYS2终端，运行以下命令：
   ```bash
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-gdb
   ```
4. 将 `C:\msys64\mingw64\bin` 添加到系统PATH环境变量
5. 重启VS Code

### 选项3：使用在线编译器（临时解决方案）

如果暂时无法安装编译器，可以使用在线C++编译器：

1. **Compiler Explorer**: https://godbolt.org/
   - 复制 `vector.h` 和 `vector.tpp` 的内容
   - 复制 `example.cpp` 或 `test_vector.cpp` 的内容
   - 在线编译和运行

2. **OnlineGDB**: https://www.onlinegdb.com/online_c++_compiler
   - 支持完整的项目文件上传
   - 可以运行测试程序

3. **Replit**: https://replit.com/
   - 创建C++项目
   - 上传所有文件
   - 在线编译运行

## 验证安装

安装编译器后，在VS Code终端中运行以下命令验证：

```powershell
# 检查Visual Studio编译器
cl

# 或检查MinGW编译器
g++ --version
```

## 编译项目

安装编译器后，可以使用以下命令编译项目：

### 使用Visual Studio编译器
```powershell
cl /EHsc /std:c++17 /Fe:test_vector.exe test_vector.cpp
cl /EHsc /std:c++17 /Fe:example.exe example.cpp
```

### 使用MinGW编译器
```powershell
g++ -std=c++17 -Wall -Wextra -O2 test_vector.cpp -o test_vector.exe
g++ -std=c++17 -Wall -Wextra -O2 example.cpp -o example.exe
```

### 使用VS Code构建任务

1. 按 `Ctrl+Shift+P` 打开命令面板
2. 选择 "Tasks: Run Task"
3. 选择相应的构建任务

## 运行程序

编译成功后，运行可执行文件：

```powershell
.\test_vector.exe
.\example.exe
```

## 故障排除

### 常见问题

1. **"cl不是内部或外部命令"**
   - 确保已安装Visual Studio并包含C++工具
   - 在"开发人员命令提示符"中运行命令

2. **"g++不是内部或外部命令"**
   - 确保MinGW的bin目录已添加到PATH环境变量
   - 重启命令提示符或VS Code

3. **VS Code IntelliSense不工作**
   - 安装编译器后重启VS Code
   - 检查 `.vscode/c_cpp_properties.json` 配置

### 获取帮助

如果遇到问题，可以：
1. 查看VS Code的C++扩展文档
2. 检查编译器官方文档
3. 使用在线编译器作为临时解决方案

---

**注意**: 本项目的MySTL库是完全基于标准C++实现的，不依赖任何第三方库，只需要支持C++11或更高版本的编译器即可。