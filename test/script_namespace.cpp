#include <Python.h>

#include <cstdio>
#include <cstring>
#include <iostream>

#include "CPython.hpp"
#include "dbg_utili.hpp"

int main() {
    PyObject *code1 =
        compile_python(load_file("./script_namespace1.py", false), "namespace");
    PyObject *code2 =
        compile_python(load_file("./script_namespace2.py", false), "namespace");
    MatchRuleReq maps;
    int i;
    // 同一个脚本 命名空间不共享
    printf("same script, different script namespace:\n");

    (*maps.mutable_context_map())["shared"].set_int32(1);
    i = 3;
    while (i--) {
        std::cerr << "loop: " << i << std::endl;
        call_python(code1, maps, "noinfo");
    }
    printf("---------------------------------------\n");

    // 不同的脚本 命名空间不共享
    printf("different script, different script namespace:\n");
    i = 3;
    while (i--) {
        std::cerr << "loop: " << i << std::endl;
        call_python(code1, maps, "noinfo");
        call_python(code2, maps, "noinfo");
    }
    printf("---------------------------------------\n");
    return 0;
}