cmake_minimum_required(VERSION 3.5)
project(PythonC)

# 查找 Protobuf 包
find_package(Protobuf REQUIRED)

# 添加生成 Protobuf 文件的路径
include_directories(${PROTOBUF_INCLUDE_DIRS})

# 生成 Protobuf 源文件
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS data.proto)

# 添加可执行文件
add_executable(main main.cpp ${PROTO_SRCS} ${PROTO_HDRS})

# 链接 Protobuf 库
target_link_libraries(main ${PROTOBUF_LIBRARIES})