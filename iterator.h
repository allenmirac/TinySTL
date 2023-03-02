#ifndef MIRTINYSTL_ITERATOR_H_
#define MIRTINYSTL_ITERATOR_H_

// 这个头文件用于迭代器设计，包含了一些模板结构体与全局函数

#include <cstddef>

#include "type_traits.h"

namespace mirstl {
// 五种迭代器类型
// 单向移动只读迭代器 Input Iterator
// 单向移动只写迭代器 Output Iterator
// 单向移动读写迭代器 Forward Iterator
// 双向移动读写迭代器 Bidirectional Iterator
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidireational_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidireational_iterator_tag {};

// iterator 模板
template <class Category, class T, class Diatance = ptrdiff_t,
    class Pointer = T*, class Reference = T&>
    struct iterator{
    typedef Category    iterator_category;  //种类
    typedef T           value_type;         //值类型
    typedef Pointer     pointer;            //指针
    typedef Reference   reference;          //引用
    typedef Distance    difference_type;    
};

//iterator traits

template <class T>
struct has_iterator_cat{
private:
    struct two { char a; char b; };
    template <class U> static two test(...);
    template <class U> static char test(typename U::iterator_category* = 0);
public:
    static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

template <class Iterator, bool>
struct iterator_traits_impl {};

template <class Iterator>
struct iterator_traits_impl<Iterator, true>{
    //如果Iterator有自己自己的value_type,萃取出来的value_type就是 Iterator::value_type.
    typedef typename Iterator::iterator_category    iterator_category;
    typedef typename Iterator::value_type           value_type;
    typedef typename Iterator::pointer              pointer;
    typedef typename Iterator::reference            reference;
    typedef typename Iterator::difference           difference_type; 
};


template <class Iterator, bool>
struct iterator_traits_helper {};

template <class Iterator>
struct iterator_traits_helper<Iterator, true>{
    std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
    std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
};

template <class Iterator>
struct iterator_traits
    : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

// 针对原生指针的偏特化版本
template <class T>
struct iterator_traits<T*>{
    typedef random_access_iterator_tag              iterator_category;
    typedef T                                       value_type;
    typedef const T*                                pointer;
    typedef const T&                                reference;
    typedef ptrdiff_t                               difference_type;
};

template <class T>
struct iterator_traits<const T*>{
    typedef random_access_iterator_tag              iterator_category;
    typedef T                                       value_type;
    typedef const T*                                pointer;
    typedef const T&                                reference;
    typedef ptrdiff_t                               difference_type;
};

template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of
    : public m_bool_constant<std::is_convertible<
    typename iterator_traits<T>::iterator_category, U>::value>{

};

// 萃取某种迭代器
template <class T, class U>
struct has_iterator_cat_of<T, U, false> : public m_false_type {};

template <class Iter>
struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag> {};

template <class Iter>
struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag> {};

template <class Iter>
struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

template <class Iter>
struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidireational_iterator_tag> {};

template <class Iter>
struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

template <class Iterator>
struct is_iterator :
    public m_bool_constant<is_input_iterator<Iterator>::value ||
        is_output_iterator<Iterator>::value>{

};

// 萃取某个迭代器的 category
template <class Iterator>
typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&){
    typedef typename iterator_traits<Iterator>::iterator_category Category;
    return Category();
}

// 萃取某个迭代器的 distance_type
template <class Iterator>
typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&){
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

// 萃取某个迭代器的 value_type
template <class Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&){
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// distance 的 input_iterator_tag 的版本
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag){
    typename iterator_traits<InputIterator>::difference_type n=0;
    while (first != last)
    {
        ++first;
        ++n;
    }
    return n;
}

// distance 的 randon_access_iterator_tag 的版本
template <class RandomIter>
typename iterator_traits<RandomIter>::difference_type
distance_dispatch(RandomIter first, RandomIter last,
                  random_access_iterator_tag){
    return last - first;
}

// advance 的 bidirection_Iterator 的版本
template <class BidirectionalIterator, class Distance>
void advance_dispatch(BidirectionalIterator& i, Distance n, bidireational_iterator_tag){
    if (n >= 0){
        while (n--) ++i;
    }else{
        while (n++) --i;
    }
}

// advance 的 random_access_iterator_tag 的版本
template <class RandomIter, class Distance>
void advance_dispatch(RandomIter& i, Distance n, random_access_iterator_tag){
    i += n;
}

template <class InputIterator, class Distance>
void advance(InputIterator& i, Distance n){
    advance_dispatch(i, n, iterator_category(i));
}

/************************************************************************************/

// 模板类: reverse_iterator
// 代表反向迭代器，使前进为后退，后退为前进
template <class Iterator>
class reverse_iterator{
private:
    Iterator current;       // 记录对应的正向迭代器

public:
    typedef typename iterator_traits<Iterator>::iterator_category   iterator_category;
    typedef typename iterator_traits<Iterator>::value_type          value_type;
    typedef typename iterator_traits<Iterator>::difference_type     difference_type;
    typedef typename iterator_traits<Iterator>::pointer             pointer;
    typedef typename iterator_traits<Iterator>::reference           reference;

    typedef Iterator                                                iterator_type;
    typedef reverse_iterator<Iterator>                              self;

public:
    // 构造函数
    reverse_iterator() {}
};
}
#endif // MIRTINYSTL_ITERATOR_H_