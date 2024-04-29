#include <Python.h>
#include <bits/stdc++.h>

#include "dbg_utili.hpp"

// python3 setup.py build_ext --inplace

// 全局整型变量
int shared_variable = 10;

// 函数：获取全局整型变量的值
static PyObject* get_shared_variable(PyObject* self, PyObject* args) {
    return Py_BuildValue("i", shared_variable);
}

// 函数：设置全局整型变量的值
static PyObject* set_shared_variable(PyObject* self, PyObject* args) {
    int new_value;
    if (!PyArg_ParseTuple(args, "i", &new_value)) {
        return NULL;
    }
    shared_variable = new_value;
    Py_RETURN_NONE;
}

// 定义模块方法
static PyMethodDef myModuleMethods[] = {
    {"get_shared_variable", get_shared_variable, METH_NOARGS,
     "Get shared variable value"},
    {"set_shared_variable", set_shared_variable, METH_VARARGS,
     "Set shared variable value"},
    {NULL, NULL, 0, NULL}  // 结束标志
};

// 模块定义
static struct PyModuleDef myModule = {PyModuleDef_HEAD_INIT, "mymodule", NULL,
                                      -1, myModuleMethods};

// 模块初始化函数
PyMODINIT_FUNC PyInit_mymodule(void) { return PyModule_Create(&myModule); }

int main() {
    // Python 脚本内容
    std::string script;
    load_file(script, std::string("./main.py"));

    std::cout << "the script" << std::endl;
    std::cout << script << std::endl;

    std::cout << shared_variable << std::endl;

    PyImport_AppendInittab("mymodule", &PyInit_mymodule);  // 增加一个模块
    Py_Initialize();

    // 执行Python代码
    PyRun_SimpleString(script.c_str());

    // 释放Python解释器
    Py_Finalize();

    std::cout << shared_variable << std::endl;

    return 0;
}