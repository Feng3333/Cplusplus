# 设置cmake最低可接受版本
cmake_minimum_required(VERSION 3.19.7)
# 项目名称
project(table_driver)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

# 生成app可执行文件
add_executable(app_table_driver
        table_driver.cpp
        table_driver.h
        test_main.cpp)
