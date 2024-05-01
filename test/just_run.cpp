#include <Python.h>

#include <cstdio>
#include <cstring>
#include <iostream>

#include "CPython.hpp"
#include "dbg_utili.hpp"

int main(int argc, char *argv[]) {
    // 初始化 Python 解释器
    PyImport_AppendInittab("mymodule", &PyInit_mymodule);
    Py_Initialize();

    std::string script = load_file("./just_run.py");
    PyObject *code =
        Py_CompileStringExFlags(script.c_str(), "haha", Py_file_input, NULL, 2);
    if (code == NULL) {
        std::cout << "Error Compiling String" << std::endl;
        return -1;
    }

    // Execute the bytecode object
    PyObject *glb = PyDict_New();
    PyObject *loc = PyDict_New();
    PyObject *result = PyEval_EvalCode(code, glb, loc);
    if (result == nullptr) {
        PyErr_Print();
        return -1;
    }

    // 清理
    Py_Finalize();

    return 0;
}