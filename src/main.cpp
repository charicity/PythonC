#include <Python.h>
#include <bits/stdc++.h>

#include "dbg_utili.hpp"
int main() {
    // Python 脚本内容
    std::string script;
    load_file(script, std::string("./main.py"));

    std::cout << "the script" << std::endl;
    std::cout << script << std::endl;

    Py_Initialize();
    // PyRun_SimpleString(
    //     "import os, sys, numpy, data_pb2 as "
    //     "pb");
    PyRun_SimpleString("import os");
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import numpy");
    PyRun_SimpleString("sys.path.append('/home/gene/PythonC/build')");
    PyRun_SimpleString("import data_pb2 as pb");
    // PyImport_ImportModule("os");
    // PyImport_ImportModule("sys");
    // PyImport_ImportModule("data_pb2");
    // 执行Python代码
    PyRun_SimpleString(script.c_str());

    // 释放Python解释器
    Py_Finalize();

    return 0;
}