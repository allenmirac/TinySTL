# TinySTL
STL标准库，STL主要包括内存管理、迭代器、容器、算法等，实现的步骤主要包括：命名空间（tinystl）、基础工具（move/pair/forward/swap）、allocator、iterator，接下来才可以继续去实现容器以及算法。

## Phase 1（1～2 周）

1. type_traits（最小集）

2. allocator

3. uninitialized_xxx

4. vector

5. string

Phase 2（进阶）

6. deque 

7. map / set = RB-tree + allocator + iterator

# 基础设施
## 命名空间

```cpp
namespace tinystl{

}
```

## 基础工具

## type_traits

Type Traits（类型特性） 是C++模板元编程的核心组件，用于在编译期查询和操作数据的类型，本身并不操作数值。

可以把他理解为类型的

实现vector/string会使用到的7个traits：
```cpp
integral_constant
true_type/false_type
enable_if
is_same
is_trivially_destructible
is_trivially_copyable
```

### integral_constant

std::integral_constant 是一个类型模板类，它把编译期的常量封装成一个类型，它接收两个参数：
* 一个内置类型Type（int、bool、double等）
* 一个常量值Value
在模板元编程中，常常需要在编译期根据不同的类别实现不同的编译期决策，而这些条件往往是常量值。std::integral_constant提供了一个标准化的手段，让编译器能以类型为单位处理这些常量，避免了运行时开销，也让模板逻辑更清晰、更强大。

### true_type/false_type
定义true和false，继承于integral_constant

### enable_if
enable_if 是 C++ 模板元编程中的条件编译工具，用于根据条件启用或禁用模板特化/重载。

### is_same
is_same 是一个编译期类型比较工具，用于检查两个类型是否完全相同。

### is_trivially_destructible
判断类型析构是否“什么都不用做”

### is_trivially_copyable
判断对象能否用字节拷贝

# allocator
allocator在STL中的定位，它只负责原始内存的获取与释放，本身不保存状态，对象的生命周期由它自己的构造和析构函数来控制。

allocator不是内存池，不负责对象的拷贝和移动操作，是最底层的基础设施。

# iterator

# 容器

## 第一阶段 vector and string

# 算法
