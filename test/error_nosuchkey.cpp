#include <Python.h>

#include <cstdio>
#include <cstring>
#include <iostream>

#include "CPython.hpp"
#include "dbg_utili.hpp"

int main() {
    PyObject *code1 =
        compile_python("pythonc.get_Pyobj('notexist')", "namespace");
    MatchRuleReq maps;

    call_python(code1, maps, "error");

    printf("still running...?\n");
    return 0;
}