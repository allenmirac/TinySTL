#ifndef TINYSTL_UTILITY_H_
#define TINYSTL_UTILITY_H_

// 这个头文件包含一些通用工具，包括 move, forward, swap, pair 等

#include <cstddef>
#include "type_traits.h"

namespace tinystl {

// move: 将左值转换为右值引用
template <class T>
typename std::remove_reference<T>::type&& move(T&& arg) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(arg);
}

// forward: 完美转发
// 左值引用版本
template <class T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept {
    return static_cast<T&&>(arg);
}

// 右值引用版本
template <class T>
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
    return static_cast<T&&>(arg);
}

// swap: 交换两个对象
template <class T>
void swap(T& lhs, T& rhs) {
    T tmp(tinystl::move(lhs));
    lhs = tinystl::move(rhs);
    rhs = tinystl::move(tmp);
}

// 交换两个数组
template <class T>
void swap(T lhs[], T rhs[], size_t n) {
    for (size_t i = 0; i < n; ++i) {
        tinystl::swap(lhs[i], rhs[i]);
    }
}

// --------------------------------------------------------------------------------------
// pair: 键值对

template <class T1, class T2>
struct pair {
    typedef T1  first_type;
    typedef T2  second_type;

    T1 first;   // 第一个元素
    T2 second;  // 第二个元素

    // 默认构造函数
    template <class U1 = T1, class U2 = T2,
        typename = typename std::enable_if<
            std::is_default_constructible<U1>::value &&
            std::is_default_constructible<U2>::value, void>::type>
    constexpr pair()
        : first(), second() {
    }

    // 隐式构造函数
    template <class U1 = T1, class U2 = T2,
        typename std::enable_if<
            std::is_copy_constructible<U1>::value &&
            std::is_copy_constructible<U2>::value &&
            std::is_convertible<const U1&, T1>::value &&
            std::is_convertible<const U2&, T2>::value, int>::type = 0>
    constexpr pair(const T1& a, const T2& b)
        : first(a), second(b) {
    }

    // 显式构造函数
    template <class U1 = T1, class U2 = T2,
        typename std::enable_if<
            std::is_copy_constructible<U1>::value &&
            std::is_copy_constructible<U2>::value &&
            (!std::is_convertible<const U1&, T1>::value ||
             !std::is_convertible<const U2&, T2>::value), int>::type = 0>
    explicit constexpr pair(const T1& a, const T2& b)
        : first(a), second(b) {
    }

    pair(const pair& rhs) = default;
    pair(pair&& rhs) = default;

    // 隐式移动构造函数
    template <class U1, class U2,
        typename std::enable_if<
            std::is_constructible<T1, U1>::value &&
            std::is_constructible<T2, U2>::value &&
            std::is_convertible<U1, T1>::value &&
            std::is_convertible<U2, T2>::value, int>::type = 0>
    constexpr pair(U1&& a, U2&& b)
        : first(tinystl::forward<U1>(a)),
          second(tinystl::forward<U2>(b)) {
    }

    // 显式移动构造函数
    template <class U1, class U2,
        typename std::enable_if<
            std::is_constructible<T1, U1>::value &&
            std::is_constructible<T2, U2>::value &&
            (!std::is_convertible<U1, T1>::value ||
             !std::is_convertible<U2, T2>::value), int>::type = 0>
    explicit constexpr pair(U1&& a, U2&& b)
        : first(tinystl::forward<U1>(a)),
          second(tinystl::forward<U2>(b)) {
    }

    // 隐式拷贝构造函数（从其他类型的 pair）
    template <class U1, class U2,
        typename std::enable_if<
            std::is_constructible<T1, const U1&>::value &&
            std::is_constructible<T2, const U2&>::value &&
            std::is_convertible<const U1&, T1>::value &&
            std::is_convertible<const U2&, T2>::value, int>::type = 0>
    constexpr pair(const pair<U1, U2>& other)
        : first(other.first),
          second(other.second) {
    }

    // 显式拷贝构造函数（从其他类型的 pair）
    template <class U1, class U2,
        typename std::enable_if<
            std::is_constructible<T1, const U1&>::value &&
            std::is_constructible<T2, const U2&>::value &&
            (!std::is_convertible<const U1&, T1>::value ||
             !std::is_convertible<const U2&, T2>::value), int>::type = 0>
    explicit constexpr pair(const pair<U1, U2>& other)
        : first(other.first),
          second(other.second) {
    }

    // 隐式移动构造函数（从其他类型的 pair）
    template <class U1, class U2,
        typename std::enable_if<
            std::is_constructible<T1, U1>::value &&
            std::is_constructible<T2, U2>::value &&
            std::is_convertible<U1, T1>::value &&
            std::is_convertible<U2, T2>::value, int>::type = 0>
    constexpr pair(pair<U1, U2>&& other)
        : first(tinystl::forward<U1>(other.first)),
          second(tinystl::forward<U2>(other.second)) {
    }

    // 显式移动构造函数（从其他类型的 pair）
    template <class U1, class U2,
        typename std::enable_if<
            std::is_constructible<T1, U1>::value &&
            std::is_constructible<T2, U2>::value &&
            (!std::is_convertible<U1, T1>::value ||
             !std::is_convertible<U2, T2>::value), int>::type = 0>
    explicit constexpr pair(pair<U1, U2>&& other)
        : first(tinystl::forward<U1>(other.first)),
          second(tinystl::forward<U2>(other.second)) {
    }

    // 拷贝赋值运算符
    pair& operator=(const pair& rhs) {
        if (this != &rhs) {
            first = rhs.first;
            second = rhs.second;
        }
        return *this;
    }

    // 移动赋值运算符
    pair& operator=(pair&& rhs) {
        if (this != &rhs) {
            first = tinystl::move(rhs.first);
            second = tinystl::move(rhs.second);
        }
        return *this;
    }

    // 从其他类型的 pair 拷贝赋值
    template <class U1, class U2>
    pair& operator=(const pair<U1, U2>& other) {
        first = other.first;
        second = other.second;
        return *this;
    }

    // 从其他类型的 pair 移动赋值
    template <class U1, class U2>
    pair& operator=(pair<U1, U2>&& other) {
        first = tinystl::forward<U1>(other.first);
        second = tinystl::forward<U2>(other.second);
        return *this;
    }

    ~pair() = default;

    void swap(pair& other) {
        if (this != &other) {
            tinystl::swap(first, other.first);
            tinystl::swap(second, other.second);
        }
    }
};

// 重载比较操作符
template <class T1, class T2>
bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <class T1, class T2>
bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
}

template <class T1, class T2>
bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return !(lhs == rhs);
}

template <class T1, class T2>
bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return rhs < lhs;
}

template <class T1, class T2>
bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return !(rhs < lhs);
}

template <class T1, class T2>
bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return !(lhs < rhs);
}

// 重载 tinystl 的 swap
template <class T1, class T2>
void swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs) {
    lhs.swap(rhs);
}

// 全局函数 make_pair
template <class T1, class T2>
pair<T1, T2> make_pair(T1&& a, T2&& b) {
    return pair<T1, T2>(tinystl::forward<T1>(a), tinystl::forward<T2>(b));
}

} // namespace tinystl

#endif // TINYSTL_UTILITY_H_
