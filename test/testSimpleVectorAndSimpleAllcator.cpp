#include <iostream>
#include <math.h>
#include <vector>
using namespace std;
namespace tinystl {
template <class T>
class Vector{
public:
    Vector() {
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }
    Vector(int _size) : size_(_size) {
        data_ = new T[size_]; // 问题1：此处做了(1)分配内存，(2)立刻构造size个T对象(默认构造函数)。
        int i=1;
        while(pow(2, i) < size_) {
            i++;
        }
        capacity_ = pow(2, i);
    }
    ~Vector() {
        if(data_ != nullptr) {
            delete[] data_;
            data_ = nullptr;
        }
    }
    void push_back(const T& _data) {
        if(size_+1 > capacity_) {
            resize();
        }
        data_[size_] = _data;
        size_++;
    }
    void pop_back() {
        if(size_ == 0) {
            return;
        }
        --size_; // 问题4：size减少了，但是对象还活着
    }

private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void resize() {
        if(capacity_ == 0) {
            capacity_ = 1;
            data_ = new T[capacity_]; // 问题2：此处做了(1)分配内存，(2)立刻构造capacity个T对象(默认构造函数)。
        } else {
            capacity_ *= 2;
            T* temp = data_;
            data_ = new T[capacity_]; // 问题3：此处做了(1)分配内存，(2)立刻构造capacity个T对象(默认构造函数)。
            for(int i=0; i<size_; i++) {
                data_[i] = temp[i];
            }
            delete[] temp;
            temp = nullptr;
        }
    }
};
class A{
public:
    A() { cout << "constructor A" << endl; }
    ~A() { cout << "deconstructor A" << endl; }
};

template<class T>
struct myallocator{
    static T* allocate(size_t n) {
        // ::opetator new
        // 特点：只负责分配内存，不构造对象
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }
    static void deallocate(T* p, size_t) {
        ::operator delete(p);
    }
    template <class... Args>
    static void construct(T* p, Args&&... args) {
        // placement new d的语法形式:new (placement_address) type (initializers);
        // 在已有内存上构造对象
        // 特点: 不分配内存，只构造对象
        ::new ((void*)p) T(std::forward<Args>(args)...);
    }
    static void destroy(T* p) {
        p->~T();
    }
};

template <class T>
class Vector1 {
public:
    Vector1():size_(0), capacity_(0), data_(nullptr) {}
    explicit Vector1(size_t size) {
        size_ = size;
        capacity_ = round_up_pow2(size);
        data_ = alloc::allocate(capacity_);
        for(size_t i=0; i<size; i++) {
            alloc::construct(data_+i);
        }
    }
    ~Vector1() {
        clear();
        if(data_) {
            alloc::deallocate(data_, capacity_);
        }
    }

    void push_back(const T& a) {
        if(size_+1 > capacity_) {
            grow();
        }
        alloc::construct(data_+size_, a);
        size_++;
    }

    void pop_back() {
        if(size_ == 0) return;
        size_--;
        alloc::destroy(data_+size_);
    }

    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }

private:
    using alloc = myallocator<T>;
    T* data_;
    size_t size_;
    size_t capacity_;
    static size_t round_up_pow2(size_t n) {
        size_t capacity=1;
        while(capacity < n) capacity <<= 1;
        return capacity;
    }
    void grow() {
        size_t new_cap = capacity_==0 ? 1 : capacity_*2;
        T* new_data = alloc::allocate(new_cap);
        for(size_t i=0; i<size_; i++) {
            alloc::construct(new_data+i, std::move(data_[i]));
            alloc::destroy(data_+i);
        }
        if (data_) {
            alloc::deallocate(data_, capacity_);
        }
        data_ = new_data;
        capacity_ = new_cap;
    }

    void clear() {
        for(size_t i=0; i<size_; i++) {
            alloc::destroy(data_+i);
        }
        size_=0;
    }

};

} // namespace tinystl

using namespace tinystl;

int main() {
    Vector1<A> v(1);
    A a1, a2;
    v.push_back(a1);
    v.push_back(a2);
    // v.pop_back();
    // v.pop_back();
    // v.push_back(a1);
    // v.push_back(a1);
    // vector<A> v1(1);
    // A a3, a4;
    // v1.push_back(a3);
    // v1.push_back(a4);
    return 0;
}