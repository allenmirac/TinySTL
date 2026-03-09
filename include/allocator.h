#ifndef TINYSTL_ALLOCATOR_H_
#define TINYSTL_ALLOCATOR_H_

// 这个头文件包含一个模板类 allocator，用于管理内存的分配、释放，对象的构造、析构

#include <new>        // for placement new
#include <cstddef>    // for ptrdiff_t, size_t
#include <cstdlib>    // for exit()
#include <climits>    // for UINT_MAX
#include <iostream>   // for cerr

namespace tinystl {

// 模板类: allocator
// 模板函数代表数据类型
template <class T>
class allocator {
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;

public:
    // 分配内存，不构造对象
    // 使用 ::operator new 只分配内存，不调用构造函数
    static T* allocate();
    static T* allocate(size_type n);

    // 释放内存，不析构对象
    // 使用 ::operator delete 只释放内存，不调用析构函数
    static void deallocate(T* ptr);
    static void deallocate(T* ptr, size_type n);

    // 在已分配的内存上构造对象
    // 使用 placement new 在指定内存位置构造对象
    static void construct(T* ptr);
    static void construct(T* ptr, const T& value);
    static void construct(T* ptr, T&& value);

    // 变参模板版本，支持完美转发
    template <class... Args>
    static void construct(T* ptr, Args&&... args);

    // 析构对象，不释放内存
    static void destroy(T* ptr);
    static void destroy(T* first, T* last);
};

// allocate 的实现

// 分配一个对象的内存
template <class T>
T* allocator<T>::allocate() {
    return static_cast<T*>(::operator new(sizeof(T)));
}

// 分配 n 个对象的内存
template <class T>
T* allocator<T>::allocate(size_type n) {
    if (n == 0) {
        return nullptr;
    }
    return static_cast<T*>(::operator new(n * sizeof(T)));
}

// deallocate 的实现

// 释放一个对象的内存
template <class T>
void allocator<T>::deallocate(T* ptr) {
    if (ptr == nullptr) {
        return;
    }
    ::operator delete(ptr);
}

// 释放 n 个对象的内存
template <class T>
void allocator<T>::deallocate(T* ptr, size_type n) {
    (void)n;
    if (ptr == nullptr) {
        return;
    }
    ::operator delete(ptr);
}

// construct 的实现

// 默认构造
template <class T>
void allocator<T>::construct(T* ptr) {
    ::new ((void*)ptr) T();
}

// 拷贝构造
template <class T>
void allocator<T>::construct(T* ptr, const T& value) {
    ::new ((void*)ptr) T(value);
}

// 移动构造
template <class T>
void allocator<T>::construct(T* ptr, T&& value) {
    ::new ((void*)ptr) T(std::move(value));
}

// 完美转发版本，支持任意参数
template <class T>
template <class... Args>
void allocator<T>::construct(T* ptr, Args&&... args) {
    ::new ((void*)ptr) T(std::forward<Args>(args)...);
}

// destroy 的实现

// 析构单个对象
template <class T>
void allocator<T>::destroy(T* ptr) {
    ptr->~T();
}

// 析构一个范围内的对象
template <class T>
void allocator<T>::destroy(T* first, T* last) {
    for (; first != last; ++first) {
        first->~T();
    }
}

} // namespace tinystl

#endif // TINYSTL_ALLOCATOR_H_
