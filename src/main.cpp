// #ifndef PY_SSIZE_T_CLEAN
// #define PY_SSIZE_T_CLEAN
// #endif

#include <Python.h>

#include <cstdio>
#include <cstring>
#include <iostream>

#include "CPython.hpp"
#include "data.pb.h"
#include "dbg_utili.hpp"

int main() {
    MatchRuleReq tmp1, tmp2;

    std::string script2 = load_file("./getPyobj_full.py", false);

    tmpPlaceAllTypes(tmp1);

    call_python(script2, tmp1, "test get_pyobj int64");

    return 0;
}