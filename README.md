# TinySTL

<div align="center">

![C++](https://img.shields.io/badge/C++-11/14/17-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Status](https://img.shields.io/badge/status-In%20Development-yellow.svg)

**一个用于学习和面试准备的 C++ STL 标准库实现**

[English](#) | [中文文档](#)

</div>

---

## 📖 项目简介

TinySTL 是一个精简的 C++ 标准模板库（STL）实现，旨在帮助深入理解 STL 的设计理念和实现细节。本项目涵盖了 STL 的核心组件：**容器**、**算法**、**迭代器**、**仿函数**、**适配器**和**空间配置器**。

### 🎯 项目目标

- 📚 **学习 STL 源码**：深入理解 STL 的设计思想和实现技巧
- 💼 **面试准备**：掌握常见的 STL 面试题和核心知识点
- 🔧 **实践能力**：通过实现提升 C++ 模板元编程能力
- 🚀 **性能优化**：学习 STL 中的各种优化技术

### ✨ 特性

- ✅ 遵循 C++11/14/17 标准
- ✅ 清晰的代码注释和文档
- ✅ 完整的学习路线图（见 [SKILLS.md](SKILLS.md)）
- ✅ 详细的项目分析报告（见 [PROJECT_ANALYSIS.md](PROJECT_ANALYSIS.md)）
- ✅ 模块化设计，易于学习和扩展

---

## 📂 项目结构

```
TinySTL/
├── type_traits.h              # 类型特性（Type Traits）
├── iterator.h                 # 迭代器系统
├── allocator.h               # 空间配置器（待实现）
├── utility.h                 # 基础工具（待实现）
├── uninitialized.h           # 未初始化内存操作（待实现）
├── vector.h                  # 动态数组（待实现）
├── string/                   # 字符串实现
│   ├── mystring.h
│   └── mystring.cpp
├── RB_Tree/                  # 红黑树实现
│   ├── RB_Tree.h
│   ├── RB_Tree_Node.h
│   └── test.cc
├── test/                     # 测试文件
│   └── testSimpleVectorAndSimpleAllcator.cpp
├── SKILLS.md                 # 学习技能文档
├── PROJECT_ANALYSIS.md       # 项目分析报告
└── README.md                 # 本文件
```

---

## 🚀 快速开始

### 环境要求

- C++11 或更高版本的编译器（GCC 5.0+, Clang 3.4+, MSVC 2015+）
- CMake 3.10+（可选）

### 编译示例

```bash
# 克隆仓库
git clone https://github.com/allenmirac/TinySTL.git
cd TinySTL

# 编译测试文件
g++ -std=c++11 test/testSimpleVectorAndSimpleAllcator.cpp -o test_allocator
./test_allocator

# 编译红黑树测试
g++ -std=c++11 RB_Tree/test.cc -o test_rbtree
./test_rbtree
```

---

## 📚 实现路线图

### ✅ Phase 0: 基础设施（已完成 70%）

#### 已实现
- [x] **命名空间**：`namespace tinystl`
- [x] **Type Traits**：类型特性系统
  - `integral_constant`、`true_type`/`false_type`
  - `enable_if`（SFINAE 条件编译）
  - `is_same`（类型相等性判断）
  - `is_trivially_destructible`（析构特性检测）
  - `is_trivially_copyable`（拷贝特性检测）
- [x] **Iterator**：迭代器系统
  - 五种迭代器类型（Input, Output, Forward, Bidirectional, Random Access）
  - `iterator_traits`（迭代器萃取器）
  - `reverse_iterator`（反向迭代器）
  - `distance` 和 `advance`（tag dispatch 优化）

#### 待完善
- [ ] 扩展更多类型的 traits 特化
- [ ] 修复 iterator.h 中的编译错误

---

### 🔄 Phase 1: 内存管理（1-2 周）

- [ ] **allocator.h**：空间配置器
  - `allocate` / `deallocate`（内存分配/释放）
  - `construct` / `destroy`（对象构造/析构）
  - 分离内存管理和对象生命周期
  
- [ ] **utility.h**：基础工具
  - `move`（移动语义）
  - `forward`（完美转发）
  - `swap`（交换）
  - `pair`（键值对）
  
- [ ] **uninitialized.h**：未初始化内存操作
  - `uninitialized_copy`
  - `uninitialized_fill`
  - `uninitialized_fill_n`
  - `uninitialized_move`

---

### 📦 Phase 2: 序列容器（2-3 周）

- [ ] **vector**：动态数组
  - 完整的构造函数（默认、拷贝、移动、范围、初始化列表）
  - `push_back` / `pop_back` / `emplace_back`
  - `insert` / `erase` / `clear`
  - `resize` / `reserve`
  - 迭代器支持
  - 2 倍容量增长策略
  
- [x] **string**：字符串（基础框架已完成）
  - [ ] 完成所有函数实现
  - [ ] 字符串操作（append, insert, erase, substr, find）
  - [ ] SSO（小字符串优化）
  - [ ] C 风格接口（c_str, data）
  
- [ ] **deque**：双端队列
  - 分段连续存储（中控器 + 缓冲区）
  - 复杂的迭代器实现
  - `push_front` / `push_back`
  - 随机访问支持

---

### 🌲 Phase 3: 关联容器（2-3 周）

- [x] **RB_Tree**：红黑树（基础实现已完成）
  - [x] 插入操作和修复
  - [x] 左旋和右旋
  - [ ] 删除操作完善
  - [ ] 迭代器支持
  - [ ] 查找功能
  - [ ] 重构为 STL 风格
  
- [ ] **map**：映射容器
  - 基于红黑树实现
  - `operator[]`（下标访问）
  - `insert` / `erase` / `find`
  
- [ ] **set**：集合容器
  - 基于红黑树实现
  - 自动排序，不允许重复

---

### 🔧 Phase 4: 算法（1-2 周）

- [ ] **非修改序列操作**
  - `find`, `find_if`, `count`, `count_if`
  - `for_each`, `all_of`, `any_of`, `none_of`
  
- [ ] **修改序列操作**
  - `copy`, `move`, `fill`, `transform`
  - `remove`, `remove_if`, `unique`, `reverse`
  
- [ ] **排序相关**
  - `sort`, `stable_sort`, `partial_sort`
  - `binary_search`, `lower_bound`, `upper_bound`
  
- [ ] **堆操作**
  - `make_heap`, `push_heap`, `pop_heap`, `sort_heap`

---

## 🎓 核心概念详解

### 1️⃣ Type Traits（类型特性）

Type Traits 是 C++ 模板元编程的核心，用于在**编译期**查询和操作类型信息。

#### 为什么需要 Type Traits？

```cpp
// 场景：高效拷贝数据
template<class T>
void copy_data(T* src, T* dst, size_t n) {
    // POD 类型可以用 memcpy（快）
    if constexpr (is_trivially_copyable<T>::value) {
        memcpy(dst, src, n * sizeof(T));
    } 
    // 非 POD 类型必须逐个拷贝构造（安全）
    else {
        for (size_t i = 0; i < n; ++i) {
            new (dst + i) T(src[i]);
        }
    }
}
```

**关键优势**：
- ✅ 编译期决策，零运行时开销
- ✅ 类型安全
- ✅ 性能优化的基础

#### 已实现的 Traits

| Trait | 作用 | 应用场景 |
|-------|------|----------|
| `integral_constant` | 将编译期常量封装为类型 | 模板元编程基础 |
| `true_type` / `false_type` | 编译期布尔标签 | Tag Dispatch |
| `enable_if` | SFINAE 条件编译 | 约束模板参数 |
| `is_same` | 类型相等性判断 | 类型检查 |
| `is_trivially_destructible` | 判断是否需要显式析构 | 优化析构循环 |
| `is_trivially_copyable` | 判断是否可用 memcpy | 优化拷贝操作 |

---

### 2️⃣ Iterator（迭代器）

迭代器是连接**容器**和**算法**的桥梁，提供统一的访问接口。

#### 五种迭代器类型

```
Input Iterator          ← 单向只读
Output Iterator         ← 单向只写
Forward Iterator        ← 单向读写
Bidirectional Iterator  ← 双向读写
Random Access Iterator  ← 随机访问
```

#### Iterator Traits（迭代器萃取）

**问题**：如何让算法同时支持迭代器和原生指针？

**解决方案**：使用 `iterator_traits` 萃取类型信息

```cpp
template<class Iterator>
typename iterator_traits<Iterator>::difference_type
distance(Iterator first, Iterator last) {
    // 根据迭代器类型选择最优实现
    return distance_dispatch(first, last, 
        typename iterator_traits<Iterator>::iterator_category());
}

// 原生指针也能工作（通过偏特化）
int arr[10];
int* p1 = arr;
int* p2 = arr + 5;
auto d = distance(p1, p2);  // ✅ 正确工作
```

#### Tag Dispatch 优化

```cpp
// Random Access Iterator：O(1)
template<class RandomIter>
auto distance_dispatch(RandomIter first, RandomIter last, 
                       random_access_iterator_tag) {
    return last - first;  // 直接相减
}

// Input Iterator：O(n)
template<class InputIter>
auto distance_dispatch(InputIter first, InputIter last, 
                       input_iterator_tag) {
    auto n = 0;
    while (first != last) {  // 遍历计数
        ++first;
        ++n;
    }
    return n;
}
```

---

### 3️⃣ Allocator（空间配置器）

Allocator 负责**内存管理**，是 STL 容器的基础设施。

#### 为什么需要 Allocator？

**问题**：`new` 和 `delete` 的局限性

```cpp
// new 做了两件事：
T* p = new T();
// 等价于：
T* p = (T*)operator new(sizeof(T));  // 1️⃣ 分配内存
new (p) T();                          // 2️⃣ 构造对象

// 容器的需求：分离这两个操作
vector<T> v;
v.reserve(100);  // 只想分配内存，不想构造 100 个对象 ❌
v.push_back(x);  // 只想在已有内存上构造 1 个对象 ❌
```

#### Allocator 的解决方案

```cpp
// 四个核心函数
T* memory = allocator<T>::allocate(100);     // 1️⃣ 只分配内存
allocator<T>::construct(memory, args...);    // 2️⃣ 在指定位置构造
allocator<T>::destroy(memory);               // 3️⃣ 只析构对象
allocator<T>::deallocate(memory, 100);       // 4️⃣ 只释放内存
```

**关键技术**：
- `::operator new`：只分配内存，不构造对象
- `placement new`：在已有内存上构造对象
- 完美转发：`std::forward<Args>(args)...`

---

## 📝 学习资源

### 📖 推荐阅读

1. **本项目文档**
   - [SKILLS.md](SKILLS.md) - 完整的学习路线图和技能文档
   - [PROJECT_ANALYSIS.md](PROJECT_ANALYSIS.md) - 项目现状分析和改进建议
   - [C++面试题-move与引用折叠.md](C++面试题-move与引用折叠.md) - 移动语义详解

2. **经典书籍**
   - 《STL 源码剖析》 - 侯捷（必读 ⭐⭐⭐⭐⭐）
   - 《Effective STL》 - Scott Meyers
   - 《C++ Templates》 - David Vandevoorde
   - 《C++ Primer》 - Stanley Lippman

3. **在线资源**
   - [cppreference.com](https://en.cppreference.com/) - C++ 标准库参考
   - [CppCon](https://www.youtube.com/user/CppCon) - C++ 会议视频
   - [GitHub](https://github.com/topics/stl) - 其他 STL 实现

---

## 🎯 面试准备

### 常见面试题

<details>
<summary><b>1. Vector 相关</b></summary>

- **Q**: vector 的扩容机制是什么？
- **A**: 通常采用 2 倍增长策略，保证 `push_back` 的均摊时间复杂度为 O(1)

- **Q**: 什么情况下 vector 的迭代器会失效？
- **A**: 插入、删除、扩容操作都可能导致迭代器失效

- **Q**: `emplace_back` 和 `push_back` 的区别？
- **A**: `emplace_back` 直接在容器内构造对象，避免临时对象的创建和拷贝
</details>

<details>
<summary><b>2. Iterator 相关</b></summary>

- **Q**: 五种迭代器的区别？
- **A**: 功能递增：Input/Output → Forward → Bidirectional → Random Access

- **Q**: 为什么需要 `iterator_traits`？
- **A**: 统一接口，让算法同时支持迭代器和原生指针

- **Q**: 什么是 Tag Dispatch？
- **A**: 根据类型标签在编译期选择不同的函数重载，实现算法优化
</details>

<details>
<summary><b>3. 内存管理相关</b></summary>

- **Q**: allocator 的作用是什么？
- **A**: 分离内存分配和对象构造，提供更灵活的内存管理

- **Q**: 什么是 placement new？
- **A**: 在已分配的内存上构造对象，语法：`new (ptr) T(args...)`

- **Q**: 什么是内存池？
- **A**: 预先分配大块内存，减少频繁的内存分配/释放开销
</details>

<details>
<summary><b>4. 红黑树相关</b></summary>

- **Q**: 红黑树的五个性质？
- **A**: 1) 节点是红或黑 2) 根是黑 3) 叶子(NIL)是黑 4) 红节点的子节点是黑 5) 任一节点到叶子的路径包含相同数量的黑节点

- **Q**: 红黑树 vs AVL 树？
- **A**: 红黑树插入/删除更快（最多 3 次旋转），AVL 树查找更快（更平衡）

- **Q**: map vs unordered_map？
- **A**: map 基于红黑树（有序，O(log n)），unordered_map 基于哈希表（无序，O(1)）
</details>

---

## 🤝 贡献指南

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

## 📊 项目进度

| 模块 | 状态 | 完成度 | 说明 |
|------|------|--------|------|
| type_traits.h | ✅ 已完成 | 70% | 需扩展更多类型特化 |
| iterator.h | ✅ 已完成 | 75% | 需修复编译错误 |
| allocator.h | 🔄 进行中 | 0% | 待实现 |
| utility.h | 📋 计划中 | 0% | 待实现 |
| vector.h | 📋 计划中 | 0% | 待实现 |
| string | 🔄 进行中 | 20% | 基础框架完成 |
| deque.h | 📋 计划中 | 0% | 待实现 |
| RB_Tree | ✅ 已完成 | 60% | 需重构和完善 |
| map / set | 📋 计划中 | 0% | 待实现 |
| algorithm.h | 📋 计划中 | 0% | 待实现 |

**图例**：✅ 已完成 | 🔄 进行中 | 📋 计划中

---

## 📄 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件

---

## 👨‍💻 作者

- **GitHub**: [@allenmirac](https://github.com/allenmirac)
- **项目地址**: [TinySTL](https://github.com/allenmirac/TinySTL)

---

## 🙏 致谢

- 感谢侯捷老师的《STL 源码剖析》
- 感谢所有为 C++ 社区做出贡献的开发者
- 感谢所有 Star 和 Fork 本项目的朋友

---

<div align="center">

**如果这个项目对你有帮助，请给一个 ⭐ Star！**

Made with ❤️ by [allenmirac](https://github.com/allenmirac)

</div>
