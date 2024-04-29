#include <Python.h>
#include <bits/stdc++.h>

#include "dbg_utili.hpp"

// python3 setup.py build_ext --inplace

int call(int x, int y) { return x + y; }

int c = 0;

static PyObject *adds(PyObject *self, PyObject *args) {
    int a, b;

    /* Parse arguments */
    if (!PyArg_ParseTuple(args, "ii", &a, &b)) {
        return NULL;
    }

    int sum = a + b;
    c = sum;

    return PyLong_FromLong(sum);
}

static PyMethodDef addsMethods[] = {
    {"adds", adds, METH_VARARGS, "Python interface for C APB Problem"},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef addsModule = {PyModuleDef_HEAD_INIT, "adds",
                                        "Python interface for C APB Problem",
                                        -1, addsMethods};

PyMODINIT_FUNC PyInit_adds(void) { return PyModule_Create(&addsModule); }

int main() {
    // Python 脚本内容
    std::string script;
    load_file(script, std::string("./main.py"));

    std::cout << "the script" << std::endl;
    std::cout << script << std::endl;

    Py_Initialize();
    std::cout << c << std::endl;

    // 执行Python代码
    PyRun_SimpleString(script.c_str());

    // 释放Python解释器
    Py_Finalize();

    std::cout << c << std::endl;

    return 0;
}