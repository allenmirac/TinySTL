#include <iostream>
#include "allocator.h"

using namespace std;

// 测试类
class TestObject {
public:
    int value;
    
    TestObject() : value(0) {
        cout << "  [Constructor] Default: value = " << value << endl;
    }
    
    explicit TestObject(int v) : value(v) {
        cout << "  [Constructor] Parameterized: value = " << value << endl;
    }
    
    ~TestObject() {
        cout << "  [Destructor] value = " << value << endl;
    }
    
    TestObject(const TestObject& other) : value(other.value) {
        cout << "  [Copy Constructor] value = " << value << endl;
    }
};

int main() {
    cout << "========================================" << endl;
    cout << "TinySTL Allocator Example" << endl;
    cout << "========================================" << endl;
    
    // 1. 分配内存（不构造对象）
    cout << "\n1. Allocating memory for 5 objects..." << endl;
    TestObject* ptr = tinystl::allocator<TestObject>::allocate(5);
    cout << "Memory allocated at: " << ptr << endl;
    cout << "Note: No constructors called yet!" << endl;
    
    // 2. 在已分配的内存上构造对象
    cout << "\n2. Constructing objects in allocated memory..." << endl;
    cout << "Constructing object at index 0:" << endl;
    tinystl::allocator<TestObject>::construct(ptr + 0, 10);
    
    cout << "Constructing object at index 1:" << endl;
    tinystl::allocator<TestObject>::construct(ptr + 1, 20);
    
    cout << "Constructing object at index 2:" << endl;
    tinystl::allocator<TestObject>::construct(ptr + 2, 30);
    
    cout << "\nOnly 3 objects constructed out of 5 allocated slots!" << endl;
    
    // 3. 访问已构造的对象
    cout << "\n3. Accessing constructed objects..." << endl;
    cout << "Object[0].value = " << ptr[0].value << endl;
    cout << "Object[1].value = " << ptr[1].value << endl;
    cout << "Object[2].value = " << ptr[2].value << endl;
    
    // 4. 析构对象（不释放内存）
    cout << "\n4. Destroying constructed objects..." << endl;
    cout << "Destroying object at index 0:" << endl;
    tinystl::allocator<TestObject>::destroy(ptr + 0);
    
    cout << "Destroying object at index 1:" << endl;
    tinystl::allocator<TestObject>::destroy(ptr + 1);
    
    cout << "Destroying object at index 2:" << endl;
    tinystl::allocator<TestObject>::destroy(ptr + 2);
    
    cout << "\nObjects destroyed, but memory still allocated!" << endl;
    
    // 5. 释放内存
    cout << "\n5. Deallocating memory..." << endl;
    tinystl::allocator<TestObject>::deallocate(ptr, 5);
    cout << "Memory deallocated." << endl;
    
    // 6. 演示批量析构
    cout << "\n6. Demonstrating batch destruction..." << endl;
    cout << "Allocating and constructing 3 objects:" << endl;
    TestObject* ptr2 = tinystl::allocator<TestObject>::allocate(3);
    for (int i = 0; i < 3; ++i) {
        tinystl::allocator<TestObject>::construct(ptr2 + i, (i + 1) * 100);
    }
    
    cout << "\nBatch destroying all 3 objects:" << endl;
    tinystl::allocator<TestObject>::destroy(ptr2, ptr2 + 3);
    
    cout << "\nDeallocating memory:" << endl;
    tinystl::allocator<TestObject>::deallocate(ptr2, 3);
    
    // 7. 对比传统 new/delete
    cout << "\n7. Comparing with traditional new/delete..." << endl;
    cout << "\nUsing new (allocates + constructs immediately):" << endl;
    TestObject* ptr3 = new TestObject[3];  // 立即构造 3 个对象
    cout << "All 3 objects constructed immediately!" << endl;
    
    cout << "\nUsing delete:" << endl;
    delete[] ptr3;
    
    cout << "\n========================================" << endl;
    cout << "Key Takeaways:" << endl;
    cout << "1. Allocator separates memory allocation from object construction" << endl;
    cout << "2. This allows containers to allocate memory without constructing objects" << endl;
    cout << "3. Objects are constructed only when needed (e.g., push_back)" << endl;
    cout << "4. This is more efficient than new/delete for containers" << endl;
    cout << "========================================" << endl;
    
    return 0;
}
