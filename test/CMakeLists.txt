# 测试程序

# test_vector - vector 容器测试
add_executable(test_vector test_vector.cpp)
target_link_libraries(test_vector PRIVATE TinySTL)
add_test(NAME VectorTest COMMAND test_vector)

# test_allocator - allocator 测试
add_executable(test_allocator testSimpleVectorAndSimpleAllcator.cpp)
target_link_libraries(test_allocator PRIVATE TinySTL)
add_test(NAME AllocatorTest COMMAND test_allocator)

# 设置测试属性
set_tests_properties(VectorTest PROPERTIES
    TIMEOUT 30
    LABELS "container;vector"
)

set_tests_properties(AllocatorTest PROPERTIES
    TIMEOUT 30
    LABELS "allocator;memory"
)

# 打印测试信息
message(STATUS "Test programs configured:")
message(STATUS "  - test_vector")
message(STATUS "  - test_allocator")
