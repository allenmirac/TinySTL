# TinySTL

<div align="center">

![C++](https://img.shields.io/badge/C++-11/14/17-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.10+-green.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Status](https://img.shields.io/badge/status-Active-success.svg)

**一个用于学习和面试准备的轻量级 C++ STL 实现**

[快速开始](#-快速开始) • [特性](#-核心特性) • [文档](#-文档) • [示例](#-使用示例)

</div>

---

## 项目简介

TinySTL 是一个教育性质的 C++ 标准模板库（STL）实现，旨在帮助深入理解 STL 的设计理念和实现细节。

### 适用人群

- 准备 C++ 技术面试的求职者
- 学习 STL 源码的开发者
- 提升模板元编程能力的工程师
- 理解数据结构和算法实现的学生

---

## 已完成功能

### 🔧 基础设施

| 模块 | 功能 | 状态 |
|------|------|------|
| **type_traits.h** | 类型萃取（integral_constant, enable_if, is_same, is_trivially_copyable 等） | ✅ |
| **iterator.h** | 迭代器系统（五种迭代器类型、iterator_traits、reverse_iterator、distance、advance） | ✅ |
| **allocator.h** | 空间配置器（分离内存分配和对象构造，支持完美转发） | ✅ |
| **uninitialized.h** | 未初始化内存操作（uninitialized_copy/fill/move，支持 POD 优化） | ✅ |
| **utility.h** | 工具函数（move, forward, swap, pair） | ✅ |
| **algorithm.h** | 基础算法（copy, move, fill, equal, lexicographical_compare, reverse 等） | ✅ |

### 📦 容器

| 容器 | 功能 | 状态 |
|------|------|------|
| **vector** | 动态数组（完整实现，支持 C++11 特性） | ✅ |
| **string** | 字符串（基础框架） | 🔄 |
| **RB_Tree** | 红黑树（基础实现） | 🔄 |
| **deque** | 双端队列 | 📋 |
| **list** | 双向链表 | 📋 |
| **map/set** | 关联容器 | 📋 |

**图例**：✅ 已完成 | 🔄 进行中 | 📋 计划中

---

## 核心特性

### 1. 内存管理分离

```cpp
// ❌ 传统 new：同时分配内存和构造对象
T* p = new T[100];  // 立即构造 100 个对象

// ✅ TinySTL allocator：分离内存分配和对象构造
T* p = allocator<T>::allocate(100);     // 只分配内存
allocator<T>::construct(p, args...);    // 按需构造对象
allocator<T>::destroy(p);               // 只析构对象
allocator<T>::deallocate(p, 100);       // 只释放内存
```

### 2. 类型萃取优化

```cpp
// 利用 type_traits 进行编译期优化
template <class T>
void copy_impl(T* first, T* last, T* result, m_true_type) {
    // POD 类型：使用 memmove 优化
    memmove(result, first, (last - first) * sizeof(T));
}

template <class T>
void copy_impl(T* first, T* last, T* result, m_false_type) {
    // 非 POD 类型：逐个拷贝构造
    for (; first != last; ++first, ++result) {
        allocator<T>::construct(result, *first);
    }
}
```

### 3. 完美转发

```cpp
// emplace_back：直接在容器内构造对象，避免临时对象
template <class... Args>
void emplace_back(Args&&... args) {
    allocator<T>::construct(end_, tinystl::forward<Args>(args)...);
    ++end_;
}
```

### 4. 迭代器萃取

```cpp
// 统一接口，支持原生指针和自定义迭代器
template <class Iterator>
typename iterator_traits<Iterator>::difference_type
distance(Iterator first, Iterator last) {
    // Tag Dispatch：根据迭代器类型选择最优实现
    return distance_dispatch(first, last, iterator_category(first));
}
```

---

## 快速开始

### 环境要求

- **编译器**：支持 C++11 或更高版本（GCC 5.0+, Clang 3.4+, MSVC 2015+）
- **构建工具**：CMake 3.10+

### 一键构建

#### Linux / macOS

```bash
# 克隆仓库
git clone https://github.com/allenmirac/TinySTL.git
cd TinySTL

# 运行构建脚本
chmod +x build.sh
./build.sh
```

#### Windows

```cmd
# 克隆仓库
git clone https://github.com/allenmirac/TinySTL.git
cd TinySTL

# 运行构建脚本
build.bat
```

### 手动构建

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 编译
cmake --build . --config Release

# 运行测试
ctest --output-on-failure
```

### 运行示例

```bash
# Linux/macOS
./build/bin/example_vector
./build/bin/example_allocator

# Windows
.\build\bin\Release\example_vector.exe
.\build\bin\Release\example_allocator.exe
```

---

## 使用示例

### Vector 示例

```cpp
#include "vector.h"
#include <iostream>

int main() {
    // 创建 vector
    tinystl::vector<int> v;
    
    // 添加元素
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    // 使用 emplace_back（完美转发，避免临时对象）
    v.emplace_back(4);
    
    // 范围 for 循环（C++11）
    for (const auto& val : v) {
        std::cout << val << " ";
    }
    // 输出：1 2 3 4
    
    // 插入和删除
    v.insert(v.begin() + 2, 999);  // 在位置 2 插入 999
    v.erase(v.begin() + 1);        // 删除位置 1 的元素
    
    // 拷贝和移动
    tinystl::vector<int> v2(v);              // 拷贝构造
    tinystl::vector<int> v3(tinystl::move(v)); // 移动构造
    
    return 0;
}
```

### Allocator 示例

```cpp
#include "allocator.h"

int main() {
    // 1. 分配内存（不构造对象）
    int* ptr = tinystl::allocator<int>::allocate(10);
    
    // 2. 在指定位置构造对象
    tinystl::allocator<int>::construct(ptr, 42);
    tinystl::allocator<int>::construct(ptr + 1, 100);
    
    // 3. 使用对象
    std::cout << ptr[0] << ", " << ptr[1] << std::endl;  // 42, 100
    
    // 4. 析构对象
    tinystl::allocator<int>::destroy(ptr);
    tinystl::allocator<int>::destroy(ptr + 1);
    
    // 5. 释放内存
    tinystl::allocator<int>::deallocate(ptr, 10);
    
    return 0;
}
```

更多示例请查看 `examples/` 目录。

---

## 📚 文档

| 文档 | 说明 |
|------|------|
| [SKILLS.md](docs/SKILLS.md) | 📖 完整的学习路线图和技能文档 |
| [PROJECT_STRUCTURE.md](docs/PROJECT_STRUCTURE.md) | 🏗️ 项目结构说明和开发指南 |
| [PROJECT_ANALYSIS.md](docs/PROJECT_ANALYSIS.md) | 📊 项目现状分析和改进建议 |
| [QUICK_START.md](docs/QUICK_START.md) | 🚀 5分钟快速入门指南 |

### 学习路线

详细的学习指南请参考 [SKILLS.md](docs/SKILLS.md)，包括：

- ✅ STL 六大组件详解
- ✅ 模板元编程技巧
- ✅ 内存管理机制
- ✅ 数据结构实现
- ✅ 面试准备清单

---

## 🏗️ 项目结构

```
TinySTL/
├── include/                    # 头文件目录（建议）
│   ├── allocator.h            # 空间配置器
│   ├── algorithm.h            # 算法库
│   ├── iterator.h             # 迭代器系统
│   ├── type_traits.h          # 类型特性
│   ├── utility.h              # 工具函数
│   ├── uninitialized.h        # 未初始化内存操作
│   └── vector.h               # vector 容器
│
├── test/                       # 测试目录
│   ├── CMakeLists.txt
│   ├── test_vector.cpp
│   └── testSimpleVectorAndSimpleAllcator.cpp
│
├── examples/                   # 示例程序
│   ├── CMakeLists.txt
│   ├── example_vector.cpp
│   └── example_allocator.cpp
│
├── CMakeLists.txt             # 根 CMake 配置
├── build.sh                   # Linux/macOS 构建脚本
├── build.bat                  # Windows 构建脚本
└── README.md                  # 本文档
```

详细的项目结构说明请参考 [PROJECT_STRUCTURE.md](docs/PROJECT_STRUCTURE.md)。

---

## 面试准备

### 核心知识点

<details>
<summary><b>1. Allocator 设计</b></summary>

- **Q**: 为什么要分离内存分配和对象构造？
- **A**: 容器需要预先分配内存但不立即构造对象，提高效率和灵活性

- **Q**: 什么是 placement new？
- **A**: 在已分配的内存上构造对象，语法：`new (ptr) T(args...)`

- **Q**: 完美转发的作用是什么？
- **A**: 保持参数的值类别（左值/右值），避免不必要的拷贝
</details>

<details>
<summary><b>2. Vector 实现</b></summary>

- **Q**: vector 的扩容机制是什么？
- **A**: 通常采用 2 倍增长策略，保证 push_back 的均摊时间复杂度为 O(1)

- **Q**: 什么情况下 vector 的迭代器会失效？
- **A**: 插入、删除、扩容操作都可能导致迭代器失效

- **Q**: emplace_back 和 push_back 的区别？
- **A**: emplace_back 直接在容器内构造对象，避免临时对象的创建和拷贝
</details>

<details>
<summary><b>3. Type Traits</b></summary>

- **Q**: 什么是 SFINAE？
- **A**: Substitution Failure Is Not An Error，模板替换失败不是错误，用于条件编译

- **Q**: enable_if 如何实现条件编译？
- **A**: 通过 SFINAE 机制，当条件为 false 时，模板替换失败，该重载被排除

- **Q**: type_traits 在 STL 中的优化作用？
- **A**: 编译期判断类型特性，选择最优实现（如 POD 类型使用 memcpy）
</details>

<details>
<summary><b>4. 迭代器系统</b></summary>

- **Q**: 五种迭代器的区别？
- **A**: 功能递增：Input/Output → Forward → Bidirectional → Random Access

- **Q**: 为什么需要 iterator_traits？
- **A**: 统一接口，让算法同时支持迭代器和原生指针

- **Q**: 什么是 Tag Dispatch？
- **A**: 根据类型标签在编译期选择不同的函数重载，实现算法优化
</details>

### 常见面试题

- vector 的扩容机制是什么？
- emplace_back 和 push_back 的区别？
- 为什么需要 allocator？
- 什么是迭代器失效？
- 如何实现一个线程安全的容器？
- 红黑树的五个性质是什么？
- map vs unordered_map 的区别？

---

## 📋 待完成功能

### 容器
- [ ] deque - 双端队列
- [ ] list - 双向链表
- [ ] map/set - 基于红黑树
- [ ] unordered_map/set - 基于哈希表
- [ ] stack/queue - 容器适配器

### 算法
- [ ] 排序算法（sort, stable_sort, partial_sort）
- [ ] 查找算法（find, binary_search, lower_bound）
- [ ] 堆算法（make_heap, push_heap, pop_heap）
- [ ] 数值算法（accumulate, inner_product）

### 高级特性
- [ ] 内存池
- [ ] 线程安全容器
- [ ] 异常安全保证
- [ ] 性能基准测试

---

## 项目亮点

### 1. 教育性设计
- ✅ 清晰的代码注释
- ✅ 详细的实现说明
- ✅ 完整的测试用例
- ✅ 面试要点总结

### 2. 现代 C++ 特性
- ✅ 右值引用和移动语义
- ✅ 完美转发
- ✅ 变参模板
- ✅ SFINAE 和 enable_if

### 3. 性能优化
- ✅ POD 类型优化（memcpy/memmove）
- ✅ Tag Dispatch（编译期分发）
- 🔄 小对象优化（计划中）
- 🔄 内存池（计划中）

---

## 参考资料

### 书籍
- 《STL 源码剖析》- 侯捷（必读 ⭐⭐⭐⭐⭐）
- 《Effective STL》- Scott Meyers
- 《C++ Templates》- David Vandevoorde
- 《C++ Primer》- Stanley Lippman

### 在线资源
- [cppreference.com](https://en.cppreference.com/) - C++ 标准库参考
- [CppCon](https://www.youtube.com/user/CppCon) - C++ 会议视频
- [GitHub](https://github.com/topics/stl) - 其他 STL 实现

---

## 贡献指南

欢迎贡献代码、报告问题或提出建议！

### 如何贡献

1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

### 代码规范

- 遵循 C++11/14/17 标准
- 使用清晰的命名（小写字母 + 下划线）
- 添加必要的注释和文档
- 编写单元测试

---

## 项目进度

| 模块 | 完成度 | 状态 |
|------|--------|------|
| 基础设施 | 100% | ✅ 完成 |
| vector | 100% | ✅ 完成 |
| string | 20% | 🔄 进行中 |
| rb_tree | 60% | 🔄 进行中 |
| 其他容器 | 0% | 📋 计划中 |
| **总体** | **60%** | **🔄 进行中** |

---

## 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件

---

## 作者

- **GitHub**: [@allenmirac](https://github.com/allenmirac)
- **项目地址**: [TinySTL](https://github.com/allenmirac/TinySTL)

---

## 致谢

- 感谢侯捷老师的《STL 源码剖析》
- 感谢所有为 C++ 社区做出贡献的开发者
- 感谢所有 Star 和 Fork 本项目的朋友

---

<div align="center">

**如果这个项目对你有帮助，请给一个 ⭐ Star！**

Made with ❤️ by [allenmirac](https://github.com/allenmirac)

**开始你的 STL 学习之旅吧！** 
</div>
