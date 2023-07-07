# 设置可接受的cmake最低版本
cmake_minimum_required(VERSION 3.19.7)

set(This Example)

project(${This} CXX)

# 设置c++版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()
add_subdirectory(googletest)

set(Headers
    Example.hpp
)

set(Sources
    Example.cpp
)

add_library(${This} STATIC ${Sources} ${Headers})

add_subdirectory(test)




