cmake_minimum_required(VERSION 3.19.7)

set(This ExampleTests)

set(Sources
    ExampleTests.cpp
)

# 生成可执行文件
add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC
    gtest_main
    Example
)

add_test(
    NAME ${This}
    COMMAND ${This}
)