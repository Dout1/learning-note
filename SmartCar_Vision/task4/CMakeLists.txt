cmake_minimum_required(VERSION 3.10)
project(task4_cross)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
if(NOT OpenCV_FOUND)
    message(WARNING "OpenCV not found, please install it locally.")
endif()

include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(cross_app
    main.cpp
    src/Utils.cpp
)

target_link_libraries(cross_app ${OpenCV_LIBS})
