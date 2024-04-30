#pragma once
// 全局变量
// #ifndef PY_SSIZE_T_CLEAN
// #define PY_SSIZE_T_CLEAN
// #endif
#include <Python.h>

#include "data.pb.h"
#include "dbg_utili.hpp"

static MatchRuleReq* ptrMatch = NULL;
static const std::string* ptrInfo = NULL;

// 获取全局变量的值
static PyObject* get_protobuf(PyObject* self, PyObject* args) {
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

PyObject* buildPyobjFromArrayInt64(const ArrayInt64* arrayInt64) {
    PyObject* pyList = PyList_New(0);
    for (int i = 0; i < arrayInt64->data_size(); i++) {
        PyObject* pyValue = PyLong_FromLongLong(arrayInt64->data(i));

        PyList_Append(pyList, pyValue);

        Py_DECREF(pyValue);  // 释放值对象!!!
    }

    return pyList;
}

PyObject* buildPyobjFromArrayUint64(const ArrayUint64* arrayUint64) {
    PyObject* pyList = PyList_New(0);
    for (int i = 0; i < arrayUint64->data_size(); i++) {
        PyObject* pyValue = PyLong_FromUnsignedLongLong(arrayUint64->data(i));

        PyList_Append(pyList, pyValue);

        Py_DECREF(pyValue);  // 释放值对象!!!
    }

    return pyList;
}

PyObject* buildPyobjFromArrayDouble(const ArrayDouble* arrayDouble) {
    PyObject* pyList = PyList_New(0);
    for (int i = 0; i < arrayDouble->data_size(); i++) {
        PyObject* pyValue = PyFloat_FromDouble(arrayDouble->data(i));

        PyList_Append(pyList, pyValue);

        Py_DECREF(pyValue);  // 释放值对象!!!
    }

    return pyList;
}

PyObject* buildPyobjFromArrayString(const ArrayString* arrayString) {
    PyObject* pyList = PyList_New(0);
    for (int i = 0; i < arrayString->data_size(); i++) {
        PyObject* pyValue = PyUnicode_FromString(arrayString->data(i).c_str());

        PyList_Append(pyList, pyValue);

        Py_DECREF(pyValue);  // 释放值对象!!!
    }

    return pyList;
}

// 获取全局变量的值
static PyObject* get_pyobj(PyObject* self, PyObject* args) {
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

    auto& value = it->second;

    switch (value.value_type_case()) {
        case value.kBool: {
            return Py_BuildValue("O&", PyBool_FromLong, value.bool_());
        }
        case value.kFloat: {
            return Py_BuildValue("O&", PyFloat_FromDouble, value.float_());
        }
        case value.kUint32: {
            return Py_BuildValue("O&", PyLong_FromUnsignedLong, value.uint32());
        }
        case value.kUint64: {
            return Py_BuildValue("O&", PyLong_FromUnsignedLongLong,
                                 value.uint64());
        }
        case value.kInt32: {
            return Py_BuildValue("O&", PyLong_FromLong, value.int32());
        }
        case value.kInt64: {
            return Py_BuildValue("O&", PyLong_FromLongLong, value.int64());
        }
        case value.kSint32: {
            return Py_BuildValue("O&", PyLong_FromLong, value.sint32());
        }
        case value.kSint64: {
            return Py_BuildValue("O&", PyLong_FromLongLong, value.sint64());
        }
        case value.kFixed32: {
            return Py_BuildValue("O&", PyLong_FromUnsignedLong,
                                 value.fixed32());
        }
        case value.kFixed64: {
            return Py_BuildValue("O&", PyLong_FromUnsignedLongLong,
                                 value.fixed64());
        }
        case value.kSfixed32: {
            return Py_BuildValue("O&", PyLong_FromLong, value.sfixed32());
        }
        case value.kSfixed64: {
            return Py_BuildValue("O&", PyLong_FromLongLong, value.sfixed64());
        }
        case value.kString: {
            return Py_BuildValue("O&", PyUnicode_FromString,
                                 value.string().c_str());
        }
        case value.kBytes: {
            return Py_BuildValue("O&", PyBytes_FromString,
                                 value.bytes().c_str());
        }
        case value.kArrayInt64: {
            return buildPyobjFromArrayInt64(&value.array_int64());
        }
        case value.kArrayUint64: {
            return buildPyobjFromArrayUint64(&value.array_uint64());
        }
        case value.kArrayDouble: {
            return buildPyobjFromArrayDouble(&value.array_double());
        }
        case value.kArrayString: {
            return buildPyobjFromArrayString(&value.array_string());
        }
        default: {
            std::cerr << "Complex Thingy" << std::endl;
            break;
        }
    }
    return NULL;
}

// 设置全局变量的值
static PyObject* set_protobuf(PyObject* self, PyObject* args) {
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

    auto& map_ref = (*ptrMatch->mutable_context_map());
    map_ref[key] = fetch;

    Py_RETURN_NONE;
}

// 定义方法
static PyMethodDef myModuleMethods[] = {
    {"get_protobuf", get_protobuf, METH_VARARGS, "Get protobuf by name"},
    {"set_protobuf", set_protobuf, METH_VARARGS, "Set protobuf by name"},
    {"get_pyobj", get_pyobj, METH_VARARGS, "Get pyobj by name"},
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
for key in globals().copy(): 
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