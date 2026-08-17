cmake_minimum_required(VERSION 3.10)
project(center_line_fit)

# ===== C++ 标准 =====
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===== 查找 OpenCV =====
find_package(OpenCV REQUIRED)

# ===== 头文件目录 =====
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

# ===== 源文件 =====
set(SOURCES
    src/main.cpp
    src/track.cpp
    src/center.cpp
    src/bezier.cpp
    src/utils.cpp
)

# ===== 生成可执行文件 =====
add_executable(center_line_fit ${SOURCES})

# ===== 链接库 =====
target_link_libraries(center_line_fit ${OpenCV_LIBS})