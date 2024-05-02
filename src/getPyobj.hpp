#pragma once

#include <Python.h>

#include "data.pb.h"

#define setErrorCombo(s)              \
    fprintf(stderr, s, __FUNCTION__); \
    PyErr_Format(PyExc_RuntimeError, s, __FUNCTION__)

#define detectNullNewObject(object)                                       \
    if (object == nullptr) {                                              \
        setErrorCombo("[Error] Failed to create a new object in %s()\n"); \
        return nullptr;                                                   \
    }

#define detectNullNewSubobject(father, object)                            \
    if (object == nullptr) {                                              \
        setErrorCombo("[Error] Failed to create a new object in %s()\n"); \
        Py_DECREF(father);                                                \
        return nullptr;                                                   \
    }

#define detectNullNewSubobject_2father(father1, father2, object)          \
    if (object == nullptr) {                                              \
        setErrorCombo("[Error] Failed to create a new object in %s()\n"); \
        Py_DECREF(father1);                                               \
        Py_DECREF(father2);                                               \
        return nullptr;                                                   \
    }

static PyObject* getPyobjFromArrayInt64(const ArrayInt64* arrayInt64) {
    PyObject* pyList = PyList_New(0);

    detectNullNewObject(pyList);

    for (int i = 0; i < arrayInt64->data_size(); i++) {
        PyObject* pyValue = PyLong_FromLongLong(arrayInt64->data(i));
        detectNullNewSubobject(pyList, pyValue);

        PyList_Append(pyList, pyValue);

        Py_DECREF(pyValue);
    }

    return pyList;
}

static PyObject* getPyobjFromArrayUint64(const ArrayUint64* arrayUint64) {
    PyObject* pyList = PyList_New(0);

    detectNullNewObject(pyList);

    for (int i = 0; i < arrayUint64->data_size(); i++) {
        PyObject* pyValue = PyLong_FromUnsignedLongLong(arrayUint64->data(i));
        detectNullNewSubobject(pyList, pyValue);

        PyList_Append(pyList, pyValue);

        Py_DECREF(pyValue);
    }

    return pyList;
}

static PyObject* getPyobjFromArrayDouble(const ArrayDouble* arrayDouble) {
    PyObject* pyList = PyList_New(0);

    detectNullNewObject(pyList);

    for (int i = 0; i < arrayDouble->data_size(); i++) {
        PyObject* pyValue = PyFloat_FromDouble(arrayDouble->data(i));
        detectNullNewSubobject(pyList, pyValue);

        PyList_Append(pyList, pyValue);

        Py_DECREF(pyValue);
    }

    return pyList;
}

static PyObject* getPyobjFromArrayString(const ArrayString* arrayString) {
    PyObject* pyList = PyList_New(0);

    detectNullNewObject(pyList);

    for (int i = 0; i < arrayString->data_size(); i++) {
        PyObject* pyValue = PyUnicode_FromString(arrayString->data(i).c_str());
        detectNullNewSubobject(pyList, pyValue);

        PyList_Append(pyList, pyValue);

        Py_DECREF(pyValue);
    }

    return pyList;
}

static PyObject* getPyobjFromContext(const context_value* value);

static PyObject* getPyobjFromArrayValue(const ArrayValue* arrayValue) {
    PyObject* pyList = PyList_New(0);

    detectNullNewObject(pyList);

    for (int i = 0; i < arrayValue->data_size(); i++) {
        PyObject* pyValue = getPyobjFromContext(&arrayValue->data(i));
        detectNullNewSubobject(pyList, pyValue);

        PyList_Append(pyList, pyValue);

        Py_DECREF(pyValue);
    }

    return pyList;
}

static PyObject* getPyobjFromMapString(const MapString* mapString) {
    PyObject* pyDict = PyDict_New();

    detectNullNewObject(pyDict);

    for (const auto& pair : mapString->data()) {
        PyObject* pyKey = PyUnicode_FromString(pair.first.c_str());
        detectNullNewSubobject(pyDict, pyKey);

        PyObject* pyValue = getPyobjFromContext(&pair.second);
        detectNullNewSubobject_2father(pyDict, pyKey, pyValue);

        PyDict_SetItem(pyDict, pyKey, pyValue);

        Py_DECREF(pyKey);
        Py_DECREF(pyValue);
    }

    return pyDict;
}

PyObject* getPyobjFromContext(const context_value* value) {
    switch (value->value_type_case()) {
        case value->kBool: {
            return PyBool_FromLong(value->bool_());
        }
        case value->kFloat: {
            return Py_BuildValue("f", value->double_());
            // return Py_BuildValue("O&", PyFloat_FromDouble, value->float_());
        }
        case value->kDouble: {
            return Py_BuildValue("d", value->double_());
            // return Py_BuildValue("O&", PyFloat_FromDouble, value->double_());
        }
        case value->kUint32: {
            return Py_BuildValue("O&", PyLong_FromUnsignedLong,
                                 value->uint32());
        }
        case value->kUint64: {
            return Py_BuildValue("O&", PyLong_FromUnsignedLongLong,
                                 value->uint64());
        }
        case value->kInt32: {
            return Py_BuildValue("O&", PyLong_FromLong, value->int32());
        }
        case value->kInt64: {
            // 我不知道为什么这个返回的 PyObject 引用计数是9...
            // 现在又变成 4 了，怪
            // 文档上说没关系，那就没关系吧
            return Py_BuildValue("O&", PyLong_FromLongLong, value->int64());
        }
        case value->kSint32: {
            return Py_BuildValue("i", value->sint32());
            // return Py_BuildValue("O&", PyLong_FromLong, value->sint32());
        }
        case value->kSint64: {
            return Py_BuildValue("O&", PyLong_FromLongLong, value->sint64());
        }
        case value->kFixed32: {
            return Py_BuildValue("O&", PyLong_FromUnsignedLong,
                                 value->fixed32());
        }
        case value->kFixed64: {
            return Py_BuildValue("O&", PyLong_FromUnsignedLongLong,
                                 value->fixed64());
        }
        case value->kSfixed32: {
            return Py_BuildValue("i", value->sfixed32());
            // return Py_BuildValue("O&", PyLong_FromLong, value->sfixed32());
        }
        case value->kSfixed64: {
            return Py_BuildValue("O&", PyLong_FromLongLong, value->sfixed64());
        }
        case value->kString: {
            return Py_BuildValue("O&", PyUnicode_FromString,
                                 value->string().c_str());
        }
        case value->kBytes: {
            return Py_BuildValue("O&", PyBytes_FromString,
                                 value->bytes().c_str());
        }
        case value->kArrayInt64: {
            return getPyobjFromArrayInt64(&value->array_int64());
        }
        case value->kArrayUint64: {
            return getPyobjFromArrayUint64(&value->array_uint64());
        }
        case value->kArrayDouble: {
            return getPyobjFromArrayDouble(&value->array_double());
        }
        case value->kArrayString: {
            return getPyobjFromArrayString(&value->array_string());
        }
        case value->kArrayValue: {
            return getPyobjFromArrayValue(&value->array_value());
        }
        case value->kMapString: {
            return getPyobjFromMapString(&value->map_string());
        }
        default: {
            fprintf(stderr, "[Error] Unrecongizable type in %s().\n",
                    __FUNCTION__);
            fprintf(stderr, "[INFO] Only allows basic data type.\n");
            PyErr_SetString(PyExc_RuntimeError, "[Error] Unrecongizable type.");
            break;
        }
    }
    return nullptr;
}