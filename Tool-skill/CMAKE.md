# Cmake 入门教程
## 1.linux编程流程
g++/gcc --> makefile --> cmake

## 2.总体流程
1. 手写 CMakeLists.txt
   ```
    #声明要求的cmake最低版本
    cmake_minimum_required(VERSION 3.5)

    #声明一个cmake工程
    project(HelloWorld)
    
    #添加一个可执行文件
    #语法： add_executable(程序名 源代码文件)
    add_executable(helloworld helloworld.cpp)
   ```
   
2. 创建中间文件夹Build
    ```
    mkdir build
    cd build
    cmake ..
    make
   ```

## 3.前期准备
1. 确认cmake是否安装  
```
cmake --version
```
