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
    context_value arr_val;
    context_value map_str;

    std::string script2 = load_file("./main2.py", false);

    for (int i = 0; i < 10; ++i) {
        val.mutable_array_int64()->add_data(i);
    }
    (*tmp1.mutable_context_map())["array_int64"] = val;
    (*map_str.mutable_map_string()->mutable_data())["array_int64"] =
        (*tmp1.mutable_context_map())["array_int64"];

    *arr_val.mutable_array_value()->add_data() = val;  // 这个接口太丑了吧...

    for (int i = 10; i > 0; --i) {
        val.mutable_array_uint64()->add_data(i);
    }
    (*tmp1.mutable_context_map())["array_uint64"] = val;
    (*map_str.mutable_map_string()->mutable_data())["array_uint64"] =
        (*tmp1.mutable_context_map())["array_uint64"];

    *arr_val.mutable_array_value()->add_data() = val;  // 这个接口太丑了吧...

    for (double i = 10.5; i > 0.5; i -= 1) {
        val.mutable_array_double()->add_data(i);
    }
    (*tmp1.mutable_context_map())["array_double"] = val;
    (*map_str.mutable_map_string()->mutable_data())["array_double"] =
        (*tmp1.mutable_context_map())["array_double"];

    *arr_val.mutable_array_value()->add_data() = val;  // 这个接口太丑了吧...

    std::string maker = "";
    char bro = 'a';
    do {
        val.mutable_array_string()->add_data(maker);
        maker += bro;
        bro++;
    } while (bro <= 'z');
    (*tmp1.mutable_context_map())["array_string"] = val;

    *arr_val.mutable_array_value()->add_data() = val;  // 这个接口太丑了吧...

    (*tmp1.mutable_context_map())["int32"].set_int32(321);

    *arr_val.mutable_array_value()->add_data() =
        (*tmp1.mutable_context_map())["int32"];  // 这个接口太丑了吧...

    (*tmp1.mutable_context_map())["string"].set_string("ximalaya");

    (*tmp1.mutable_context_map())["bool"].set_bool_(true);
    (*tmp1.mutable_context_map())["int64"].set_int64(123);

    (*tmp1.mutable_context_map())["array_value"] = arr_val;

    (*tmp1.mutable_context_map())["map_string"] = map_str;
    call_python(script2, tmp1, "test get_pyobj int64");

    std::cout << val.has_array_int64() << std::endl;
    std::cout << (val.value_type_case() == val.kArrayInt64) << std::endl;
    std::cout << (val.value_type_case() == val.kBool) << std::endl;

    return 0;
}