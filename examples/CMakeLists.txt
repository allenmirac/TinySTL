# 示例程序

# example_vector - vector 使用示例
add_executable(example_vector example_vector.cpp)
target_link_libraries(example_vector PRIVATE TinySTL)

# example_allocator - allocator 使用示例
add_executable(example_allocator example_allocator.cpp)
target_link_libraries(example_allocator PRIVATE TinySTL)

# 打印示例信息
message(STATUS "Example programs configured:")
message(STATUS "  - example_vector")
message(STATUS "  - example_allocator")
