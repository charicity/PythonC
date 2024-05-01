cmake_minimum_required(VERSION 3.5)

message("adding ${CMAKE_CURRENT_BINARY_DIR}")

# 查找 Protobuf 包
find_package(Protobuf REQUIRED)
find_package(Python REQUIRED COMPONENTS Development)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ./src/data.proto)

set(PROTO_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/data.pb.cc PARENT_SCOPE) # 这里我真找不到更好的设置方法了QAQ
set(PROTO_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/data.pb.h PARENT_SCOPE)
