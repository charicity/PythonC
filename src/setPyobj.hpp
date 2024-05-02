#pragma once

#include <Python.h>

#include "data.pb.h"
#include "dbg_utili.hpp"

#define setErrorCombo(s)              \
    fprintf(stderr, s, __FUNCTION__); \
    PyErr_Format(PyExc_RuntimeError, s, __FUNCTION__)

void setFromPyObj(PyObject* object, context_value* context);

static bool isBasicType(PyObject* object) {
    PyTypeObject* type = Py_TYPE(object);
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
    }
    return false;
}

static void setPyobjByBool(PyObject* object, context_value* site) {
    long val = PyLong_AsLong(object);
    site->set_bool_(val);
}

static void setPyobjByLong(PyObject* object, context_value* site) {
    // 可以继续优化数据的类型
    int overflow;

    {
        long long val = PyLong_AsLongLongAndOverflow(object, &overflow);
        if (!overflow) {
            site->set_int64(val);
            return;
        }
    }

    {
        unsigned long long val = PyLong_AsUnsignedLongLong(object);
        site->set_uint64(val);
        return;
    }

    PyErr_SetString(PyExc_RuntimeError, "Integer too long!");
}

static void setPyobjByBytes(PyObject* object, context_value* site) {
    auto val = PyBytes_AsString(object);
    site->set_bytes(val);
}

static void setPyobjByString(PyObject* object, context_value* site) {
    auto val = PyUnicode_AsUTF8(object);
    site->set_string(val);
}

static void setPyobjByFloat(PyObject* object, context_value* site) {
    auto val = PyFloat_AsDouble(object);
    site->set_double_(val);
}

static void setPyobjByList(PyObject* object, context_value* site) {
    ArrayValue* tmp = new ArrayValue();
    Py_ssize_t size = PyList_Size(object);

    for (Py_ssize_t i = 0; i < size; ++i) {
        PyObject* item = PyList_GetItem(object, i);  // 窃取引用

        if (!isBasicType(item)) {
            delete tmp;
            PyErr_SetString(PyExc_TypeError, "List contains not basical value");
            return;
        }

        auto ptr = tmp->add_data();
        setFromPyObj(item, ptr);
    }
    site->set_allocated_array_value(tmp);
}

static void setPyobjByDict(PyObject* object, context_value* site) {
    PyObject *key, *value;
    Py_ssize_t pos = 0;
    MapString* tmp = new MapString();

    while (PyDict_Next(object, &pos, &key, &value)) {
        if (PyUnicode_Check(key)) {
            std::string keyStr(PyUnicode_AsUTF8(key));

            setFromPyObj(value, &(*tmp->mutable_data())[keyStr]);
        } else {
            delete tmp;
            PyErr_SetString(PyExc_TypeError, "Dict's key is not str type");
            return;
        }
    }
    site->set_allocated_map_string(tmp);
}

static void setPyobjByTuple(PyObject* object, context_value* site) {
    ArrayValue* tmp = new ArrayValue();
    Py_ssize_t size = PyTuple_Size(object);

    for (Py_ssize_t i = 0; i < size; ++i) {
        PyObject* item = PyTuple_GetItem(object, i);  // 窃取引用

        if (!isBasicType(item)) {
            delete tmp;
            PyErr_SetString(PyExc_TypeError, "List contains not basical value");
            return;
        }

        auto ptr = tmp->add_data();
        setFromPyObj(item, ptr);
    }
    site->set_allocated_array_value(tmp);
}

void setFromPyObj(PyObject* object, context_value* context) {
    PyTypeObject* type = Py_TYPE(object);
    if (type == &PyBool_Type) {
        setPyobjByBool(object, context);
    } else if (type == &PyLong_Type) {
        setPyobjByLong(object, context);
        // 数据类型可以优化
    } else if (type == &PyBytes_Type) {
        // 二进制安全可以加上？
        setPyobjByBytes(object, context);
    } else if (type == &PyFloat_Type) {
        // 看起来只能变成 double 类型...
        setPyobjByFloat(object, context);
    } else if (type == &PyUnicode_Type) {
        setPyobjByString(object, context);
    } else if (type == &PyList_Type) {
        setPyobjByList(object, context);
    } else if (type == &PyDict_Type) {
        setPyobjByDict(object, context);
    } else if (type == &PyTuple_Type) {
        setPyobjByTuple(object, context);
    }
}