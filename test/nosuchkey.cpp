#include <Python.h>

#include <cstdio>
#include <cstring>
#include <iostream>

#include "PythonC.hpp"
#include "dbg_utili.hpp"

int main() {
    PyObject *code1 =
        compile_python(load_file("./nosuchkey.py", true), "nokey");
    MatchRuleReq maps;

    call_python(code1, maps, "error");

    pythoncNamespace::shutdownInterpreter();
    return 0;
}