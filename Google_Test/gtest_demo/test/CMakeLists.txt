cmake_minimum_required(VERSION 3.19.7)

set(This ExampleTests)

set(Sources
    ExampleTests.cpp
)

include_directories(
# 这里的需要根据test测试文件中实际需要使用到的内容添加
        ${CMAKE_CURRENT_SOURCE_DIR}/
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
