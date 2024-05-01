# # -*- coding:UTF-8 -*-
import os
import sys
import numpy
sys.path.append('.')
import data_pb2
import mymodule

if 'k' not in locals() and 'k' not in globals():
    k = 10

def test_getpyobj(strs):
    print('-------------')
    print('getting obj \'',strs,'\'',sep="")
    obj = mymodule.get_pyobj(strs)
    print(obj)
    print('ref count=',sys.getrefcount(obj))
    print('type=',type(obj))
    print('-------------')

if __name__ == "__main__":
    print('full test of getPyobj')

    test_getpyobj("bool")
    test_getpyobj("float")
    test_getpyobj("double")

    test_getpyobj("uint32")
    test_getpyobj("uint64")
    test_getpyobj("int32")

    test_getpyobj("int64")
    test_getpyobj("sint32")
    test_getpyobj("sint64")

    test_getpyobj("fixed32")
    test_getpyobj("fixed64")
    test_getpyobj("sfixed32")

    test_getpyobj("sfixed64")
    test_getpyobj("string")
    test_getpyobj("bytes")

    test_getpyobj("array_int64")
    test_getpyobj("array_uint64")
    test_getpyobj("array_double")

    test_getpyobj("array_string")
    test_getpyobj("array_value")
    test_getpyobj("map_string")