cmake_minimum_required(VERSION 3.5)

message("adding ${CMAKE_CURRENT_BINARY_DIR}")

# 查找 Protobuf 包
find_package(Protobuf REQUIRED)
find_package(Python REQUIRED COMPONENTS Development)


# 添加链接库
link_directories(${Python_LIBRARY_DIRS})

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ./src/data.proto)