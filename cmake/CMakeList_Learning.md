# CmakeList编写介绍  
## 目录

- [1. 设定需要的最低版本的CMake](#1-设定需要的最低版本的cmake)
- [2. 指定cmake工程的名字](#2-指定cmake工程的名字)
- [3. add_library()](#3-add_library)
- [4. add_executable()](#4-add_executable)
- [5. target_include_directories()](#5-target_include_directories)
- [6. include_directories()](#6-include_directories)
- [7. target_link_libraries()](#7-target_link_libraries)
- [8. set()](#8-set)

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

### 3.3 接口库
```
add_library(<name> INTERFACE [IMPORTED [GLOBAL]])
```
生成一个接口库，这类库不编译任何文件，也不在磁盘上产生库文件。它有一些属性设置被设置，并且能够被安装和导出。  
通常使用以下命令在接口目标上填充属性: 
```
set_property()
target_link_library(INTERFACE)
target_link_options(INTERFACE)
target_include_directions(INTERFACE)
target_compile_options(INTERFACE)
target_compile_definitions(INTERFACE)
target_sources(INTERFACE)
```
然后像其他目标一样被用作参数给target_link_libraries()

### 3.4 导入的库
```
add_library(<name> <STATIC|SHARED|MODULE|UNKNOWN> IMPORTED [GLOBAL])
```
用来导入已经存在的库，CMake也不会添加任何编译规则给它；  
此类库的标志就是有IMPORTED属性，导入的库的作用域为创建它的目录及更下级目录。但是如果有GLOBAL属性，则作用域被拓展到全工程；  
导入的库类型必须是 STATIC| SHARED | MODULE| UNKNOWN 中的一种，对于UNKNOWN类型，不需要知道类型就可以使用的从工程外部引入的一个库，使用IMPORTED_LOCATION属性确定库文件在磁盘上的完整路径。  

### 3.5 别名库
```
add_library(<name> ALIAS <target>)
```
为给定library添加一个别名，后续可使用<name>来代替<target>


## 4. add_executable()
使用指定的源文件创建出一个可执行文件

### 4.1 普通可执行文件
```
add_executable(<name> [WIN32] [MACOSX_BUNDLE]
                [EXCLUDE_FROM_ALL]
                [source1] [source2 ...])
```
添加一个可执行文件目标，此目标:  
1. 由source列出的文件构建而来
2. 名字为name

命令参数：  
- name: 生成可执行文件的名字，必须在工程内全局唯一
- WIN32: 有此参数时，WIN32_EXECUTABLE属性会被置位true，此时在windows环境下的创建的可执行文件将以WinMain函数代替main函数作为程序入口，构建而成的可执行文件为GUI应用程序而不是控制台应用程序；
- MACOSX_BUNDLE: 有此参数时，MACOSX_BUNDLE属性会被置位true，此时在macOS或者iOS上构建可执行文件目标时，目标会称为一个从Finder启动的GUI可执行程序；
- EXCLUDE_FROM_ALL: 有此参数时，此目标就会被排除在all target列表之外，即在执行默认的make时，不会构造此目标，需要构造此目标的时候，需要手动构建，如：
 ```
add_executable(test EXCLUDE_FROM_ALL text.cpp)

make test
```
- 可以使用target_sources()继续为构建可执行文件目标添加源文件，但是target_sources()指令必须在 add_executable 或 add_library 之后调用.
 
示例程序(普通)：
```
cmake_minimum_required(VERSION 3.5)

project(hello_cmake)

add_execuable(hello_cmake main.cpp)
```

### 4.2 导入的可执行文件
将工程外部的可执行目标文件导入进来，不会有任何构建可执行目标文件的动作发生；  
```
add_executable(<name> IMPORTED [GLOBAL])
```
- name: 导入可执行文件的名字
- IMPORTED: 导入的目标文件需指定IMPORTED属性，IMPORTED属性指定后，目标文件的属性IMPORTED被置位true，在工程内构建生成的可执行文件的IMPORTED属性会被置位false；

### 4.3 别名可执行文件
为目标文件取一个别名，一遍后续继续使用
```
add_executable(<name> ALIAS <target>)
```
为目标创建别名后，可以使用别名读取目标的属性，但不能修改目标属性. 


## 5. target_include_directories()
```
target_include_directories(<target> [SYSTEM] [AFTER |  BEFORE]6. include_directories()
                           <INTERFACE|PUBLIC|PRIVATE> [items1...]
                           [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```
该命令用于为指定的目标 target 添加包含目录 include directories 。指定编译目标时需要搜索头文件的路径，以确保编译器可以找到所需的头文件，
这对于创建CMake项目中的库和可执行文件非常有用，因为它们可能依赖于其他模块或库的头文件。  
参数说明：  
- target: 要添加包含目录的目标名称
- SYSTEM: 可选参数，指定被包含的目录是系统目录，这会告诉编译器将这些目录视为系统的标准头文件目录。
- BEFORE: 可选参数，指定将包含目录添加到已有的包含目录之前，而不是默认的添加到后面。
- INTERFACE|PUBLIC|PRIVATE: 指定包含目录的可见性级别
  INTERFACE: 表示包含目录将应用于目标及其使用该目标的其他目标；
  PUBLIC: 表示包含目录将应用于目标本身和使用该目标的其他目标
  PRIVATE: 表示包含目录仅应用于目标本身；
- items: 指定要添加的包含目录路径，可以是目录名、绝对路径或者相对路径。

代码示例：
```
# 使用绝对路径
target_include_directories(target_name PRIVATE /path/to/directory)
```
这将为指定的目标 target_name 添加一个私有的包含目录，即只有该目标及其依赖项可以访问这个目录。

```
# 使用相对路径
target_include_directories(target_name PRIVATE relative/path/to/directory)
```
这将使用相对于当前CMakeLists.txt文件所在路径的相对路径来指定包含目录

```
# 还可以一次添加多个查找目录
target_include_directories(target_name PRIVATE
                           directory1
                           directory2
                           ...
)
```

## 6. include_directories()
```
include_directories([AFTER|BEFORE] [SYSTEM] dir1 [dir2 ...])
```
include_directories用于指定包含头文件目录。include_directories函数允许在CMakeLists.txt文件中指定项目的头文件目录，以便在编译过程中能够正确的找到包含目录中的头文件。  
参数说明：  
- AFTER|BEFORE: 可选参数，用于指定包含目录的添加顺序，如果使用AFTER修饰符，那么添加的目录将会放到已有包含目录的后面，使用BEFORE修饰符则会把添加的目录将放到已有包含目录的前面，默认为AFTER；
- SYSTEM: 可选的修饰符，用于指定所包含的目录是系统拒接的目录。当使用SYSTEM修饰符时，编译器会将这些目录视为系统级别的头文件目录，这意味着编译器不会产生关于这些目录的警告信息。

代码示例：
```
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/dir1/
        directory1
        directory2
        ...
)
```

注意事项：
- include_directories命令应该在 add_exectable 或 add_library 之前使用，以确保编译器在构建过程中能够找到所需的头文件  
- include_directories命令仅仅告诉编译器在哪些目录中查找头文件，并不会自动包含这些头文件。要包含头文件，需要使用#include预处理指令在代码中显式包含。  
- 尽量避免使用include_directories命令的绝对路径。推荐使用相对于CMakeLists.txt文件的相对路径，以增加项目的可移植性；  
- 在使用include_directories命令时，应确保指定的目录中包含正确的头文件。否则，编译器可能无法找到所需的头文件，导致编译错误。  


## 7. target_link_libraries()
```
target_link_libraries(<target> ... <item>... ...)
```
该命令用于将目标与所需的库进行链接，它用于指定一个目标(例如可执行文件或库)需要依赖的其他库，以便在构建过程中正确地链接这些库。该命令有多种使用方式。

参数说明：  
- target: 要链接库的目标
  target必须是由 add_exectable 或 add_library 等命令创建的，并且不能是ALIAS目标；  
  对同一target的重复调用按照调用的顺序追加项；  
- item: item可以是:  
  一个库目标的名称  
  库文件的完整路径  
  普通的库名称  
  链接标志  
  一个生成器表达式  
  构建配置的关键字

使用示例：  
```
target_link_libraries(<target>
                      <PRIVATE|PUBLIC|INTERFACE> <item>...
                      [<PRIVATE|PUBLIC|INTERFACE> <item>...]...)
```


假设有两个目标: my_app 和 my_library, 目前想将 my_library 链接到 my_app中:  
```
# 将 my_library 链接到 my_app 中
add_library(my_library STATIC my_library.cpp)
add_executable(my_app main.cpp)
target_link_libraries(my_app my_library)

# 先将other_library链接到my_library上
add_library(other_library STATIC other_library.cpp)
target_link_libraries(my_library other_library)
# 最后构建my_app时，my_app会链接到other_library 和 my_library，并将他们一起包含在最终的可执行文件中
target_link_libraries(my_app my_library)
```

## 8. set()
```
set(<variable> <value>... [PARENT_SCOPE])
```
在CMake中，set用于设置变量的值。这个变量可以为普通变量、Cache、或者是环境变量；  
如果提供了一个或多个 \<value> 参数，则将 \<variable> 设置为给定的值.  
如果没有提供 \<value> 参数，则取消设置 \<variable>，将其值设置为无，相当于unset  

参数说明：  
- variable: 要被赋值的变量
- value: 要付给变量的值
- PARENT_SCOPE: 是一个修饰符，用于指定变量的作用域作为父级作用域。

使用示例:  
```
function(set_variable)
	set(variable "Hello, World!" PARENT_SCOPE)
endfunction()

set_variable()
# 输出: Hello, World!
message("${variable}")
```
在上述示例中，set_variable()函数内部使用了set命令设置了变量variable的值，并使用PARENT_SCOPE修饰符将其作用域提升到调用函数的作用域.   
因此，message命令在函数外部打印出了变量variable的值。  
- 需要注意的是：PARENT_SCOPE修饰符只能将变量的作用域提升到直接的父级作用域，无法跳过中间的作用域.
  如果要将变量的作用域提升到更高层次的作用域，需要在对应的父级作用域中再次使用set命令.

#### 设置Cache变量
set命令可以用于设置缓存变量(cache entry), 并提供用户可设置的值.
```
set(<variable> <value>... CACHE <type> <docstring> [FORCE])
```
参数说明:  
- <variable>: 要设置的缓存变量的名称  
- <value>: 要为缓存变量设置的值(可以有多个)
- CACHE: 指定该变量是一个缓存变量
- <type>: 变量的类型，可以是\[BOOL, FILEPATH, PATH, STRING, INTERNAL]
- FORCE: 可选项，用于强制覆盖现有的缓存变量。缓存默认是不覆盖的，如果在调用之前缓存项不存在，或者给出了FORCE选项，那么缓存项将被设置为给定的值.

使用示例：
```
cmake_minimum_required(VERSION 3.5)
project(hello_library)

set(MY_VARIABLE "Hello, World!" CACHE STRING "A greeting message" FORCE)
message("${MY_VARIABLE}")  # 输出 Hello, World!

set(MY_VARIABLE "Good Morning" CACHE STRING "A greeting message")
message("${MY_VARIABLE}")  # 输出 Hello, World! 因为不加FORCE默认不覆盖

set(MY_VARIABLE "Good Morning" CACHE STRING "A greeting message" FORCE)
message("${MY_VARIABLE}")  # 输出 Good Morning 加了FORCE会强制覆盖
```

#### 设置系统变量
设置环境变量的值，执行这个命令时，它只会对当前的CMake进程中的环境变量进行修改;  
它不会影响调用CMake的进程的环境变量，这个系统环境，或者后续构建或测试过程中的环境变量.  
```
set(ENV{variable} [<value>])
```
参数说明:  
- variable: 环境变量的名称   
- value: 要设置的值, 如果没有提供value或者value为空字符串, 该命令将清楚环境变量的任何现有值
- 在set(ENV{} [])命令之后提供的任何参数都会被忽略，如果发现额外的参数，将会发出警告.  

使用示例:  
```
cmake_minimum_required(VERSION 3.5)
project(setCommand)

# 设置一个名为 MY_VARIABLE 的环境变量，其值为 Hello World
set(ENV{MY_VARIABLE} "Hello World")
message(">>> MY_VARIABLE: $ENV{MY_VARIABLE}")

# 清除名为 MY_VARIABLE 环境变量的值
set(ENV{MY_VARIABLE} )
message(">>> MY_VARIABLE: $ENV{MY_VARIABLE}")

# 设置一个名为 NUM_THREADS 的环境变量，其值为4
set(ENV{NUM_THREADS} 4)
message(">>> NUM_THREADS: $ENV{NUM_THREADS}")

# 清除名为 NUM_THREADS 环境变量的值
set(ENV{NUM_THREADS} )
message(">>> NUM_THREADS: $ENV{NUM_THREADS}")
```
