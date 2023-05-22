cmake_minimum_required(VERSION 3.19.7)
# 设置工程名称
PROJECT(TESTLIB)
# 创建变量SRC 源码文件
SET(SRC test.cpp)
# 添加构建目标， 动态链接库 名字叫做test
# 生成 动态链接库 libtest.dll
ADD_LIBRARY(test SHARED  ${SRC})
