#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <cstdio>
#include <cstring>
#include <iostream>

#include "data.pb.h"
#include "dbg_utili.hpp"

// 全局变量
MatchRuleReq* ptrMatch = NULL;
const std::string* ptrInfo = NULL;

// 获取全局变量的值
static PyObject* get_shared_variable(PyObject* self, PyObject* args) {
    const char* str;
    if (!PyArg_ParseTuple(args, "s", &str)) {
        return NULL;
    }

    printf("request_name: %s [GET] '%s'\n", ptrInfo->c_str(), str);
    std::string s = convertToStdString(str);

    const auto& context_map = ptrMatch->context_map();
    auto it = context_map.find(s);
    if (it == context_map.end()) {
        return NULL;
    }

    context_value giveaway;
    std::string msg;
    giveaway = it->second;
    giveaway.SerializeToString(&msg);

    PyObject* tmp = Py_BuildValue("y#", msg.c_str(), msg.length());

    return tmp;  // dangerous?
}

// 设置全局变量的值
static PyObject* set_shared_variable(PyObject* self, PyObject* args) {
    int new_value;
    const char* str;

    if (!PyArg_ParseTuple(args, "si", &str, &new_value)) {
        return NULL;
    }
    printf("request_name: %s [SET] '%s'\n", ptrInfo->c_str(), str);

    std::string s = convertToStdString(str);

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

void py_init() {
    static bool initiated = false;

    if (initiated) return;
    initiated = true;

    PyImport_AppendInittab("mymodule", &PyInit_mymodule);
    Py_Initialize();
}

void py_end() {
    Py_Finalize();
    ;
}

void call_init(MatchRuleReq& maps, const std::string& info) {
    ptrMatch = &maps;
    ptrInfo = &info;
}

void call_end() {
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

int main() {
    MatchRuleReq tmp1, tmp2;
    context_value val;

    for (int i = 0; i < 10; ++i) val.mutable_array_int64()->add_data(i);
    tmp1.mutable_context_map()->insert({"key1", val});

    val.set_float_(1.2);
    tmp2.mutable_context_map()->insert({"key1", val});

    std::string script1;
    load_file(script1, std::string("./main1.py"));

    call_python(script1, tmp1, "info1");
    call_python(script1, tmp2, "info2");
    return 0;
}