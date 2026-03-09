#!/bin/bash
# TinySTL 构建脚本 (Linux/macOS)

set -e  # 遇到错误立即退出

echo "=========================================="
echo "TinySTL Build Script"
echo "=========================================="

# 创建构建目录
BUILD_DIR="build"
if [ -d "$BUILD_DIR" ]; then
    echo "Cleaning existing build directory..."
    rm -rf "$BUILD_DIR"
fi

echo "Creating build directory..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 运行 CMake
echo ""
echo "Running CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译
echo ""
echo "Building project..."
cmake --build . --config Release

# 运行测试
echo ""
echo "Running tests..."
ctest --output-on-failure

echo ""
echo "=========================================="
echo "Build completed successfully!"
echo "=========================================="
echo ""
echo "Executables are in: $BUILD_DIR/bin/"
echo ""
echo "To run examples:"
echo "  ./bin/example_vector"
echo "  ./bin/example_allocator"
echo ""
echo "To run tests:"
echo "  ./bin/test_vector"
echo "  ./bin/test_allocator"
echo ""
