cmake_minimum_required(VERSION 3.5)
project(PythonC)

# 查找 Protobuf 包
find_package(Protobuf REQUIRED)
find_package(Python REQUIRED COMPONENTS Development)


# 添加包含路径
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${Python_INCLUDE_DIRS})

message((${Python_INCLUDE_DIRS}))

# 添加链接库
link_directories(${Python_LIBRARY_DIRS})

# 生成 Protobuf 源文件
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS data.proto)


## main
# 添加可执行文件
add_executable(main main.cpp ${PROTO_SRCS} ${PROTO_HDRS})

# 链接 Protobuf 库
target_link_libraries(main ${PROTOBUF_LIBRARIES})
target_link_libraries(main ${Python_LIBRARIES})

## pb_spawn
# 添加可执行文件
add_executable(pb_spawn pb_spawn.cpp ${PROTO_SRCS} ${PROTO_HDRS})

# 链接 Protobuf 库
target_link_libraries(pb_spawn ${PROTOBUF_LIBRARIES})
target_link_libraries(pb_spawn ${Python_LIBRARIES})

add_custom_target(python 
    COMMAND cp -f ../src/*.py ./
    COMMAND cp -f ../src/*.so ./
)

add_custom_target(cleanpy 
    COMMAND rm -f ./*.py
    COMMAND rm -f ./*.so
)