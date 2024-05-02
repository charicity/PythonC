#pragma once

#include <Python.h>

#include "data.pb.h"
#include "dbg_utili.hpp"

#define setErrorCombo(s)              \
    fprintf(stderr, s, __FUNCTION__); \
    PyErr_Format(PyExc_RuntimeError, s, __FUNCTION__)

bool setFromPyObj(PyObject* object, context_value* context);

static bool isBasicType(PyObject* object) {
    PyTypeObject* type = Py_TYPE(object);  // 不需要处理
    if (type == &PyBool_Type) {
        return true;
    } else if (type == &PyLong_Type) {
        return true;
    } else if (type == &PyBytes_Type) {
        return true;
    } else if (type == &PyUnicode_Type) {
        return true;
    } else if (type == &PyFloat_Type) {
        return true;
    } else if (type == &PyList_Type) {
        return true;
    } else if (type == &PyDict_Type) {
        return true;
    } else if (type == &PyTuple_Type) {
        return true;
    } else if (type == &_PyNone_Type) {
        fprintf(stderr, "[Warning] Setting none type.");
        return true;
    }
    return false;
}

/*以下的这个 setPyobjBy'xxtype' 都是经过分流的，不需要额外判断类型*/
static bool setPyobjByBool(PyObject* object, context_value* site) {
    bool val = static_cast<bool>(PyLong_AsLong(object));  // hummm
    site->set_bool_(val);
    return true;
}

static bool setPyobjByLong(PyObject* object, context_value* site) {
    // 可以继续优化数据的类型
    int overflow;

    {
        long long val = PyLong_AsLongLongAndOverflow(object, &overflow);
        if (!overflow) {
            site->set_int64(val);
            return false;
        }
    }

    {
        unsigned long long val = PyLong_AsUnsignedLongLong(object);
        if (PyErr_ExceptionMatches(PyExc_OverflowError)) {
            fprintf(stderr, "[Error] PyLong type overflowed in %s().\n",
                    __FUNCTION__);
            return false;
        }
        site->set_uint64(val);
    }
    return true;
}

static bool setPyobjByBytes(PyObject* object, context_value* site) {
    auto val = PyBytes_AsString(object);
    // 文档没说别的异常...
    site->set_bytes(val);
    return true;
}

static bool setPyobjByString(PyObject* object, context_value* site) {
    auto val = PyUnicode_AsUTF8(object);
    // 文档没说别的异常...
    site->set_string(val);
    return true;
}

static bool setPyobjByFloat(PyObject* object, context_value* site) {
    auto val = PyFloat_AsDouble(object);
    // 限制了 float 类型，所以不用异常处理
    site->set_double_(val);
    return true;
}

static bool setPyobjByList(PyObject* object, context_value* site) {
    ArrayValue* tmp = new ArrayValue();
    if (tmp == nullptr) {
        setErrorCombo("[Error] Failed to malloc for ArrayValue in %s().\n");
        return false;
    }
    Py_ssize_t size = PyList_Size(object);

    for (Py_ssize_t i = 0; i < size; ++i) {
        PyObject* item = PyList_GetItem(object, i);  // 借入引用

        if (!isBasicType(item)) {
            delete tmp;
            setErrorCombo("[Error] List contains non-basic values in %s().\n");
            return false;
        }

        auto ptr = tmp->add_data();
        if (!setFromPyObj(item, ptr)) {
            setErrorCombo("[Error] Failed to set PyObj in %s().\n");
            return false;
        }
    }
    site->set_allocated_array_value(tmp);  // 指针管理转移
    tmp = nullptr;
    return true;
}

static bool setPyobjByDict(PyObject* object, context_value* site) {
    PyObject *key, *value;
    Py_ssize_t pos = 0;
    MapString* tmp = new MapString();

    while (PyDict_Next(object, &pos, &key, &value)) {
        if (PyUnicode_Check(key)) {
            std::string keyStr(PyUnicode_AsUTF8(key));

            if (!setFromPyObj(value, &(*tmp->mutable_data())[keyStr])) {
                setErrorCombo("[Error] Failed to set PyObj in %s().\n");
                return false;
            }
        } else {
            delete tmp;
            setErrorCombo("Dict's key is not str type in %s().\n");
            return false;
        }
    }
    site->set_allocated_map_string(tmp);
    return true;
}

static bool setPyobjByTuple(PyObject* object, context_value* site) {
    ArrayValue* tmp = new ArrayValue();
    Py_ssize_t size = PyTuple_Size(object);

    for (Py_ssize_t i = 0; i < size; ++i) {
        PyObject* item = PyTuple_GetItem(object, i);  // 借入引用

        if (!isBasicType(item)) {
            delete tmp;
            setErrorCombo(
                "[ERROR] Tuple contains not basical value in %s().\n");
            return false;
        }

        auto ptr = tmp->add_data();
        if (!setFromPyObj(item, ptr)) {
            delete tmp;
            setErrorCombo("[Error] Failed to set PyObj in %s().\n");
            return false;
        }
    }
    site->set_allocated_array_value(tmp);
    return true;
}

bool setFromPyObj(PyObject* object, context_value* context) {
    PyTypeObject* type = Py_TYPE(object);
    if (type == &PyBool_Type) {
        return setPyobjByBool(object, context);
    } else if (type == &PyLong_Type) {
        return setPyobjByLong(object, context);
        // 数据类型可以优化
    } else if (type == &PyBytes_Type) {
        // 二进制安全可以加上？
        return setPyobjByBytes(object, context);
    } else if (type == &PyFloat_Type) {
        // 看起来只能变成 double 类型...
        return setPyobjByFloat(object, context);
    } else if (type == &PyUnicode_Type) {
        return setPyobjByString(object, context);
    } else if (type == &PyList_Type) {
        return setPyobjByList(object, context);
    } else if (type == &PyDict_Type) {
        return setPyobjByDict(object, context);
    } else if (type == &PyTuple_Type) {
        return setPyobjByTuple(object, context);
    }
    return false;
}