cmake_minimum_required(VERSION 3.10)
project(otsu_demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 OpenCV
find_package(OpenCV REQUIRED)

# 头文件目录
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

# 源文件
set(SOURCES
    main.cpp
    src/otsu.cpp
)

# 生成可执行文件
add_executable(otsu_demo ${SOURCES})

# 链接 OpenCV 库
target_link_libraries(otsu_demo ${OpenCV_LIBS})