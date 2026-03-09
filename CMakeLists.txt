cmake_minimum_required(VERSION 3.10)
project(TinySTL VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 选项：是否构建测试
option(BUILD_TESTS "Build test programs" ON)
option(BUILD_EXAMPLES "Build example programs" ON)

# 头文件库（Header-only library）
add_library(TinySTL INTERFACE)
target_include_directories(TinySTL INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(TinySTL INTERFACE cxx_std_11)

# 构建测试
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# 构建示例
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# 安装规则
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

install(TARGETS TinySTL
    EXPORT TinySTLTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# 导出配置
install(EXPORT TinySTLTargets
    FILE TinySTLTargets.cmake
    NAMESPACE TinySTL::
    DESTINATION lib/cmake/TinySTL
)

# 打印配置信息
message(STATUS "")
message(STATUS "========== TinySTL Configuration ==========")
message(STATUS "CMake version:        ${CMAKE_VERSION}")
message(STATUS "C++ standard:         ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type:           ${CMAKE_BUILD_TYPE}")
message(STATUS "Build tests:          ${BUILD_TESTS}")
message(STATUS "Build examples:       ${BUILD_EXAMPLES}")
message(STATUS "Install prefix:       ${CMAKE_INSTALL_PREFIX}")
message(STATUS "==========================================")
message(STATUS "")
