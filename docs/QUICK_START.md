# TinySTL 快速入门指南

## 5 分钟快速开始

### 步骤 1：克隆项目

```bash
git clone https://github.com/allenmirac/TinySTL.git
cd TinySTL
```

### 步骤 2：一键构建

#### Linux / macOS
```bash
chmod +x build.sh
./build.sh
```

#### Windows
```cmd
build.bat
```

### 步骤 3：运行示例

```bash
# Linux/macOS
./build/bin/example_vector

# Windows
.\build\bin\Release\example_vector.exe
```

就这么简单！🎉

---

## 第一个程序

创建文件 `my_first_tinystl.cpp`：

```cpp
#include "vector.h"
#include <iostream>

int main() {
    // 创建一个 vector
    tinystl::vector<int> v;
    
    // 添加一些元素
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    
    // 打印元素
    std::cout << "My first TinySTL vector: ";
    for (const auto& val : v) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

### 编译运行

#### 方式 1：使用 g++ 直接编译

```bash
# 确保头文件在当前目录或 include 目录
g++ -std=c++11 my_first_tinystl.cpp -o my_program
./my_program
```

#### 方式 2：添加到 CMake 项目

在 `examples/CMakeLists.txt` 中添加：

```cmake
add_executable(my_first_tinystl my_first_tinystl.cpp)
target_link_libraries(my_first_tinystl PRIVATE TinySTL)
```

然后重新构建：

```bash
cd build
cmake ..
cmake --build .
./bin/my_first_tinystl
```

---

## 核心概念速览

### 1. Allocator（空间配置器）

**作用**：分离内存分配和对象构造

```cpp
#include "allocator.h"

// 分配内存（不构造对象）
int* ptr = tinystl::allocator<int>::allocate(10);

// 在指定位置构造对象
tinystl::allocator<int>::construct(ptr, 42);

// 使用对象
std::cout << *ptr << std::endl;  // 42

// 析构对象
tinystl::allocator<int>::destroy(ptr);

// 释放内存
tinystl::allocator<int>::deallocate(ptr, 10);
```

### 2. Vector（动态数组）

**特点**：连续存储、动态扩容、随机访问

```cpp
#include "vector.h"

tinystl::vector<int> v;

// 添加元素
v.push_back(1);
v.push_back(2);
v.emplace_back(3);  // 完美转发，更高效

// 访问元素
std::cout << v[0] << std::endl;     // 1
std::cout << v.front() << std::endl; // 1
std::cout << v.back() << std::endl;  // 3

// 插入和删除
v.insert(v.begin() + 1, 999);  // 在位置 1 插入 999
v.erase(v.begin());            // 删除第一个元素

// 容量管理
v.reserve(100);  // 预留容量
v.resize(50);    // 调整大小
```

### 3. Iterator（迭代器）

**作用**：统一的容器访问接口

```cpp
#include "vector.h"

tinystl::vector<int> v = {1, 2, 3, 4, 5};

// 正向遍历
for (auto it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << " ";
}

// 反向遍历
for (auto it = v.rbegin(); it != v.rend(); ++it) {
    std::cout << *it << " ";
}

// 范围 for（C++11）
for (const auto& val : v) {
    std::cout << val << " ";
}
```

### 4. Type Traits（类型特性）

**作用**：编译期类型判断和优化

```cpp
#include "type_traits.h"

// 判断类型是否相同
static_assert(tinystl::is_same<int, int>::value, "Should be same");

// 判断类型是否可平凡拷贝（可用 memcpy）
static_assert(tinystl::is_trivially_copyable<int>::value, "int is trivially copyable");

// 条件编译
template <class T>
typename tinystl::enable_if<tinystl::is_trivially_copyable<T>::value, void>::type
fast_copy(T* src, T* dst, size_t n) {
    memcpy(dst, src, n * sizeof(T));
}
```

---

## 学习路径

### 初学者（1-2 周）

1. **理解基础概念**
   - 阅读 [README.md](README.md)
   - 运行 `examples/` 中的示例程序
   - 理解 allocator 的作用

2. **学习 vector**
   - 阅读 `vector.h` 源码
   - 运行 `test/test_vector.cpp`
   - 尝试修改和扩展功能

3. **掌握迭代器**
   - 理解五种迭代器类型
   - 学习 iterator_traits
   - 实践 tag dispatch

### 进阶（2-4 周）

1. **深入内存管理**
   - 研究 allocator 实现
   - 理解 uninitialized_xxx 函数
   - 学习异常安全

2. **模板元编程**
   - 学习 type_traits
   - 理解 SFINAE
   - 掌握 enable_if

3. **实现新容器**
   - 尝试实现 deque
   - 完善 string
   - 重构 rb_tree

### 高级（1-2 个月）

1. **性能优化**
   - POD 类型优化
   - 内存池设计
   - 性能基准测试

2. **面试准备**
   - 阅读 [SKILLS.md](SKILLS.md)
   - 练习手写代码
   - 准备面试问题

---

## 常见问题

### Q1: 编译错误：找不到头文件

**解决方案**：

```bash
# 方式 1：确保头文件在当前目录
g++ -std=c++11 -I. your_file.cpp

# 方式 2：使用 CMake（推荐）
mkdir build && cd build
cmake ..
cmake --build .
```

### Q2: 如何在自己的项目中使用 TinySTL？

**方式 1：直接包含头文件**

```cpp
// 将 TinySTL 的头文件复制到你的项目
#include "tinystl/vector.h"
#include "tinystl/allocator.h"
```

**方式 2：作为 CMake 子项目**

```cmake
# 在你的 CMakeLists.txt 中
add_subdirectory(TinySTL)
target_link_libraries(your_target PRIVATE TinySTL)
```

### Q3: 如何添加新的测试？

1. 在 `test/` 目录创建测试文件
2. 在 `test/CMakeLists.txt` 中添加：

```cmake
add_executable(test_myfeature test_myfeature.cpp)
target_link_libraries(test_myfeature PRIVATE TinySTL)
add_test(NAME MyFeatureTest COMMAND test_myfeature)
```

3. 重新构建并运行测试：

```bash
cd build
cmake ..
cmake --build .
ctest
```

### Q4: 如何调试？

**使用 GDB（Linux/macOS）**：

```bash
# 编译时添加调试信息
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .

# 使用 GDB 调试
gdb ./bin/test_vector
(gdb) break main
(gdb) run
(gdb) next
```

**使用 Visual Studio（Windows）**：

1. 用 Visual Studio 打开 CMake 项目
2. 设置断点
3. 按 F5 开始调试

---

## 推荐阅读顺序

1. **README.md** - 项目概览
2. **QUICK_START.md**（本文档）- 快速入门
3. **examples/** - 运行示例程序
4. **SKILLS.md** - 详细学习路线
5. **PROJECT_STRUCTURE.md** - 项目结构
6. **源码阅读** - 按以下顺序：
   - `type_traits.h`
   - `iterator.h`
   - `allocator.h`
   - `utility.h`
   - `uninitialized.h`
   - `algorithm.h`
   - `vector.h`

---

## 下一步

- 运行所有示例程序
- 阅读 vector.h 源码
- 完成一个小项目（使用 TinySTL）
- 阅读 [SKILLS.md](SKILLS.md) 深入学习
- 尝试实现新功能
- 参与项目贡献

---

## 🔗 相关链接

- [GitHub 仓库](https://github.com/allenmirac/TinySTL)
- [完整文档](README.md)
- [学习路线](SKILLS.md)
- [项目结构](PROJECT_STRUCTURE.md)
- [C++ 参考](https://en.cppreference.com/)

---


如有问题，欢迎提 Issue 或 PR！
