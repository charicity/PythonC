#pragma once
#include <Python.h>

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include "PythonC.hpp"

std::string convertToStdString(const char* c_str) {
    if (c_str == nullptr) {
        return "";  // 如果输入是空指针，则返回空字符串
    }
    return std::string(c_str);
}

void py_ref_count(const PyObject* object, const char* s = "") {
    printf("Reference count of %s: %ld\n", s, Py_REFCNT(object));
}

std::string load_file(const std::string& file, bool display = false) {
    std::ifstream infile(file.c_str());
    std::stringstream buffer;

    if (infile.is_open()) {
        buffer << infile.rdbuf();
        infile.close();
    } else {
        std::cerr << "Failed!" << std::endl;
    }

    if (display) {
        std::cout << "-------LOADED-------" << std::endl;
        std::cout << buffer.str() << std::endl;
        std::cout << "-------LOADED-------" << std::endl;
    }

    return buffer.str();
}

void tmpPlaceAllTypes(MatchRuleReq& maps) {
    context_value val;
    context_value arr_val;
    context_value map_str;

    (*maps.mutable_context_map())["bool"].set_bool_(true);
    (*maps.mutable_context_map())["float"].set_float_(1);
    (*maps.mutable_context_map())["double"].set_double_(5.5);

    (*maps.mutable_context_map())["uint32"].set_uint32(-1919);
    (*maps.mutable_context_map())["uint64"].set_uint64(-810);

    (*maps.mutable_context_map())["int32"].set_int32(-1919);
    (*maps.mutable_context_map())["int64"].set_int64(-810);
    (*maps.mutable_context_map())["sint32"].set_sint32(-810);
    (*maps.mutable_context_map())["sint64"].set_sint64(-810);

    (*maps.mutable_context_map())["fixed32"].set_fixed32(-810);
    (*maps.mutable_context_map())["fixed64"].set_fixed32(-810);

    (*maps.mutable_context_map())["sfixed32"].set_sfixed32(-810);
    (*maps.mutable_context_map())["sfixed64"].set_sfixed64(-810);

    (*maps.mutable_context_map())["string"].set_string("ximalaya");
    (*maps.mutable_context_map())["bytes"].set_bytes("chigua");

    for (int i = 0; i < 10; ++i) {
        val.mutable_array_int64()->add_data(i);
    }
    (*maps.mutable_context_map())["array_int64"] = val;
    (*map_str.mutable_map_string()->mutable_data())["array_int64"] =
        (*maps.mutable_context_map())["array_int64"];  // 加上map

    for (int i = 10; i > 0; --i) {
        val.mutable_array_uint64()->add_data(i);
    }
    (*maps.mutable_context_map())["array_uint64"] = val;
    (*map_str.mutable_map_string()->mutable_data())["array_uint64"] =
        (*maps.mutable_context_map())["array_uint64"];  // 加上map

    for (double i = 10.5; i > 0.5; i -= 1) {
        val.mutable_array_double()->add_data(i);
    }
    (*maps.mutable_context_map())["array_double"] = val;
    (*map_str.mutable_map_string()->mutable_data())["array_double"] =
        (*maps.mutable_context_map())["array_double"];  // 加上map

    std::string maker = "";
    char bro = 'a';
    do {
        val.mutable_array_string()->add_data(maker);
        maker += bro;
        bro++;
    } while (bro <= 'z');
    (*maps.mutable_context_map())["array_string"] = val;

    (*maps.mutable_context_map())["int32"].set_int32(321);

    (*maps.mutable_context_map())["string"].set_string("ximalaya");

    (*maps.mutable_context_map())["int64"].set_int64(123);

    *arr_val.mutable_array_value()->add_data() =
        (*maps.mutable_context_map())["int32"];  // 这个接口太丑了吧...
    *arr_val.mutable_array_value()->add_data() =
        (*maps.mutable_context_map())["array_int64"];
    *arr_val.mutable_array_value()->add_data() =
        (*maps.mutable_context_map())["array_string"];

    (*maps.mutable_context_map())["array_value"] = arr_val;

    (*maps.mutable_context_map())["map_string"] = map_str;
}

void writeStringToFile(std::string& s) {
    std::ofstream out("output.txt");
    out << s;
    out.close();
}