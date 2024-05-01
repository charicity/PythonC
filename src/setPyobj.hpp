#pragma once

#include <Python.h>

#include "data.pb.h"
#include "dbg_utili.hpp"

static void setPyobjByBool(PyObject*& object, context_value& site) {
    long val = PyLong_AsLong(object);
    site.set_bool_(val);
    printf("analyzed as %d\n", site);
}

void setFromPyObj(PyObject* object, context_value& context) {
    PyTypeObject* type = Py_TYPE(object);
    if (type == &PyBool_Type) {
        setPyobjByBool(object, context);
        printf("Bool: changed to %d\n", context.bool_());
    } else if (type == &PyLong_Type) {
        ;
    } else if (type == &PyBytes_Type) {
        ;
    } else if (type == &PyDict_Type) {
        ;
    } else if (type == &PyFloat_Type) {
        ;
    } else if (type == &PyList_Type) {
        ;
    } else if (type == &PyLong_Type) {
        ;
    } else if (type == &PyTuple_Type) {
        ;
    }
}