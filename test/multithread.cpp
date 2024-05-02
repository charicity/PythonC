#include <Python.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <thread>

#include "PythonC.hpp"
#include "dbg_utili.hpp"

void process1to1000000(MatchRuleReq &maps) {
    PyObject *code1 =
        compile_python(load_file("./multithread.py", true), "nokey");
    fprintf(stdout, "Running...\n");
    call_python(code1, maps, "error");
}

int main() {
    pythoncNamespace::init_python();
    MatchRuleReq maps1, maps2;
    (*maps1.mutable_context_map())["val"].set_int32(0);
    (*maps2.mutable_context_map())["val"].set_int32(0);

    std::thread t1(process1to1000000, std::ref(maps1));
    std::thread t2(process1to1000000, std::ref(maps2));

    t1.join();
    t2.join();

    fprintf(stdout, "im done\n");

    pythoncNamespace::shutdownInterpreter();
    return 0;
}