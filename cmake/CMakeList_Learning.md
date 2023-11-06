# CmakeList编写介绍  
## 目录

- [1. 设定需要的最低版本的CMake](#1-设定需要的最低版本的cmake)
- [2. 指定cmake工程的名字](#2-指定cmake工程的名字)
- [3. add_library()](#3-add_library)

## 1. 设定需要的最低版本的CMake
```
cmake_minimum_required(VERSION <min>[...<policy_max>] [FATAL_ERROE])
```
设置project所需的最低cmake版本，还会更新policy设置；<min>和可选的<policy_max>都是major.minor[.patch[.tweak]]形式的CMake版本，而"..."是literal，低版本和高版本之间的链接符号；  
如果CMake运行的版本低于<min>要求的版本，它将停止处理project并报告错误。  

```
cmake_minimum_required(VERSION 3.16)
```

## 2. 指定cmake工程的名字
```
project(<PROJECT-NAME> [<language-name>...])
```
该命令的主要功能时指定cmake工程的名字，此外还可以指定版本号、描述、主页链接、以及编译工程中锁需要的语言；  
```
project(<PROJECT-NAME>
        [VERSION <major>[.<minor>[.<patch>[.<tweak>]]]]
        [DESCRIPTION <project-description-string>]
        [HOMEPAGE_URL <url-string>]
        [LANGUAGES <language-name>...]
)
```
PROJECT-NAME : 必选，用来设置工程名，设置后，会把设置的值存储在CMAKE_PROJECT_NAME变量中；  
VERSION      : 可选，工程版本号，有主版本号，次版本号，补丁版本号  
DESCRIPTION  : 可选，工程简单的描述  
HOMEPAGE_URL : 可选，工程主页url  
LANGUAGES    : 可选，工程使用的语言，默认为C或者CXX


## 3. add_library()
add_library命令用来使用指定的源文件向工程中添加一个目录库，主要有以下形式：  

### 3.1 普通库
```
add_library(<name> [STATIC | SHARED | MODULE]
            [EXCLUDE_FROM_ALL]
            [<source>...])
```
添加一个从source列表列出的文件构建而来的目标明为 name 的库，name必须全局唯一 ；  
构建库的源文件可以直接指定，也可以后续使用target_sources()指定；  
STATIC(静态库) SHARED(动态库) MODULE(模块库) 用来指定库的类型， 使用STATIC(静态库)构建生成静态库(name.a),使用SHARED(动态库)构建生成动态库(name.so)；  

代码示例：
```
add_library(hello_library STATIC src/Hello.cpp)
```

### 3.2 对象库
```
add_library(<name> OBJECT [<source>...])
```
这种形式类型固定为OBJECT,以这种方式，只编译source列表的文件，但不将生成的目标文件打包或者链接为库，而是在其他add_library()或者add_executable()生成目标的时候，可以使用形如$<TRAGET_OBJECTS:objlib>的表达式将对象库作为源引入。  

示例代码：
```
add_library(test_library OBJECT a.cpp b.cpp c.cpp)

add_executable(test_app main.cpp $<TRAGET_OBJECTS:test_library>)
add_library(anotherlib STATIC other.cpp $<TRAGET_OBJECTS:test_library>)
```

