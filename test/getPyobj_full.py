# # -*- coding:UTF-8 -*-
import os
import sys
import numpy
sys.path.append('.')
import mymodule

if 'k' not in locals() and 'k' not in globals():
    k = 10

def test_getPyobj(strs):
    print('-------------')
    print('getting obj \'',strs,'\'',sep="")
    obj = mymodule.get_Pyobj(strs)
    print(obj)
    print('ref count=',sys.getrefcount(obj))
    print('type=',type(obj))
    print('-------------')

print('full test of getPyobj')

test_getPyobj("bool")
test_getPyobj("float")
test_getPyobj("double")
test_getPyobj("uint32")
test_getPyobj("uint64")

test_getPyobj("int32")
test_getPyobj("int64")
test_getPyobj("sint32")
test_getPyobj("sint64")
test_getPyobj("fixed32")

test_getPyobj("fixed64")
test_getPyobj("sfixed32")
test_getPyobj("sfixed64")
test_getPyobj("string")

test_getPyobj("bytes")
test_getPyobj("array_int64")
test_getPyobj("array_uint64")
test_getPyobj("array_double")
test_getPyobj("array_string")

test_getPyobj("array_value")
test_getPyobj("map_string")