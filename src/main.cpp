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
    context_value val;

    std::string script2 = load_file("./main2.py", false);

    for (int i = 0; i < 10; ++i) {
        val.mutable_array_int64()->add_data(i);
    }
    (*tmp1.mutable_context_map())["array_int64"] = val;

    for (int i = 10; i > 0; --i) {
        val.mutable_array_uint64()->add_data(i);
    }
    (*tmp1.mutable_context_map())["array_uint64"] = val;

    for (double i = 10.5; i > 0.5; i -= 1) {
        val.mutable_array_double()->add_data(i);
    }
    (*tmp1.mutable_context_map())["array_double"] = val;

    std::string maker = "";
    char bro = 'a';
    do {
        val.mutable_array_string()->add_data(maker);
        maker += bro;
        bro++;
    } while (bro <= 'z');
    (*tmp1.mutable_context_map())["array_string"] = val;

    (*tmp1.mutable_context_map())["int32"].set_int32(321);
    (*tmp1.mutable_context_map())["string"].set_string("ximalaya");

    (*tmp1.mutable_context_map())["bool"].set_bool_(true);
    (*tmp1.mutable_context_map())["int64"].set_int64(123);

    // int i = 1;
    // while (i--) {
    //     call_python(script2, tmp1, "test get_pyobj int64");
    // }
    call_python(script2, tmp1, "test get_pyobj int64");

    std::cout << val.has_array_int64() << std::endl;
    std::cout << (val.value_type_case() == val.kArrayInt64) << std::endl;
    std::cout << (val.value_type_case() == val.kBool) << std::endl;

    return 0;
}