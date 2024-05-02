#include <Python.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <thread>

#include "PythonC.hpp"
#include "dbg_utili.hpp"

void process1to1000000(MatchRuleReq &maps) {
    PyObject *code1 =
        compile_python(load_file("./multithread.py", true), "threads");
    if (code1 == nullptr) {
        printf("failed to compile!\n");
        return;
    }
    fprintf(stderr, "Running...\n");
    call_python(code1, maps, "threads");
}

int main() {
    pythoncNamespace::init_python();
    MatchRuleReq maps1;
    (*maps1.mutable_context_map())["val"].set_int32(0);

    process1to1000000(maps1);

    fprintf(stdout, "im done\n");

    pythoncNamespace::shutdownInterpreter();
    return 0;
}