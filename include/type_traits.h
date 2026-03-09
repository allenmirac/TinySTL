#ifndef TINYSTL_TYPE_TRAITS_H_
#define TINYSTL_TYPE_TRAITS_H_

// 这个头文件用于提取类型信息

#include <type_traits>

namespace tinystl{
// helper strutc
template <class T, T _val>
struct m_integral_constant
{
    static constexpr T value = _val;
    using value_type = T;
    using type = m_integral_constant;
    constexpr operator value_type() { return value; }
};

// m_bool_constant —— m_integral_constant<bool>类的别名，编译期分支标签
template <bool _val>
using m_bool_constant = m_integral_constant<bool, _val>;

using m_true_type = m_bool_constant<true>;
using m_false_type = m_bool_constant<false>;

// enable_if —— 模板的 if
template <bool, class T = void>
struct enable_if {};

template <class T>
struct enable_if<true, T> {
    using type = T;
};

// C++14
template <bool _val, class T = void>
using enable_if_t = typename enable_if<_val, T>::type;
// C++17 还增加了概念检查

// is_same —— 类型系统的 ==
template <class T, class U>
struct is_same : m_false_type {};

template <class T>
struct is_same<T, T> : m_true_type {};

// is_trivially_destructible —— 析构是否循环调用
// 默认 false
template <class T>
struct is_trivially_destructible : m_false_type {};
// 特化
template <> struct is_trivially_destructible<int> : m_true_type {};
template <> struct is_trivially_destructible<char> : m_true_type {};
template <> struct is_trivially_destructible<float> : m_true_type {};

// is_trivially_copyable —— 能不能 memcpy
// 默认 false
template <class T>
struct is_trivially_copyable : m_false_type {};
// 特化
template <> struct is_trivially_copyable<int> : m_true_type {};
template <> struct is_trivially_copyable<char> : m_true_type {};
template <> struct is_trivially_copyable<double> : m_true_type {};


template <class T1, class T2>
struct pair;

template <class T>
struct is_pair : tinystl::m_false_type {};

template <class T1, class T2>
struct is_pair<tinystl::pair<T1, T2>> : tinystl::m_true_type {};
} // namesapce tinystl

#endif // TINYSTL_TYPE_TRAITS_H_