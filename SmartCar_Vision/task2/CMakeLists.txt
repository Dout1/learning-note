cmake_minimum_required(VERSION 3.10)
project(TrackLineSearch)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

# 添加可执行文件
add_executable(track_search
    src/main.cpp
    src/tracking.cpp
)

# 链接 OpenCV 库
target_link_libraries(track_search ${OpenCV_LIBS})