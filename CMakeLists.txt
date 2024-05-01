cmake_minimum_required(VERSION 3.5)
project(PythonC)
message("adding ${CMAKE_CURRENT_BINARY_DIR}")

# 查找 Protobuf 包
find_package(Protobuf REQUIRED)
find_package(Python REQUIRED COMPONENTS Development)

# 添加包含路径
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${Python_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# 生成 Protobuf 源文件

add_subdirectory(src)
add_subdirectory(test)
