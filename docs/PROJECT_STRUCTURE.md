# TinySTL 项目结构说明

## 目录结构

```
TinySTL/
├── include/                    # 头文件目录（建议）
│   ├── allocator.h            # 空间配置器
│   ├── algorithm.h            # 算法库
│   ├── iterator.h             # 迭代器系统
│   ├── type_traits.h          # 类型特性
│   ├── utility.h              # 工具函数
│   ├── uninitialized.h        # 未初始化内存操作
│   ├── vector.h               # vector 容器
│   └── ...                    # 其他头文件
│
├── test/                       # 测试目录
│   ├── CMakeLists.txt         # 测试构建配置
│   ├── test_vector.cpp        # vector 测试
│   └── testSimpleVectorAndSimpleAllcator.cpp  # allocator 测试
│
├── examples/                   # 示例程序目录
│   ├── CMakeLists.txt         # 示例构建配置
│   ├── example_vector.cpp     # vector 使用示例
│   └── example_allocator.cpp  # allocator 使用示例
│
├── string/                     # string 实现（待整合）
│   ├── mystring.h
│   └── mystring.cpp
│
├── RB_Tree/                    # 红黑树实现（待整合）
│   ├── RB_Tree.h
│   ├── RB_Tree_Node.h
│   └── test.cc
│
├── docs/                       # 文档目录
│   ├── SKILLS.md              # 学习技能文档
│   ├── PROJECT_ANALYSIS.md    # 项目分析报告
│   └── C++面试题-move与引用折叠.md
│
├── CMakeLists.txt             # 根 CMake 配置文件
├── build.sh                   # Linux/macOS 构建脚本
├── build.bat                  # Windows 构建脚本
├── .gitignore                 # Git 忽略文件
├── LICENSE                    # 许可证
└── README.md                  # 项目说明文档
```

## 当前状态

### 已完成的文件

#### 核心头文件（当前在根目录，建议移至 include/）
- `type_traits.h` - 类型特性系统
- `iterator.h` - 迭代器系统
- `allocator.h` - 空间配置器
- `utility.h` - 工具函数（move, forward, swap, pair）
- `uninitialized.h` - 未初始化内存操作
- `algorithm.h` - 基础算法
- `vector.h` - 完整的 vector 实现

#### 测试文件
- `test/test_vector.cpp` - 完整的 vector 测试套件
- `test/testSimpleVectorAndSimpleAllcator.cpp` - allocator 测试

#### 示例程序
- `examples/example_vector.cpp` - vector 使用示例
- `examples/example_allocator.cpp` - allocator 使用示例

#### 构建系统
- `CMakeLists.txt` - 根 CMake 配置
- `test/CMakeLists.txt` - 测试构建配置
- `examples/CMakeLists.txt` - 示例构建配置
- `build.sh` - Linux/macOS 构建脚本
- `build.bat` - Windows 构建脚本

#### 文档
- `README.md` - 项目说明
- `SKILLS.md` - 学习路线图
- `PROJECT_ANALYSIS.md` - 项目分析
- `PROJECT_STRUCTURE.md` - 本文档

### 待整合的模块

#### string 模块
- 位置：`string/` 目录
- 状态：基础框架完成，需要完善
- 建议：
  1. 完善 string 实现
  2. 移动到 `include/` 目录
  3. 添加测试用例

#### RB_Tree 模块
- 位置：`RB_Tree/` 目录
- 状态：基础实现完成，需要重构
- 建议：
  1. 重构为 STL 风格
  2. 移动到 `include/` 目录
  3. 添加完整的测试

## 建议的目录重组

### 方案 1：Header-Only Library（推荐）

```
TinySTL/
├── include/
│   └── tinystl/
│       ├── allocator.h
│       ├── algorithm.h
│       ├── iterator.h
│       ├── type_traits.h
│       ├── utility.h
│       ├── uninitialized.h
│       ├── vector.h
│       ├── string.h
│       ├── rb_tree.h
│       ├── map.h
│       └── set.h
├── test/
├── examples/
├── docs/
└── CMakeLists.txt
```

**优点**：
- 清晰的头文件组织
- 易于安装和使用
- 符合现代 C++ 项目规范

### 方案 2：分模块组织

```
TinySTL/
├── include/
│   └── tinystl/
│       ├── core/           # 核心组件
│       │   ├── allocator.h
│       │   ├── iterator.h
│       │   └── type_traits.h
│       ├── container/      # 容器
│       │   ├── vector.h
│       │   ├── string.h
│       │   └── rb_tree.h
│       └── algorithm/      # 算法
│           └── algorithm.h
├── test/
├── examples/
└── docs/
```

**优点**：
- 模块化清晰
- 易于维护和扩展
- 适合大型项目

## 迁移步骤

### 步骤 1：创建 include 目录结构

```bash
mkdir -p include/tinystl
```

### 步骤 2：移动头文件

```bash
# 移动核心头文件
mv *.h include/tinystl/

# 或者使用 Git
git mv *.h include/tinystl/
```

### 步骤 3：更新 CMakeLists.txt

已经配置好，无需修改。CMakeLists.txt 已经指向 `include/` 目录。

### 步骤 4：更新 #include 路径

在所有源文件中，将：
```cpp
#include "vector.h"
```

改为：
```cpp
#include "tinystl/vector.h"
```

或者保持不变（如果使用 `include_directories`）。

## 使用方式

### 方式 1：使用 CMake 构建

```bash
# Linux/macOS
./build.sh

# Windows
build.bat

# 或手动
mkdir build && cd build
cmake ..
cmake --build .
ctest
```

### 方式 2：直接包含头文件

```cpp
// 如果头文件在根目录
#include "vector.h"
#include "allocator.h"

// 如果头文件在 include/tinystl/
#include "tinystl/vector.h"
#include "tinystl/allocator.h"
```

### 方式 3：作为子模块使用

```cmake
# 在你的 CMakeLists.txt 中
add_subdirectory(TinySTL)
target_link_libraries(your_target PRIVATE TinySTL)
```

## 开发建议

### 添加新容器

1. 在 `include/tinystl/` 创建头文件（如 `deque.h`）
2. 在 `test/` 创建测试文件（如 `test_deque.cpp`）
3. 在 `examples/` 创建示例（如 `example_deque.cpp`）
4. 更新 `test/CMakeLists.txt` 和 `examples/CMakeLists.txt`

### 添加新算法

1. 在 `algorithm.h` 中添加实现
2. 在 `test/` 创建测试
3. 在 `examples/` 创建示例

### 文档更新

- 更新 `README.md` - 项目概述和使用说明
- 更新 `SKILLS.md` - 学习路线和技能点
- 更新 `PROJECT_ANALYSIS.md` - 项目分析和改进建议

## 项目统计

### 代码行数（估算）

| 模块 | 文件 | 行数 | 状态 |
|------|------|------|------|
| type_traits | type_traits.h | ~150 |  完成 |
| iterator | iterator.h | ~350 |  完成 |
| allocator | allocator.h | ~100 |  完成 |
| utility | utility.h | ~150 |  完成 |
| uninitialized | uninitialized.h | ~200 |  完成 |
| algorithm | algorithm.h | ~250 |  完成 |
| vector | vector.h | ~800 |  完成 |
| string | string/ | ~300 |  进行中 |
| rb_tree | RB_Tree/ | ~500 |  进行中 |
| **总计** | | **~2800** | **60%** |

### 测试覆盖率

| 模块 | 测试文件 | 覆盖率 | 状态 |
|------|----------|--------|------|
| vector | test_vector.cpp | 90% | 完成 |
| allocator | testSimpleVectorAndSimpleAllcator.cpp | 80% | 完成 |
| string | - | 0% |  待添加 |
| rb_tree | test.cc | 50% |  需完善 |

## 下一步计划

### 短期目标（1-2 周）
1.  创建完整的构建系统
2.  添加示例程序
3. [ ] 重组目录结构（移动头文件到 include/）
4. [ ] 完善 string 实现
5. [ ] 重构 rb_tree

### 中期目标（1 个月）
1. [ ] 实现 deque
2. [ ] 实现 list
3. [ ] 基于 rb_tree 实现 map/set
4. [ ] 添加更多算法
5. [ ] 完善测试覆盖率

### 长期目标（2-3 个月）
1. [ ] 实现哈希表和 unordered_map/set
2. [ ] 添加性能基准测试
3. [ ] 实现内存池
4. [ ] 添加线程安全容器
5. [ ] 生成 API 文档

## 参考资料

- [CMake 官方文档](https://cmake.org/documentation/)
- [Modern CMake](https://cliutils.gitlab.io/modern-cmake/)
- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [STL 源码剖析](https://book.douban.com/subject/1110934/)

---

**最后更新**：2026/3/1
