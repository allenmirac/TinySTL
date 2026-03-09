#ifndef TINYSTL_UNINITIALIZED_H_
#define TINYSTL_UNINITIALIZED_H_

// 这个头文件用于对未初始化空间构造元素

#include "allocator.h"
#include "iterator.h"
#include "type_traits.h"
#include "utility.h"
#include <algorithm> // for std::copy_n
#include <cstring>   // for memmove

namespace tinystl {

/*****************************************************************************************/
// uninitialized_copy
// 把 [first, last) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
/*****************************************************************************************/

// POD 类型版本：使用 memmove 优化
template <class InputIter, class ForwardIter>
ForwardIter 
unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, m_true_type) {
    return std::copy(first, last, result);
}

// 非 POD 类型版本：逐个拷贝构造
template <class InputIter, class ForwardIter>
ForwardIter 
unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, m_false_type) {
    auto cur = result;
    try {
        for (; first != last; ++first, ++cur) {
            tinystl::allocator<typename iterator_traits<ForwardIter>::value_type>::
                construct(&*cur, *first);
        }
    }
    catch (...) {
        // 异常安全：如果构造失败，析构已构造的对象
        for (; result != cur; ++result) {
            tinystl::allocator<typename iterator_traits<ForwardIter>::value_type>::
                destroy(&*result);
        }
        throw;
    }
    return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_copy(InputIter first, InputIter last, ForwardIter result) {
    return unchecked_uninit_copy(first, last, result,
        typename is_trivially_copyable<
            typename iterator_traits<ForwardIter>::value_type>::type{});
}

/*****************************************************************************************/
// uninitialized_copy_n
// 把 [first, first + n) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
/*****************************************************************************************/

// POD 类型版本
template <class InputIter, class Size, class ForwardIter>
ForwardIter 
unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result, m_true_type) {
    return std::copy_n(first, n, result);
}

// 非 POD 类型版本
template <class InputIter, class Size, class ForwardIter>
ForwardIter 
unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result, m_false_type) {
    auto cur = result;
    try {
        for (; n > 0; --n, ++first, ++cur) {
            tinystl::allocator<typename iterator_traits<ForwardIter>::value_type>::
                construct(&*cur, *first);
        }
    }
    catch (...) {
        for (; result != cur; ++result) {
            tinystl::allocator<typename iterator_traits<ForwardIter>::value_type>::
                destroy(&*result);
        }
        throw;
    }
    return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter uninitialized_copy_n(InputIter first, Size n, ForwardIter result) {
    return unchecked_uninit_copy_n(first, n, result,
        typename is_trivially_copyable<
            typename iterator_traits<ForwardIter>::value_type>::type{});
}

/*****************************************************************************************/
// uninitialized_fill
// 在 [first, last) 区间内填充元素值
/*****************************************************************************************/

// POD 类型版本
template <class ForwardIter, class T>
void 
unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value, m_true_type) {
    std::fill(first, last, value);
}

// 非 POD 类型版本
template <class ForwardIter, class T>
void 
unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value, m_false_type) {
    auto cur = first;
    try {
        for (; cur != last; ++cur) {
            tinystl::allocator<typename iterator_traits<ForwardIter>::value_type>::
                construct(&*cur, value);
        }
    }
    catch (...) {
        for (; first != cur; ++first) {
            tinystl::allocator<typename iterator_traits<ForwardIter>::value_type>::
                destroy(&*first);
        }
        throw;
    }
}

template <class ForwardIter, class T>
void uninitialized_fill(ForwardIter first, ForwardIter last, const T& value) {
    unchecked_uninit_fill(first, last, value,
        typename is_trivially_copyable<
            typename iterator_traits<ForwardIter>::value_type>::type{});
}

/*****************************************************************************************/
// uninitialized_fill_n
// 从 first 位置开始，填充 n 个元素值，返回填充结束的位置
/*****************************************************************************************/

// POD 类型版本
template <class ForwardIter, class Size, class T>
ForwardIter 
unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, m_true_type) {
    return std::fill_n(first, n, value);
}

// 非 POD 类型版本
template <class ForwardIter, class Size, class T>
ForwardIter 
unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, m_false_type) {
    auto cur = first;
    try {
        for (; n > 0; --n, ++cur) {
            tinystl::allocator<typename iterator_traits<ForwardIter>::value_type>::
                construct(&*cur, value);
        }
    }
    catch (...) {
        for (; first != cur; ++first) {
            tinystl::allocator<typename iterator_traits<ForwardIter>::value_type>::
                destroy(&*first);
        }
        throw;
    }
    return cur;
}

template <class ForwardIter, class Size, class T>
ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& value) {
    return unchecked_uninit_fill_n(first, n, value,
        typename is_trivially_copyable<
            typename iterator_traits<ForwardIter>::value_type>::type{});
}

/*****************************************************************************************/
// uninitialized_move
// 把 [first, last) 上的内容移动到以 result 为起始处的空间，返回移动结束的位置
/*****************************************************************************************/

// POD 类型版本
template <class InputIter, class ForwardIter>
ForwardIter 
unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, m_true_type) {
    return std::move(first, last, result);
}

// 非 POD 类型版本
template <class InputIter, class ForwardIter>
ForwardIter 
unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, m_false_type) {
    auto cur = result;
    try {
        for (; first != last; ++first, ++cur) {
            tinystl::allocator<typename iterator_traits<ForwardIter>::value_type>::
                construct(&*cur, tinystl::move(*first));
        }
    }
    catch (...) {
        for (; result != cur; ++result) {
            tinystl::allocator<typename iterator_traits<ForwardIter>::value_type>::
                destroy(&*result);
        }
        throw;
    }
    return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_move(InputIter first, InputIter last, ForwardIter result) {
    return unchecked_uninit_move(first, last, result,
        typename is_trivially_copyable<
            typename iterator_traits<ForwardIter>::value_type>::type{});
}

/*****************************************************************************************/
// uninitialized_move_n
// 把 [first, first + n) 上的内容移动到以 result 为起始处的空间，返回移动结束的位置
/*****************************************************************************************/

// POD 类型版本
template <class InputIter, class Size, class ForwardIter>
ForwardIter 
unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, m_true_type) {
    return std::move(first, first + n, result);
}

// 非 POD 类型版本
template <class InputIter, class Size, class ForwardIter>
ForwardIter 
unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, m_false_type) {
    auto cur = result;
    try {
        for (; n > 0; --n, ++first, ++cur) {
            tinystl::allocator<typename iterator_traits<ForwardIter>::value_type>::
                construct(&*cur, tinystl::move(*first));
        }
    }
    catch (...) {
        for (; result != cur; ++result) {
            tinystl::allocator<typename iterator_traits<ForwardIter>::value_type>::
                destroy(&*result);
        }
        throw;
    }
    return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter uninitialized_move_n(InputIter first, Size n, ForwardIter result) {
    return unchecked_uninit_move_n(first, n, result,
        typename is_trivially_copyable<
            typename iterator_traits<ForwardIter>::value_type>::type{});
}

} // namespace tinystl

#endif // TINYSTL_UNINITIALIZED_H_
