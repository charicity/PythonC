#include <Python.h>

#include <cstdio>
#include <cstring>
#include <iostream>

#include "CPython.hpp"
#include "dbg_utili.hpp"

int main() {
    MatchRuleReq tmp1, tmp2;

    std::string script2 = load_file("./setPyobj_full.py", false);

    tmpPlaceAllTypes(tmp1);

    call_python(script2, tmp1, "test getPyobj_full");

    return 0;
}