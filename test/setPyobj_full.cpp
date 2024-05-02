#include <Python.h>

#include <cstdio>
#include <cstring>
#include <iostream>

#include "PythonC.hpp"
#include "dbg_utili.hpp"

int main() {
    currentRunSingle("./setPyobj_full.py");
    pythoncNamespace::shutdownInterpreter();

    return 0;
}