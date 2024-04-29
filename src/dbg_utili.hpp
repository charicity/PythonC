#pragma once
#include <Python.h>

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

std::string convertToStdString(const char* c_str) {
    if (c_str == nullptr) {
        return "";  // 如果输入是空指针，则返回空字符串
    }
    return std::string(c_str);
}

void py_ref_count(const PyObject* object, const char* s = "") {
    printf("Reference count of %s: %ld\n", s, Py_REFCNT(object));
}

void load_file(std::string& str, const std::string& file) {
    std::ifstream infile(file.c_str());
    std::stringstream buffer;

    if (infile.is_open()) {
        buffer << infile.rdbuf();
        infile.close();
    } else {
        std::cerr << "Failed!" << std::endl;
    }

    str = buffer.str();
}

void writeStringToFile(std::string& s) {
    std::ofstream out("output.txt");
    out << s;
    out.close();
}