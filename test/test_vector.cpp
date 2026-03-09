#include <iostream>
#include "../include/vector.h"

using namespace std;

// 测试类，用于观察构造和析构
class TestClass {
public:
    int value;
    
    TestClass() : value(0) {
        cout << "Default constructor: " << value << endl;
    }
    
    explicit TestClass(int v) : value(v) {
        cout << "Constructor: " << value << endl;
    }
    
    TestClass(const TestClass& other) : value(other.value) {
        cout << "Copy constructor: " << value << endl;
    }
    
    TestClass(TestClass&& other) noexcept : value(other.value) {
        cout << "Move constructor: " << value << endl;
        other.value = 0;
    }
    
    TestClass& operator=(const TestClass& other) {
        cout << "Copy assignment: " << other.value << endl;
        value = other.value;
        return *this;
    }
    
    TestClass& operator=(TestClass&& other) noexcept {
        cout << "Move assignment: " << other.value << endl;
        value = other.value;
        other.value = 0;
        return *this;
    }
    
    ~TestClass() {
        cout << "Destructor: " << value << endl;
    }
};

void test_basic_operations() {
    cout << "\n=== Test 1: Basic Operations ===" << endl;
    
    // 默认构造
    tinystl::vector<int> v1;
    cout << "v1 size: " << v1.size() << ", capacity: " << v1.capacity() << endl;
    
    // 带大小的构造
    tinystl::vector<int> v2(5);
    cout << "v2 size: " << v2.size() << ", capacity: " << v2.capacity() << endl;
    
    // 带值的构造
    tinystl::vector<int> v3(5, 100);
    cout << "v3 elements: ";
    for (size_t i = 0; i < v3.size(); ++i) {
        cout << v3[i] << " ";
    }
    cout << endl;
}

void test_push_pop() {
    cout << "\n=== Test 2: Push and Pop ===" << endl;
    
    tinystl::vector<int> v;
    cout << "Pushing elements..." << endl;
    for (int i = 1; i <= 5; ++i) {
        v.push_back(i * 10);
        cout << "Pushed " << i * 10 << ", size: " << v.size() 
             << ", capacity: " << v.capacity() << endl;
    }
    
    cout << "Elements: ";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
    
    cout << "Popping elements..." << endl;
    while (!v.empty()) {
        cout << "Pop " << v.back() << ", size before: " << v.size();
        v.pop_back();
        cout << ", size after: " << v.size() << endl;
    }
}

void test_allocator_separation() {
    cout << "\n=== Test 3: Allocator Separation (Memory vs Construction) ===" << endl;
    cout << "Creating vector with 3 TestClass objects..." << endl;
    
    tinystl::vector<TestClass> v;
    v.push_back(TestClass(1));
    v.push_back(TestClass(2));
    v.push_back(TestClass(3));
    
    cout << "\nVector size: " << v.size() << ", capacity: " << v.capacity() << endl;
    cout << "Note: Only 3 objects constructed, not " << v.capacity() << endl;
    
    cout << "\nPopping one element..." << endl;
    v.pop_back();
    cout << "Size: " << v.size() << ", capacity: " << v.capacity() << endl;
    
    cout << "\nDestroying vector..." << endl;
}

void test_copy_and_move() {
    cout << "\n=== Test 4: Copy and Move ===" << endl;
    
    tinystl::vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    
    cout << "v1: ";
    for (size_t i = 0; i < v1.size(); ++i) {
        cout << v1[i] << " ";
    }
    cout << endl;
    
    // 拷贝构造
    cout << "\nCopy constructing v2 from v1..." << endl;
    tinystl::vector<int> v2(v1);
    cout << "v2: ";
    for (size_t i = 0; i < v2.size(); ++i) {
        cout << v2[i] << " ";
    }
    cout << endl;
    
    // 移动构造
    cout << "\nMove constructing v3 from v1..." << endl;
    tinystl::vector<int> v3(tinystl::move(v1));
    cout << "v3: ";
    for (size_t i = 0; i < v3.size(); ++i) {
        cout << v3[i] << " ";
    }
    cout << endl;
    cout << "v1 size after move: " << v1.size() << endl;
}

void test_insert_erase() {
    cout << "\n=== Test 5: Insert and Erase ===" << endl;
    
    tinystl::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(5);
    
    cout << "Initial: ";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
    
    // 插入
    cout << "Inserting 3 and 4..." << endl;
    v.insert(v.begin() + 2, 3);
    v.insert(v.begin() + 3, 4);
    
    cout << "After insert: ";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
    
    // 删除
    cout << "Erasing element at index 1..." << endl;
    v.erase(v.begin() + 1);
    
    cout << "After erase: ";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
}

void test_resize() {
    cout << "\n=== Test 6: Resize ===" << endl;
    
    tinystl::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    cout << "Initial size: " << v.size() << endl;
    
    cout << "Resizing to 5..." << endl;
    v.resize(5, 99);
    cout << "After resize: ";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
    
    cout << "Resizing to 2..." << endl;
    v.resize(2);
    cout << "After resize: ";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
}

void test_emplace() {
    cout << "\n=== Test 7: Emplace (Perfect Forwarding) ===" << endl;
    
    tinystl::vector<TestClass> v;
    
    cout << "Using emplace_back(1)..." << endl;
    v.emplace_back(1);
    
    cout << "\nUsing emplace_back(2)..." << endl;
    v.emplace_back(2);
    
    cout << "\nVector size: " << v.size() << endl;
    cout << "Values: " << v[0].value << ", " << v[1].value << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "TinySTL Vector Test Suite" << endl;
    cout << "========================================" << endl;
    
    try {
        test_basic_operations();
        test_push_pop();
        test_allocator_separation();
        test_copy_and_move();
        test_insert_erase();
        test_resize();
        test_emplace();
        
        cout << "\n========================================" << endl;
        cout << "All tests completed successfully!" << endl;
        cout << "========================================" << endl;
    }
    catch (const exception& e) {
        cout << "\nException caught: " << e.what() << endl;
        return 1;
    }
    catch (...) {
        cout << "\nUnknown exception caught!" << endl;
        return 1;
    }
    
    return 0;
}
