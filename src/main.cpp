#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <cstdio>
#include <cstring>
#include <iostream>

#include "data.pb.h"
#include "dbg_utili.hpp"

// 全局变量
std::map<std::string, int> shared_variable;

// 获取全局变量的值
static PyObject* get_shared_variable(PyObject* self, PyObject* args) {
    const char* str;
    if (!PyArg_ParseTuple(args, "s", &str)) {
        return NULL;
    }
    printf("request_name: (TBD) [GET] '%s'=(TBD)\n", str);
    // std::string s = convertToStdString(str);

    context_value a;

    a.set_uint32(10);

    std::string msg;
    a.SerializeToString(&msg);

    return Py_BuildValue("y#", msg.c_str(), msg.length());  // dangerous
}

// 设置全局变量的值
static PyObject* set_shared_variable(PyObject* self, PyObject* args) {
    int new_value;
    const char* str;

    if (!PyArg_ParseTuple(args, "si", &str, &new_value)) {
        return NULL;
    }

    std::string s = convertToStdString(str);

    shared_variable[s] = new_value;
    Py_RETURN_NONE;
}

// 定义方法
static PyMethodDef myModuleMethods[] = {
    {"get_shared_variable", get_shared_variable, METH_VARARGS,
     "Get shared variable value"},
    {"set_shared_variable", set_shared_variable, METH_VARARGS,
     "Set shared variable value"},
    {NULL, NULL, 0, NULL}};

// 定义模块
static struct PyModuleDef myModule = {PyModuleDef_HEAD_INIT, "mymodule", NULL,
                                      -1, myModuleMethods};

// 模块初始化函数
PyMODINIT_FUNC PyInit_mymodule(void) { return PyModule_Create(&myModule); }

int main() {
    // Python 脚本内容
    std::string script1;
    load_file(script1, std::string("./main1.py"));

    std::string script2;
    load_file(script2, std::string("./main2.py"));

    std::string script3;
    load_file(script3, std::string("./main3.py"));

    std::string clean;
    load_file(clean, std::string("./clean.py"));

    // 执行Python代码
    // run_statement_in_module()
    PyImport_AppendInittab("mymodule", &PyInit_mymodule);

    Py_Initialize();

    PyRun_SimpleString(script3.c_str());
    PyRun_SimpleString(clean.c_str());
    PyRun_SimpleString(script2.c_str());
    PyRun_SimpleString(clean.c_str());
    PyRun_SimpleString(script2.c_str());
    PyRun_SimpleString(clean.c_str());

    Py_Finalize();

    return 0;
}