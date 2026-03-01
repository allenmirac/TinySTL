# C++ 面试题总结：move底层实现与引用折叠

> **核心知识点**：右值引用、移动语义、std::move、完美转发、引用折叠规则
> 
> **难度等级**：⭐⭐⭐⭐⭐（高级）
> 
> **适用场景**：C++后端开发、系统编程、性能优化相关岗位

---

## 📚 目录
1. [右值引用基础](#右值引用基础)
2. [std::move底层实现](#stdmove底层实现)
3. [引用折叠规则](#引用折叠规则)
4. [完美转发](#完美转发)
5. [移动语义应用](#移动语义应用)
6. [面试高频问题](#面试高频问题)
7. [实战案例](#实战案例)

---

## 🎯 右值引用基础

### 1. 左值 vs 右值

#### 基本概念
```cpp
// 左值（lvalue）：有名字、可取地址、持久存在
int x = 10;        // x是左值
int* p = &x;       // 可以取地址

// 右值（rvalue）：临时对象、字面量、表达式结果
int y = 20;        // 20是右值
int z = x + y;     // x+y是右值（临时结果）
// int* p = &(x+y); // 错误！右值不能取地址
```

#### 左值引用 vs 右值引用
```cpp
// 左值引用（&）：只能绑定到左值
int x = 10;
int& lref = x;           // ✅ 正确
// int& lref2 = 10;      // ❌ 错误：不能绑定到右值

// const左值引用：可以绑定到右值（延长生命周期）
const int& cref = 10;    // ✅ 正确：临时对象生命周期延长

// 右值引用（&&）：只能绑定到右值
int&& rref = 10;         // ✅ 正确
int&& rref2 = x + 10;    // ✅ 正确
// int&& rref3 = x;      // ❌ 错误：不能绑定到左值
```

### 2. 为什么需要右值引用？

#### 问题：拷贝开销大
```cpp
class BigData {
private:
    int* data_;
    size_t size_;
    
public:
    // 构造函数
    BigData(size_t size) : size_(size) {
        data_ = new int[size];
        std::cout << "构造：分配 " << size << " 个int\n";
    }
    
    // 拷贝构造（深拷贝，开销大）
    BigData(const BigData& other) : size_(other.size_) {
        data_ = new int[size_];
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "拷贝构造：分配并复制 " << size_ << " 个int\n";
    }
    
    // 析构函数
    ~BigData() {
        delete[] data_;
        std::cout << "析构：释放内存\n";
    }
};

// 使用场景
BigData create_data() {
    BigData data(1000000);  // 构造
    return data;            // 返回时拷贝（C++11前）
}

int main() {
    BigData d = create_data();  // 又一次拷贝
    // 总共：1次构造 + 2次拷贝 + 3次析构
    // 性能损失巨大！
}
```

#### 解决方案：移动语义
```cpp
class BigData {
private:
    int* data_;
    size_t size_;
    
public:
    // 构造函数
    BigData(size_t size) : size_(size) {
        data_ = new int[size];
        std::cout << "构造：分配 " << size << " 个int\n";
    }
    
    // 拷贝构造（深拷贝）
    BigData(const BigData& other) : size_(other.size_) {
        data_ = new int[size_];
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "拷贝构造：分配并复制 " << size_ << " 个int\n";
    }
    
    // 移动构造（浅拷贝 + 资源转移）
    BigData(BigData&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        // 窃取资源
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "移动构造：转移资源（零拷贝）\n";
    }
    
    // 析构函数
    ~BigData() {
        delete[] data_;
        std::cout << "析构：释放内存\n";
    }
};

int main() {
    BigData d = create_data();  
    // 总共：1次构造 + 2次移动 + 3次析构
    // 移动几乎零开销！
}
```

---

## 🔧 std::move底层实现

### 1. std::move的本质

#### 源码实现（简化版）
```cpp
// std::move的实现（C++11标准库）
template<typename T>
typename std::remove_reference<T>::type&& move(T&& t) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}
```

#### 关键点解析
```cpp
// 1. std::remove_reference：移除引用
std::remove_reference<int&>::type   // -> int
std::remove_reference<int&&>::type  // -> int
std::remove_reference<int>::type    // -> int

// 2. 添加右值引用
int&& rref = ...;

// 3. static_cast强制类型转换
static_cast<int&&>(x);  // 将左值x转换为右值引用
```

### 2. std::move的作用

#### std::move不移动任何东西！
```cpp
std::vector<int> v1 = {1, 2, 3, 4, 5};
std::vector<int> v2 = std::move(v1);  // std::move只是类型转换

// std::move做的事情：
// 1. 将v1从左值转换为右值引用
// 2. 触发vector的移动构造函数
// 3. 移动构造函数才真正转移资源
```

#### 完整示例
```cpp
#include <iostream>
#include <utility>

class MyString {
private:
    char* data_;
    size_t size_;
    
public:
    // 构造函数
    MyString(const char* str) {
        size_ = strlen(str);
        data_ = new char[size_ + 1];
        strcpy(data_, str);
        std::cout << "构造: " << data_ << "\n";
    }
    
    // 拷贝构造
    MyString(const MyString& other) : size_(other.size_) {
        data_ = new char[size_ + 1];
        strcpy(data_, other.data_);
        std::cout << "拷贝构造: " << data_ << "\n";
    }
    
    // 移动构造
    MyString(MyString&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "移动构造: " << data_ << "\n";
    }
    
    // 拷贝赋值
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            strcpy(data_, other.data_);
            std::cout << "拷贝赋值: " << data_ << "\n";
        }
        return *this;
    }
    
    // 移动赋值
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
            std::cout << "移动赋值: " << data_ << "\n";
        }
        return *this;
    }
    
    ~MyString() {
        delete[] data_;
        std::cout << "析构\n";
    }
};

int main() {
    MyString s1("Hello");
    
    // 拷贝：s1仍然有效
    MyString s2 = s1;
    std::cout << "s1仍然有效\n";
    
    // 移动：s1被"掏空"
    MyString s3 = std::move(s1);
    std::cout << "s1已失效（data_=nullptr）\n";
    
    // 输出：
    // 构造: Hello
    // 拷贝构造: Hello
    // s1仍然有效
    // 移动构造: Hello
    // s1已失效（data_=nullptr）
    // 析构
    // 析构
    // 析构
}
```

### 3. std::move的实现细节

#### 模板参数推导
```cpp
template<typename T>
typename std::remove_reference<T>::type&& move(T&& t) noexcept;

// 示例1：传入左值
int x = 10;
std::move(x);
// T推导为 int&（引用折叠）
// T&& -> int& && -> int&（参数类型）
// remove_reference<int&>::type&& -> int&&（返回类型）

// 示例2：传入右值
std::move(10);
// T推导为 int
// T&& -> int&&（参数类型）
// remove_reference<int>::type&& -> int&&（返回类型）
```

#### 为什么需要remove_reference？
```cpp
// 如果不用remove_reference
template<typename T>
T&& bad_move(T&& t) {
    return static_cast<T&&>(t);
}

int x = 10;
bad_move(x);
// T推导为 int&
// 返回类型：int& &&（引用折叠）-> int&（左值引用！）
// 错误：返回的是左值引用，不是右值引用

// 使用remove_reference
template<typename T>
typename std::remove_reference<T>::type&& good_move(T&& t) {
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}

int x = 10;
good_move(x);
// T推导为 int&
// remove_reference<int&>::type -> int
// 返回类型：int&&（右值引用！）
// 正确：返回右值引用
```

---

## 📐 引用折叠规则

### 1. 什么是引用折叠？

#### 基本规则
```cpp
// C++中不允许"引用的引用"
int x = 10;
int& ref = x;
// int& & ref2 = ref;  // ❌ 错误：不允许引用的引用

// 但在模板推导中会出现"引用的引用"
// 编译器使用引用折叠规则处理
```

#### 引用折叠规则（4种情况）
```cpp
// 规则：只有"右值引用的右值引用"才折叠为右值引用
//      其他情况都折叠为左值引用

T& &   -> T&   // 左值引用 + 左值引用 = 左值引用
T& &&  -> T&   // 左值引用 + 右值引用 = 左值引用
T&& &  -> T&   // 右值引用 + 左值引用 = 左值引用
T&& && -> T&&  // 右值引用 + 右值引用 = 右值引用（唯一例外）

// 记忆口诀：有左则左，全右才右
```

### 2. 引用折叠的应用场景

#### 场景1：std::move
```cpp
template<typename T>
typename std::remove_reference<T>::type&& move(T&& t) noexcept;

// 传入左值
int x = 10;
std::move(x);
// T推导为 int&
// T&& -> int& && -> int&（引用折叠）
// 但返回类型是 remove_reference<int&>::type&& -> int&&

// 传入右值
std::move(10);
// T推导为 int
// T&& -> int&&（无折叠）
// 返回类型是 int&&
```

#### 场景2：完美转发
```cpp
template<typename T>
void wrapper(T&& arg) {
    func(std::forward<T>(arg));
}

// 传入左值
int x = 10;
wrapper(x);
// T推导为 int&
// T&& -> int& && -> int&（引用折叠）
// arg是左值引用

// 传入右值
wrapper(10);
// T推导为 int
// T&& -> int&&（无折叠）
// arg是右值引用
```

### 3. 万能引用（Universal Reference）

#### 什么是万能引用？
```cpp
// 万能引用（转发引用）：T&&形式，且T是推导类型
template<typename T>
void func(T&& arg);  // 万能引用：可以接受左值或右值

// 不是万能引用的情况
void func(int&& arg);  // 普通右值引用：只能接受右值

template<typename T>
void func(std::vector<T>&& arg);  // 普通右值引用：T是推导的，但整体不是T&&
```

#### 万能引用的推导规则
```cpp
template<typename T>
void func(T&& arg) {
    // ...
}

// 传入左值
int x = 10;
func(x);
// T推导为 int&
// T&& -> int& && -> int&（引用折叠）
// arg是左值引用，绑定到x

// 传入右值
func(10);
// T推导为 int
// T&& -> int&&（无折叠）
// arg是右值引用，绑定到临时对象
```

#### 万能引用 vs 右值引用
```cpp
// 万能引用：模板参数 + T&&
template<typename T>
void universal(T&& arg);  // 万能引用

universal(x);   // T=int&, arg是int&
universal(10);  // T=int, arg是int&&

// 右值引用：具体类型 + &&
void rvalue_only(int&& arg);  // 右值引用

rvalue_only(x);   // ❌ 错误：不能绑定左值
rvalue_only(10);  // ✅ 正确：绑定右值
```

---

## 🚀 完美转发

### 1. 什么是完美转发？

#### 问题：转发丢失值类别
```cpp
void process(int& x) {
    std::cout << "左值版本\n";
}

void process(int&& x) {
    std::cout << "右值版本\n";
}

// 不完美的转发
template<typename T>
void bad_wrapper(T&& arg) {
    process(arg);  // 问题：arg是左值（有名字）
}

int x = 10;
bad_wrapper(x);   // 输出：左值版本 ✅
bad_wrapper(10);  // 输出：左值版本 ❌（期望右值版本）
```

#### 解决方案：std::forward
```cpp
// 完美转发
template<typename T>
void good_wrapper(T&& arg) {
    process(std::forward<T>(arg));  // 保持值类别
}

int x = 10;
good_wrapper(x);   // 输出：左值版本 ✅
good_wrapper(10);  // 输出：右值版本 ✅
```

### 2. std::forward的实现

#### 源码实现（简化版）
```cpp
// std::forward的实现
template<typename T>
T&& forward(typename std::remove_reference<T>::type& t) noexcept {
    return static_cast<T&&>(t);
}

template<typename T>
T&& forward(typename std::remove_reference<T>::type&& t) noexcept {
    return static_cast<T&&>(t);
}
```

#### 工作原理
```cpp
template<typename T>
void wrapper(T&& arg) {
    func(std::forward<T>(arg));
}

// 场景1：传入左值
int x = 10;
wrapper(x);
// T推导为 int&
// std::forward<int&>(arg)
// 返回类型：int& &&（引用折叠）-> int&
// 转发为左值引用 ✅

// 场景2：传入右值
wrapper(10);
// T推导为 int
// std::forward<int>(arg)
// 返回类型：int&&
// 转发为右值引用 ✅
```

### 3. std::move vs std::forward

#### 对比表格
| 特性 | std::move | std::forward |
|------|-----------|--------------|
| **作用** | 无条件转换为右值引用 | 有条件转换（保持值类别） |
| **使用场景** | 明确要移动资源 | 完美转发（模板） |
| **参数类型** | 任意类型 | 万能引用（T&&） |
| **返回类型** | 右值引用 | 取决于模板参数 |

#### 代码对比
```cpp
// std::move：无条件转换为右值
template<typename T>
void use_move(T&& arg) {
    func(std::move(arg));  // 总是转发为右值
}

int x = 10;
use_move(x);   // 转发为右值（丢失左值信息）
use_move(10);  // 转发为右值

// std::forward：保持值类别
template<typename T>
void use_forward(T&& arg) {
    func(std::forward<T>(arg));  // 保持原始值类别
}

int x = 10;
use_forward(x);   // 转发为左值 ✅
use_forward(10);  // 转发为右值 ✅
```

---

## 💡 移动语义应用

### 1. 容器优化

#### std::vector的移动
```cpp
#include <vector>
#include <iostream>

class BigObject {
private:
    int* data_;
    size_t size_;
    
public:
    BigObject(size_t size) : size_(size) {
        data_ = new int[size];
        std::cout << "构造 BigObject(" << size << ")\n";
    }
    
    ~BigObject() {
        delete[] data_;
        std::cout << "析构 BigObject\n";
    }
    
    // 拷贝构造
    BigObject(const BigObject& other) : size_(other.size_) {
        data_ = new int[size_];
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "拷贝构造 BigObject\n";
    }
    
    // 移动构造
    BigObject(BigObject&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "移动构造 BigObject\n";
    }
};

int main() {
    std::vector<BigObject> vec;
    
    // 使用emplace_back（直接构造，无拷贝/移动）
    vec.emplace_back(1000);  // 构造
    
    // 使用push_back + 临时对象（移动）
    vec.push_back(BigObject(2000));  // 构造 + 移动
    
    // 使用push_back + 左值（拷贝）
    BigObject obj(3000);
    vec.push_back(obj);  // 拷贝
    
    // 使用push_back + std::move（移动）
    vec.push_back(std::move(obj));  // 移动（obj失效）
}
```

### 2. 智能指针转移

#### unique_ptr的移动
```cpp
#include <memory>
#include <iostream>

class Resource {
public:
    Resource() { std::cout << "Resource 构造\n"; }
    ~Resource() { std::cout << "Resource 析构\n"; }
    void use() { std::cout << "使用 Resource\n"; }
};

int main() {
    // unique_ptr只能移动，不能拷贝
    std::unique_ptr<Resource> ptr1 = std::make_unique<Resource>();
    
    // std::unique_ptr<Resource> ptr2 = ptr1;  // ❌ 错误：不能拷贝
    std::unique_ptr<Resource> ptr2 = std::move(ptr1);  // ✅ 移动
    
    // ptr1已失效
    if (!ptr1) {
        std::cout << "ptr1 is nullptr\n";
    }
    
    // ptr2拥有资源
    ptr2->use();
}
```

### 3. 返回值优化（RVO）

#### RVO vs 移动
```cpp
#include <vector>
#include <iostream>

std::vector<int> create_vector_rvo() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    return vec;  // RVO：直接在调用者的内存中构造，零开销
}

std::vector<int> create_vector_move() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    return std::move(vec);  // ❌ 不推荐：阻止RVO，反而更慢
}

int main() {
    // 编译器优化：RVO（Return Value Optimization）
    auto v1 = create_vector_rvo();  // 零开销
    
    // 不要画蛇添足
    auto v2 = create_vector_move();  // 移动（比RVO慢）
}
```

#### 何时需要std::move？
```cpp
// 场景1：返回成员变量（RVO不适用）
class Container {
private:
    std::vector<int> data_;
    
public:
    std::vector<int> get_data() {
        return std::move(data_);  // ✅ 需要move
    }
};

// 场景2：返回参数（RVO不适用）
std::vector<int> process(std::vector<int> vec) {
    // 处理vec
    return std::move(vec);  // ✅ 需要move
}

// 场景3：返回局部变量（RVO适用）
std::vector<int> create() {
    std::vector<int> vec = {1, 2, 3};
    return vec;  // ✅ 不需要move，RVO自动优化
}
```

---

## 🎤 面试高频问题

### Q1: std::move做了什么？

**标准答案**：
```cpp
// std::move的本质是类型转换
template<typename T>
typename std::remove_reference<T>::type&& move(T&& t) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}

// 作用：
// 1. 将左值转换为右值引用
// 2. 不移动任何数据，只是类型转换
// 3. 触发移动构造/移动赋值函数
// 4. 真正的移动由移动构造/赋值函数完成
```

**关键点**：
- std::move不移动，只是cast
- 移动后的对象处于"有效但未指定"状态
- 必须实现移动构造/赋值函数才有效果

---

### Q2: 引用折叠规则是什么？

**标准答案**：
```cpp
// 4种情况，只有全右才右
T& &   -> T&   // 左 + 左 = 左
T& &&  -> T&   // 左 + 右 = 左
T&& &  -> T&   // 右 + 左 = 左
T&& && -> T&&  // 右 + 右 = 右

// 记忆：有左则左，全右才右
```

**应用场景**：
1. std::move的实现
2. std::forward的实现
3. 万能引用的推导

---

### Q3: 什么是完美转发？

**标准答案**：
```cpp
// 完美转发：保持参数的值类别（左值/右值）
template<typename T>
void wrapper(T&& arg) {
    func(std::forward<T>(arg));  // 保持arg的值类别
}

// 原理：
// 1. T&&是万能引用，可接受左值或右值
// 2. std::forward<T>根据T的类型决定转发方式
// 3. T是左值引用 -> 转发为左值
// 4. T是非引用 -> 转发为右值
```

**关键点**：
- 使用万能引用（T&&）
- 使用std::forward转发
- 保持值类别不变

---

### Q4: 移动构造函数应该注意什么？

**标准答案**：
```cpp
class MyClass {
public:
    // 移动构造函数的要点
    MyClass(MyClass&& other) noexcept  // 1. 标记noexcept
        : data_(other.data_)           // 2. 窃取资源
    {
        other.data_ = nullptr;         // 3. 清空源对象
    }
    
private:
    int* data_;
};
```

**关键点**：
1. **noexcept**：保证不抛异常，容器才会使用移动
2. **窃取资源**：转移指针/句柄，不分配新内存
3. **清空源对象**：防止析构时重复释放
4. **成员逐个移动**：对每个成员调用std::move

---

### Q5: 什么时候会调用移动构造？

**标准答案**：
```cpp
// 1. 返回局部对象（RVO失败时）
MyClass create() {
    MyClass obj;
    return obj;  // 可能移动（如果RVO失败）
}

// 2. 传递临时对象
void func(MyClass obj);
func(MyClass());  // 移动

// 3. 使用std::move
MyClass obj1;
MyClass obj2 = std::move(obj1);  // 移动

// 4. 容器扩容
std::vector<MyClass> vec;
vec.push_back(MyClass());  // 移动

// 5. 初始化列表
MyClass obj = MyClass();  // 移动（或RVO）
```

---

### Q6: std::move vs std::forward的区别？

**标准答案**：

| 特性 | std::move | std::forward |
|------|-----------|--------------|
| **作用** | 无条件转为右值 | 有条件转发 |
| **使用场景** | 明确要移动 | 完美转发 |
| **参数** | 任意类型 | 万能引用 |
| **返回** | 右值引用 | 保持值类别 |

```cpp
// std::move：总是转为右值
int x = 10;
func(std::move(x));  // 转为右值

// std::forward：保持值类别
template<typename T>
void wrapper(T&& arg) {
    func(std::forward<T>(arg));  // 左值->左值，右值->右值
}
```

---

### Q7: 为什么移动后的对象还能用？

**标准答案**：
```cpp
std::vector<int> v1 = {1, 2, 3};
std::vector<int> v2 = std::move(v1);

// v1处于"有效但未指定"状态
// 可以：
v1.clear();        // ✅ 调用成员函数
v1 = {4, 5, 6};    // ✅ 重新赋值
v1.size();         // ✅ 查询状态

// 不应该：
// v1[0];          // ❌ 未定义行为（可能为空）
// v1.push_back(7); // ❌ 未定义行为（可能崩溃）
```

**关键点**：
- 移动后对象仍然有效（可析构）
- 但状态未指定（不要依赖其值）
- 可以重新赋值或调用不依赖状态的函数

---

### Q8: 如何实现移动赋值运算符？

**标准答案**：
```cpp
class MyClass {
public:
    // 移动赋值运算符
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {           // 1. 自赋值检查
            delete[] data_;             // 2. 释放当前资源
            
            data_ = other.data_;        // 3. 窃取资源
            size_ = other.size_;
            
            other.data_ = nullptr;      // 4. 清空源对象
            other.size_ = 0;
        }
        return *this;                   // 5. 返回*this
    }
    
private:
    int* data_;
    size_t size_;
};
```

**关键点**：
1. 自赋值检查（`this != &other`）
2. 释放当前资源
3. 窃取源对象资源
4. 清空源对象
5. 标记noexcept

---

## 🔥 实战案例

### 案例1：自定义String类

```cpp
#include <iostream>
#include <cstring>
#include <utility>

class String {
private:
    char* data_;
    size_t size_;
    
public:
    // 构造函数
    String(const char* str = "") {
        size_ = strlen(str);
        data_ = new char[size_ + 1];
        strcpy(data_, str);
        std::cout << "构造: " << data_ << " (地址: " << (void*)data_ << ")\n";
    }
    
    // 析构函数
    ~String() {
        std::cout << "析构: " << (data_ ? data_ : "nullptr") 
                  << " (地址: " << (void*)data_ << ")\n";
        delete[] data_;
    }
    
    // 拷贝构造
    String(const String& other) : size_(other.size_) {
        data_ = new char[size_ + 1];
        strcpy(data_, other.data_);
        std::cout << "拷贝构造: " << data_ 
                  << " (新地址: " << (void*)data_ << ")\n";
    }
    
    // 移动构造
    String(String&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        std::cout << "移动构造: " << data_ 
                  << " (转移地址: " << (void*)data_ << ")\n";
        other.data_ = nullptr;
        other.size_ = 0;
    }
    
    // 拷贝赋值
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            strcpy(data_, other.data_);
            std::cout << "拷贝赋值: " << data_ << "\n";
        }
        return *this;
    }
    
    // 移动赋值
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            std::cout << "移动赋值: " << other.data_ << "\n";
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }
    
    // 获取C字符串
    const char* c_str() const { return data_ ? data_ : ""; }
};

// 测试函数
String create_string(const char* str) {
    String s(str);
    return s;  // RVO或移动
}

int main() {
    std::cout << "=== 测试1：拷贝 vs 移动 ===\n";
    String s1("Hello");
    String s2 = s1;              // 拷贝构造
    String s3 = std::move(s1);   // 移动构造
    
    std::cout << "\n=== 测试2：返回值优化 ===\n";
    String s4 = create_string("World");  // RVO
    
    std::cout << "\n=== 测试3：容器操作 ===\n";
    std::vector<String> vec;
    vec.push_back(String("C++"));        // 移动
    vec.push_back(create_string("17"));  // RVO + 移动
    
    std::cout << "\n=== 测试4：赋值操作 ===\n";
    String s5("Old");
    s5 = String("New");  // 移动赋值
    
    std::cout << "\n=== 程序结束，开始析构 ===\n";
}
```

### 案例2：智能指针实现

```cpp
#include <iostream>
#include <utility>

template<typename T>
class UniquePtr {
private:
    T* ptr_;
    
public:
    // 构造函数
    explicit UniquePtr(T* ptr = nullptr) : ptr_(ptr) {
        std::cout << "UniquePtr 构造: " << ptr_ << "\n";
    }
    
    // 析构函数
    ~UniquePtr() {
        std::cout << "UniquePtr 析构: " << ptr_ << "\n";
        delete ptr_;
    }
    
    // 禁止拷贝
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;
    
    // 移动构造
    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
        std::cout << "UniquePtr 移动构造: " << ptr_ << "\n";
        other.ptr_ = nullptr;
    }
    
    // 移动赋值
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            std::cout << "UniquePtr 移动赋值: " << other.ptr_ << "\n";
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }
    
    // 解引用
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    
    // 获取原始指针
    T* get() const { return ptr_; }
    
    // 释放所有权
    T* release() {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }
    
    // 重置指针
    void reset(T* ptr = nullptr) {
        delete ptr_;
        ptr_ = ptr;
    }
    
    // 布尔转换
    explicit operator bool() const { return ptr_ != nullptr; }
};

// 工厂函数
template<typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

// 测试类
class Resource {
private:
    int id_;
    
public:
    Resource(int id) : id_(id) {
        std::cout << "Resource(" << id_ << ") 构造\n";
    }
    
    ~Resource() {
        std::cout << "Resource(" << id_ << ") 析构\n";
    }
    
    void use() {
        std::cout << "使用 Resource(" << id_ << ")\n";
    }
};

int main() {
    std::cout << "=== 测试1：基本使用 ===\n";
    UniquePtr<Resource> ptr1 = make_unique<Resource>(1);
    ptr1->use();
    
    std::cout << "\n=== 测试2：移动语义 ===\n";
    UniquePtr<Resource> ptr2 = std::move(ptr1);
    if (!ptr1) {
        std::cout << "ptr1 已失效\n";
    }
    ptr2->use();
    
    std::cout << "\n=== 测试3：容器存储 ===\n";
    std::vector<UniquePtr<Resource>> vec;
    vec.push_back(make_unique<Resource>(2));
    vec.push_back(make_unique<Resource>(3));
    
    std::cout << "\n=== 程序结束 ===\n";
}
```

### 案例3：完美转发工厂

```cpp
#include <iostream>
#include <memory>
#include <utility>

// 产品类
class Product {
private:
    int id_;
    std::string name_;
    
public:
    Product(int id, const std::string& name) 
        : id_(id), name_(name) {
        std::cout << "Product(" << id_ << ", " << name_ << ") 构造\n";
    }
    
    Product(int id, std::string&& name) 
        : id_(id), name_(std::move(name)) {
        std::cout << "Product(" << id_ << ", " << name_ << ") 移动构造\n";
    }
    
    void show() const {
        std::cout << "Product[" << id_ << "]: " << name_ << "\n";
    }
};

// 完美转发工厂
template<typename T, typename... Args>
std::unique_ptr<T> make_product(Args&&... args) {
    std::cout << "工厂：创建产品...\n";
    return std::make_unique<T>(std::forward<Args>(args)...);
}

int main() {
    std::cout << "=== 测试1：传递左值 ===\n";
    std::string name1 = "Laptop";
    auto p1 = make_product<Product>(1, name1);  // 拷贝
    p1->show();
    std::cout << "name1仍然有效: " << name1 << "\n";
    
    std::cout << "\n=== 测试2：传递右值 ===\n";
    auto p2 = make_product<Product>(2, std::string("Phone"));  // 移动
    p2->show();
    
    std::cout << "\n=== 测试3：传递临时对象 ===\n";
    auto p3 = make_product<Product>(3, "Tablet");  // 移动
    p3->show();
}
```

---

## 📊 性能对比

### 拷贝 vs 移动性能测试

```cpp
#include <iostream>
#include <vector>
#include <chrono>

class BigData {
private:
    std::vector<int> data_;
    
public:
    BigData(size_t size) : data_(size, 0) {}
    
    // 拷贝构造
    BigData(const BigData& other) : data_(other.data_) {}
    
    // 移动构造
    BigData(BigData&& other) noexcept : data_(std::move(other.data_)) {}
};

// 性能测试
void test_copy() {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<BigData> vec;
    for (int i = 0; i < 1000; ++i) {
        BigData data(10000);
        vec.push_back(data);  // 拷贝
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "拷贝耗时: " << duration.count() << "ms\n";
}

void test_move() {
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<BigData> vec;
    for (int i = 0; i < 1000; ++i) {
        BigData data(10000);
        vec.push_back(std::move(data));  // 移动
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "移动耗时: " << duration.count() << "ms\n";
}

int main() {
    test_copy();
    test_move();
    
    // 典型输出：
    // 拷贝耗时: 1523ms
    // 移动耗时: 45ms
    // 性能提升：33倍！
}
```

---

## 🎯 总结

### 核心要点

1. **std::move**
   - 本质是类型转换（cast）
   - 不移动任何数据
   - 触发移动构造/赋值

2. **引用折叠**
   - 有左则左，全右才右
   - 用于模板推导
   - 支持万能引用

3. **完美转发**
   - 使用万能引用（T&&）
   - 使用std::forward转发
   - 保持值类别不变

4. **移动语义**
   - 避免深拷贝
   - 转移资源所有权
   - 标记noexcept

### 最佳实践

✅ **推荐做法**
```cpp
// 1. 移动构造标记noexcept
MyClass(MyClass&& other) noexcept;

// 2. 返回局部变量不用std::move（RVO）
MyClass func() {
    MyClass obj;
    return obj;  // 不要return std::move(obj);
}

// 3. 完美转发使用std::forward
template<typename T>
void wrapper(T&& arg) {
    func(std::forward<T>(arg));
}

// 4. 明确移动使用std::move
vec.push_back(std::move(obj));
```

❌ **避免做法**
```cpp
// 1. 返回局部变量使用std::move（阻止RVO）
return std::move(local_var);

// 2. 移动const对象（无效）
const MyClass obj;
func(std::move(obj));  // 仍然拷贝

// 3. 移动后继续使用
auto obj2 = std::move(obj1);
obj1.use();  // 未定义行为

// 4. 忘记标记noexcept
MyClass(MyClass&& other);  // 容器可能不使用移动
```