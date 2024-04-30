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

    std::cout << val.has_array_int64() << std::endl;
    std::cout << (val.value_type_case() == val.kArrayInt64) << std::endl;
    std::cout << (val.value_type_case() == val.kBool) << std::endl;

    return 0;
}