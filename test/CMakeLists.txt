cmake_minimum_required(VERSION 3.5)

message("adding ${CMAKE_CURRENT_BINARY_DIR}")

find_package(PythonLibs REQUIRED)

link_directories(${Python_LIBRARY_DIRS})
add_executable(getPyobj_full getPyobj_full.cpp ${CMAKE_SOURCE_DIR}/src/data.pb.cc ${CMAKE_SOURCE_DIR}/src/data.pb.h getPyobj_full.py)
target_link_libraries(getPyobj_full ${PROTOBUF_LIBRARIES})
target_link_libraries(getPyobj_full ${PYTHON_LIBRARIES})

add_custom_target(py
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/*.py ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Copying *.py to build directory"
)

add_dependencies(getPyobj_full py)