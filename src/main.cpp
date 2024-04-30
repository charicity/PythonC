#ifndef PY_SSIZE_T_CLEAN
#define PY_SSIZE_T_CLEAN
#endif

#include <Python.h>

#include <cstdio>
#include <cstring>
#include <iostream>

#include "CPython.hpp"
#include "data.pb.h"
#include "dbg_utili.hpp"

int main() {
    MatchRuleReq tmp1, tmp2;
    context_value val;

    std::string script2 = load_file("./main2.py", false);

    for (int i = 0; i < 10; ++i) {
        val.mutable_array_int64()->add_data(i);
    }

    tmp1.mutable_context_map()->insert({"key1", val});

    call_python(script2, tmp1, "info2");

    context_value& tmp_context = (*tmp1.mutable_context_map())["key1"];
    if (tmp_context.has_array_int64()) {
        std::cout << "MAP[key1]" << std::endl;
        for (const auto& i : tmp_context.array_int64().data()) {
            std::cout << i << std::endl;
        }
    }

    return 0;
}