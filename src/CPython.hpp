#pragma once
// 全局变量
#include "data.pb.h"
#include "getPyobj.hpp"
#include "setPyobj.hpp"

static MatchRuleReq* ptrMatch = NULL;
static const std::string* ptrInfo = NULL;

// python 调用 get_pyobj(name) ，返回 PyObject
static PyObject* get_Pyobj(PyObject* self, PyObject* args) {
    char* str;
    if (!PyArg_ParseTuple(args, "s", &str)) {
        return NULL;
    }
    std::string key = str;

    auto& map_ref = (*ptrMatch->mutable_context_map());
    auto it = map_ref.find(key);
    if (it == map_ref.end()) {
        return NULL;
    }

    return getPyobjFromContext(&it->second);
}

// python调用 set_pyobj(PyObject, name) ，设置对应 MatchRuleReq ，无返回
static PyObject* set_Pyobj(PyObject* self, PyObject* args) {
    char* str;
    PyObject* object = NULL;
    if (!PyArg_ParseTuple(args, "Os", &object, &str) || object == NULL) {
        return NULL;
    }
    std::string key = str;

    auto& context = (*ptrMatch->mutable_context_map())[key];

    setFromPyObj(object, &context);

    Py_RETURN_NONE;
}

/* protobuf 的通信，不建议使用*/

// 设置全局变量的值
static PyObject* set_Protobuf(PyObject* self, PyObject* args) {
    PyObject* new_value;
    char* str;
    char* data;

    if (!PyArg_ParseTuple(args, "sy*", &str, &data)) {
        // y* 会有 Memory Leak ，但是这个方法不建议使用，所以晚一点再修
        // 需要 PyBuffer_Release();
        return NULL;
    }

    context_value fetch;
    fetch.ParseFromArray(data, strlen(data));

    printf("request_name: %s [SET] '%s'\n", ptrInfo->c_str(), str);

    std::string key = str;

    auto& map_ref = (*ptrMatch->mutable_context_map());
    map_ref[key] = fetch;

    Py_RETURN_NONE;
}

// 获取全局变量的值
static PyObject* get_Protobuf(PyObject* self, PyObject* args) {
    char* str;
    if (!PyArg_ParseTuple(args, "s", &str)) {
        return NULL;
    }

    printf("request_name: %s [GET] '%s'\n", ptrInfo->c_str(), str);
    std::string key = str;

    auto& map_ref = (*ptrMatch->mutable_context_map());
    auto it = map_ref.find(key);
    if (it == map_ref.end()) {
        return NULL;
    }

    context_value ret;
    std::string msg;
    ret = it->second;
    ret.SerializeToString(&msg);

    return Py_BuildValue("y", msg.c_str());
}

/*接口区*/

// 定义方法
static PyMethodDef myModuleMethods[] = {
    {"get_Protobuf", get_Protobuf, METH_VARARGS, "Get protobuf by name"},
    {"set_Protobuf", set_Protobuf, METH_VARARGS, "Set protobuf by name"},
    {"get_Pyobj", get_Pyobj, METH_VARARGS, "Get pyobj by name"},
    {"set_Pyobj", set_Pyobj, METH_VARARGS, "Set pyobj to C++ host by name"},
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
    static std::string clean = R"(for key in globals().copy(): 
    if not key.startswith("__"):
        del key)";
    PyRun_SimpleString(clean.c_str());
}

void call_python(const std::string& script, MatchRuleReq& maps,
                 const std::string& info) {
    py_init();
    call_init(maps, info);

    PyRun_SimpleString(script.c_str());

    call_end();
}