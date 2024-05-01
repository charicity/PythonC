#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>

#include "data.pb.h"
#include "dbg_utili.hpp"

// 不会高端的模板操作...
template <typename T>
inline void setMatch_bool(MatchRuleReq &maps, const std::string &name, T &a) {
    auto &map_ref = *maps.mutable_context_map();
    map_ref[name].set_bool_(a);
}

template <typename T>
inline void setMatch_float(MatchRuleReq &maps, const std::string &name, T &a) {
    auto &map_ref = *maps.mutable_context_map();
    map_ref[name].set_float_(a);
}

template <typename T>
inline void setMatch_double(MatchRuleReq &maps, const std::string &name, T &a) {
    auto &map_ref = *maps.mutable_context_map();
    map_ref[name].set_double_(a);
}

template <typename T>
inline void setMatch_uint32(MatchRuleReq &maps, const std::string &name, T &a) {
    auto &map_ref = *maps.mutable_context_map();
    map_ref[name].set_uint32(a);
}

template <typename T>
inline void setMatch_uint64(MatchRuleReq &maps, const std::string &name, T &a) {
    auto &map_ref = *maps.mutable_context_map();
    map_ref[name].set_uint64(a);
}

template <typename T>
inline void setMatch_int32(MatchRuleReq &maps, const std::string &name, T &a) {
    auto &map_ref = *maps.mutable_context_map();
    map_ref[name].set_int32(a);
}

template <typename T>
inline void setMatch_int64(MatchRuleReq &maps, const std::string &name, T &a) {
    auto &map_ref = *maps.mutable_context_map();
    map_ref[name].set_int64(a);
}

template <typename T>
inline void setMatch_sint32(MatchRuleReq &maps, const std::string &name, T &a) {
    auto &map_ref = *maps.mutable_context_map();
    map_ref[name].set_sint32(a);
}

template <typename T>
inline void setMatch_sint64(MatchRuleReq &maps, const std::string &name, T &a) {
    auto &map_ref = *maps.mutable_context_map();
    map_ref[name].set_sint64(a);
}

template <typename T>
inline void setMatch_fixed32(MatchRuleReq &maps, const std::string &name,
                             T &a) {
    auto &map_ref = *maps.mutable_context_map();
    map_ref[name].set_fixed32(a);
}

template <typename T>
inline void setMatch_fixed64(MatchRuleReq &maps, const std::string &name,
                             T &a) {
    auto &map_ref = *maps.mutable_context_map();
    map_ref[name].set_fixed64(a);
}

template <typename T>
inline void setMatch_sfixed32(MatchRuleReq &maps, const std::string &name,
                              T &a) {
    auto &map_ref = *maps.mutable_context_map();
    map_ref[name].set_sfixed32(a);
}

template <typename T>
inline void setMatch_sfixed64(MatchRuleReq &maps, const std::string &name,
                              T &a) {
    auto &map_ref = *maps.mutable_context_map();
    map_ref[name].set_sfixed64(a);
}

template <typename T>
inline void setMatch_string(MatchRuleReq &maps, const std::string &name, T &a) {
    auto &map_ref = *maps.mutable_context_map();
    map_ref[name].set_string(a);
}

void setMatch_single(MatchRuleReq &maps, const std::string &name, int a) {
    context_value tmp_val;
}