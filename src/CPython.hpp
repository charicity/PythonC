#pragma once
// 全局变量
#ifndef PY_SSIZE_T_CLEAN
#define PY_SSIZE_T_CLEAN
#endif
#include <Python.h>

#include "data.pb.h"
#include "dbg_utili.hpp"

static MatchRuleReq* ptrMatch = NULL;
static const std::string* ptrInfo = NULL;

// 获取全局变量的值
static PyObject* get_shared_variable(PyObject* self, PyObject* args) {
    char* str;
    if (!PyArg_ParseTuple(args, "s", &str)) {
        return NULL;
    }

    printf("request_name: %s [GET] '%s'\n", ptrInfo->c_str(), str);
    std::string key = str;

    const auto& context_map = ptrMatch->context_map();
    auto it = context_map.find(key);
    if (it == context_map.end()) {
        return NULL;
    }

    context_value ret;
    std::string msg;
    ret = it->second;
    ret.SerializeToString(&msg);

    PyObject* tmp = Py_BuildValue("y#", msg.c_str(), msg.length());

    return tmp;  // dangerous?
}

// 设置全局变量的值
static PyObject* set_shared_variable(PyObject* self, PyObject* args) {
    PyObject* new_value;
    char* str;
    char* data;

    // 使用 PyArg_ParseTuple 解析参数
    if (!PyArg_ParseTuple(args, "sy*", &str, &data)) {
        // 解析失败，返回 NULL 表示出错
        return NULL;
    }

    context_value fetch;
    fetch.ParseFromArray(data, strlen(data));

    printf("len=%lu, val=%s\n", strlen(data), fetch.string().c_str());

    printf("request_name: %s [SET] '%s'\n", ptrInfo->c_str(), str);

    std::string key = str;

    (*ptrMatch->mutable_context_map())[key] = fetch;

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
struct PyModuleDef myModule = {PyModuleDef_HEAD_INIT, "mymodule", NULL, -1,
                               myModuleMethods};

// 模块初始化函数
PyMODINIT_FUNC PyInit_mymodule(void) { return PyModule_Create(&myModule); }

static void py_init() {
    static bool initiated = false;

    if (initiated) return;
    initiated = true;

    PyImport_AppendInittab("mymodule", &PyInit_mymodule);
    Py_Initialize();
}

static void py_end() {
    Py_Finalize();
    ;
}

static void call_init(MatchRuleReq& maps, const std::string& info) {
    ptrMatch = &maps;
    ptrInfo = &info;
}

static void call_end() {
    static std::string clean = R"(
print('cleaning')
for key in globals().copy(): 
    if not key.startswith("__"):
        globals().pop(key))";
    PyRun_SimpleString(clean.c_str());
}

void call_python(const std::string& script, MatchRuleReq& maps,
                 const std::string& info) {
    py_init();
    call_init(maps, info);

    PyRun_SimpleString(script.c_str());

    call_end();
}