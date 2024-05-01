cmake_minimum_required(VERSION 3.5)

message("adding ${CMAKE_CURRENT_BINARY_DIR}")

find_package(PythonLibs REQUIRED)

link_directories(${Python_LIBRARY_DIRS})
link_libraries(${PYTHON_LIBRARIES})
link_libraries(${PROTOBUF_LIBRARIES})

#####################

add_custom_target( py
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/*.py ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying *.py"
)

add_executable(getPyobj_full getPyobj_full.cpp ${PROTO_SRCS} ${PROTO_HDRS})
add_dependencies(getPyobj_full py)

add_executable(setPyobj_full setPyobj_full.cpp ${PROTO_SRCS} ${PROTO_HDRS})
add_dependencies(setPyobj_full py)

# add_executable(script_namespace script_namespace.cpp ${PROTO_SRCS} ${PROTO_HDRS})
# add_dependencies(script_namespace py)

# add_executable(just_run just_run.cpp ${PROTO_SRCS} ${PROTO_HDRS})
# add_dependencies(just_run py)

