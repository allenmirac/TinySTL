@echo off
REM TinySTL 构建脚本 (Windows)

echo ==========================================
echo TinySTL Build Script (Windows)
echo ==========================================

REM 创建构建目录
set BUILD_DIR=build
if exist %BUILD_DIR% (
    echo Cleaning existing build directory...
    rmdir /s /q %BUILD_DIR%
)

echo Creating build directory...
mkdir %BUILD_DIR%
cd %BUILD_DIR%

REM 运行 CMake
echo.
echo Running CMake...
cmake .. -DCMAKE_BUILD_TYPE=Release

if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    cd ..
    exit /b 1
)

REM 编译
echo.
echo Building project...
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    cd ..
    exit /b 1
)

REM 运行测试
echo.
echo Running tests...
ctest -C Release --output-on-failure

echo.
echo ==========================================
echo Build completed successfully!
echo ==========================================
echo.
echo Executables are in: %BUILD_DIR%\bin\Release\
echo.
echo To run examples:
echo   .\bin\Release\example_vector.exe
echo   .\bin\Release\example_allocator.exe
echo.
echo To run tests:
echo   .\bin\Release\test_vector.exe
echo   .\bin\Release\test_allocator.exe
echo.

cd ..
