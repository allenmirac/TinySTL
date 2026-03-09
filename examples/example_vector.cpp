#include <iostream>
#include "vector.h"

using namespace std;

int main() {
    cout << "========================================" << endl;
    cout << "TinySTL Vector Example" << endl;
    cout << "========================================" << endl;
    
    // 1. 创建 vector
    cout << "\n1. Creating vector..." << endl;
    tinystl::vector<int> v;
    cout << "Empty vector created. Size: " << v.size() 
         << ", Capacity: " << v.capacity() << endl;
    
    // 2. 添加元素
    cout << "\n2. Adding elements with push_back..." << endl;
    for (int i = 1; i <= 5; ++i) {
        v.push_back(i * 10);
        cout << "Added " << i * 10 << " - Size: " << v.size() 
             << ", Capacity: " << v.capacity() << endl;
    }
    
    // 3. 访问元素
    cout << "\n3. Accessing elements..." << endl;
    cout << "Elements: ";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
    cout << "Front: " << v.front() << ", Back: " << v.back() << endl;
    
    // 4. 使用迭代器遍历
    cout << "\n4. Iterating with iterators..." << endl;
    cout << "Forward: ";
    for (auto it = v.begin(); it != v.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    
    // 5. 使用范围 for（C++11）
    cout << "\n5. Range-based for loop (C++11)..." << endl;
    cout << "Elements: ";
    for (const auto& val : v) {
        cout << val << " ";
    }
    cout << endl;
    
    // 6. 插入元素
    cout << "\n6. Inserting element at position 2..." << endl;
    v.insert(v.begin() + 2, 999);
    cout << "After insert: ";
    for (const auto& val : v) {
        cout << val << " ";
    }
    cout << endl;
    
    // 7. 删除元素
    cout << "\n7. Erasing element at position 2..." << endl;
    v.erase(v.begin() + 2);
    cout << "After erase: ";
    for (const auto& val : v) {
        cout << val << " ";
    }
    cout << endl;
    
    // 8. 使用 emplace_back（完美转发）
    cout << "\n8. Using emplace_back (perfect forwarding)..." << endl;
    v.emplace_back(60);
    cout << "After emplace_back(60): ";
    for (const auto& val : v) {
        cout << val << " ";
    }
    cout << endl;
    
    // 9. resize
    cout << "\n9. Resizing vector..." << endl;
    cout << "Current size: " << v.size() << endl;
    v.resize(10, 100);
    cout << "After resize(10, 100): ";
    for (const auto& val : v) {
        cout << val << " ";
    }
    cout << endl;
    cout << "New size: " << v.size() << endl;
    
    // 10. reserve
    cout << "\n10. Reserving capacity..." << endl;
    cout << "Current capacity: " << v.capacity() << endl;
    v.reserve(20);
    cout << "After reserve(20), capacity: " << v.capacity() << endl;
    
    // 11. 拷贝构造
    cout << "\n11. Copy construction..." << endl;
    tinystl::vector<int> v2(v);
    cout << "v2 (copy of v): ";
    for (const auto& val : v2) {
        cout << val << " ";
    }
    cout << endl;
    
    // 12. 移动构造
    cout << "\n12. Move construction..." << endl;
    tinystl::vector<int> v3(tinystl::move(v2));
    cout << "v3 (moved from v2): ";
    for (const auto& val : v3) {
        cout << val << " ";
    }
    cout << endl;
    cout << "v2 size after move: " << v2.size() << endl;
    
    // 13. 初始化列表
    cout << "\n13. Initializer list construction..." << endl;
    tinystl::vector<int> v4 = {1, 2, 3, 4, 5};
    cout << "v4 from {1,2,3,4,5}: ";
    for (const auto& val : v4) {
        cout << val << " ";
    }
    cout << endl;
    
    cout << "\n========================================" << endl;
    cout << "All examples completed successfully!" << endl;
    cout << "========================================" << endl;
    
    return 0;
}
