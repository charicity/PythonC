#include <Python.h>

#include <cstdio>
#include <cstring>
#include <iostream>

#include "CPython.hpp"
#include "dbg_utili.hpp"

int main() {
    MatchRuleReq maps1, maps2, maps3;
    PyObject *fill_method1 =
        compile_python(load_file("./map_namespace1.py", false), "diff");
    PyObject *fill_method2 =
        compile_python(load_file("./map_namespace2.py", false), "diff");
    PyObject *reveal =
        compile_python(load_file("./map_namespace3.py", false), "diff");
    PyObject *add1 =
        compile_python(load_file("./map_namespace4.py", false), "diff");

    // 不同的脚本 访问 同一个 Maps
    printf("different script, same map namespace\n");

    call_python(fill_method1, maps1, "noinfo");
    call_python(fill_method2, maps1, "noinfo");
    call_python(reveal, maps1, "noinfo");
    printf("---------------------------------------\n");

    // 不同的脚本 访问 不同 Maps
    printf("different script, different map namespace\n");

    call_python(fill_method1, maps2, "noinfo");
    call_python(fill_method2, maps3, "noinfo");
    call_python(reveal, maps2, "noinfo");
    call_python(reveal, maps3, "noinfo");
    printf("---------------------------------------\n");

    // 同一个脚本 访问 不同 Maps
    (*maps1.mutable_context_map())["1"].set_int32(0);
    (*maps1.mutable_context_map())["2"].set_int32(0);

    (*maps2.mutable_context_map())["1"].set_int32(0);
    (*maps2.mutable_context_map())["2"].set_int32(0);
    printf("same script, different map namespace\n");

    call_python(add1, maps1, "noinfo");
    call_python(add1, maps2, "noinfo");
    call_python(reveal, maps1, "noinfo");
    call_python(reveal, maps2, "noinfo");
    printf("---------------------------------------\n");
    return 0;
}