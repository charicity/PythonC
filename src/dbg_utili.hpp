#pragma once
#include <Python.h>

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

void py_ref_count(const PyObject* object, const char* s = "") {
    printf("Reference count of %s: %ld\n", s, Py_REFCNT(object));
}

void load_file(std::string& str, const std::string& file) {
    std::ifstream infile(file.c_str());
    std::stringstream buffer;  // 创建一个字符串流

    if (infile.is_open()) {
        buffer << infile.rdbuf();
        infile.close();
    } else {
        std::cerr << "Failed!" << std::endl;
    }

    str = buffer.str();
}