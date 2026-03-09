#ifndef TINYSTL_ALGORITHM_H_
#define TINYSTL_ALGORITHM_H_

// 这个头文件包含了 tinystl 的一系列算法

#include "iterator.h"
#include "utility.h"

namespace tinystl {

/*****************************************************************************************/
// max
// 取二者中的较大值，语义相等时保证返回第一个参数
/*****************************************************************************************/
template <class T>
const T& max(const T& lhs, const T& rhs) {
    return lhs < rhs ? rhs : lhs;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class T, class Compare>
const T& max(const T& lhs, const T& rhs, Compare comp) {
    return comp(lhs, rhs) ? rhs : lhs;
}

/*****************************************************************************************/
// min 
// 取二者中的较小值，语义相等时保证返回第一个参数
/*****************************************************************************************/
template <class T>
const T& min(const T& lhs, const T& rhs) {
    return rhs < lhs ? rhs : lhs;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class T, class Compare>
const T& min(const T& lhs, const T& rhs, Compare comp) {
    return comp(rhs, lhs) ? rhs : lhs;
}

/*****************************************************************************************/
// copy
// 把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
/*****************************************************************************************/
template <class InputIter, class OutputIter>
OutputIter copy(InputIter first, InputIter last, OutputIter result) {
    for (; first != last; ++first, ++result) {
        *result = *first;
    }
    return result;
}

/*****************************************************************************************/
// copy_backward
// 将 [first, last)区间内的元素拷贝到 [result - (last - first), result)内
/*****************************************************************************************/
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2
copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
              BidirectionalIter2 result) {
    while (first != last)
        *--result = *--last;
    return result;
}

/*****************************************************************************************/
// copy_n
// 把 [first, first + n)区间内的元素拷贝到 [result, result + n)内
// 返回一个 pair 分别指向拷贝结束的尾部
/*****************************************************************************************/
template <class InputIter, class Size, class OutputIter>
pair<InputIter, OutputIter>
copy_n(InputIter first, Size n, OutputIter result) {
    for (; n > 0; --n, ++first, ++result) {
        *result = *first;
    }
    return pair<InputIter, OutputIter>(first, result);
}

/*****************************************************************************************/
// move
// 把 [first, last)区间内的元素移动到 [result, result + (last - first))内
/*****************************************************************************************/
template <class InputIter, class OutputIter>
OutputIter move(InputIter first, InputIter last, OutputIter result) {
    for (; first != last; ++first, ++result) {
        *result = tinystl::move(*first);
    }
    return result;
}

/*****************************************************************************************/
// move_backward
// 将 [first, last)区间内的元素移动到 [result - (last - first), result)内
/*****************************************************************************************/
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2
move_backward(BidirectionalIter1 first, BidirectionalIter1 last,
              BidirectionalIter2 result) {
    while (first != last)
        *--result = tinystl::move(*--last);
    return result;
}

/*****************************************************************************************/
// equal
// 比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
/*****************************************************************************************/
template <class InputIter1, class InputIter2>
bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2) {
    for (; first1 != last1; ++first1, ++first2) {
        if (*first1 != *first2)
            return false;
    }
    return true;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class InputIter1, class InputIter2, class Compare>
bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compare comp) {
    for (; first1 != last1; ++first1, ++first2) {
        if (!comp(*first1, *first2))
            return false;
    }
    return true;
}

/*****************************************************************************************/
// fill_n
// 从 first 位置开始填充 n 个值
/*****************************************************************************************/
template <class OutputIter, class Size, class T>
OutputIter fill_n(OutputIter first, Size n, const T& value) {
    for (; n > 0; --n, ++first) {
        *first = value;
    }
    return first;
}

/*****************************************************************************************/
// fill
// 为 [first, last)区间内的所有元素填充新值
/*****************************************************************************************/
template <class ForwardIter, class T>
void fill(ForwardIter first, ForwardIter last, const T& value) {
    for (; first != last; ++first) {
        *first = value;
    }
}

/*****************************************************************************************/
// lexicographical_compare
// 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
// (1)如果第一序列的元素较小，返回 true ，否则返回 false
// (2)如果到达 last1 而尚未到达 last2 返回 true
// (3)如果到达 last2 而尚未到达 last1 返回 false
// (4)如果同时到达 last1 和 last2 返回 false
/*****************************************************************************************/
template <class InputIter1, class InputIter2>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                              InputIter2 first2, InputIter2 last2) {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2)
            return true;
        if (*first2 < *first1)
            return false;
    }
    return first1 == last1 && first2 != last2;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class InputIter1, class InputIter2, class Compare>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                              InputIter2 first2, InputIter2 last2, Compare comp) {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (comp(*first1, *first2))
            return true;
        if (comp(*first2, *first1))
            return false;
    }
    return first1 == last1 && first2 != last2;
}

/*****************************************************************************************/
// reverse
// 将[first, last)内的元素反转
/*****************************************************************************************/
// reverse_dispatch 的 bidirectional_iterator_tag 版本
template <class BidirectionalIter>
void reverse_dispatch(BidirectionalIter first, BidirectionalIter last,
                      bidireational_iterator_tag) {
    while (true) {
        if (first == last || first == --last)
            return;
        tinystl::swap(*first++, *last);
    }
}

// reverse_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter>
void reverse_dispatch(RandomIter first, RandomIter last,
                      random_access_iterator_tag) {
    while (first < last)
        tinystl::swap(*first++, *--last);
}

template <class BidirectionalIter>
void reverse(BidirectionalIter first, BidirectionalIter last) {
    reverse_dispatch(first, last, iterator_category(first));
}

/*****************************************************************************************/
// distance
// 计算两个迭代器之间的距离
/*****************************************************************************************/
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
    return distance_dispatch(first, last, iterator_category(first));
}

/*****************************************************************************************/
// address_of
// 获取对象的实际地址
/*****************************************************************************************/
template <class T>
constexpr T* address_of(T& value) noexcept {
    return &value;
}

} // namespace tinystl

#endif // TINYSTL_ALGORITHM_H_
