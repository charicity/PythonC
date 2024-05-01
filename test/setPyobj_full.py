# # -*- coding:UTF-8 -*-
import os
import sys
import numpy
sys.path.append('.')
import mymodule

def test_getPyobj(strs):
    # print('-------------')
    # print('getting obj \'',strs,'\'',sep="")
    obj = mymodule.get_Pyobj(strs)
    # print(obj)
    # print('ref count=',sys.getrefcount(obj))
    # print('type=',type(obj))
    # print('-------------')

def test_setPyobj(val, strs):
    # print('-------------')
    # print('setting obj \'',strs,'\'',sep="")
    mymodule.set_Pyobj(val,strs)
    # print('-------------')

print('full test of setPyobj')
test_setPyobj(True, "bool")
test_setPyobj(False, "bool")
test_setPyobj(-2147483648, "long")
test_setPyobj(2147483647, "long")
test_setPyobj(-9223372036854775808, "long")
test_setPyobj(9223372036854775807, "long")
test_setPyobj(9223372036854775808, "long")
# test_setPyobj(29223372036854775808, "long") # overflow
strs="asdf"
test_setPyobj(strs.encode(), "bytes")
test_setPyobj(strs, "string")
test_setPyobj(0.0, "float")
test_setPyobj(0.1, "float")
test_setPyobj(-0.1, "float")
test_setPyobj(114.514, "float")
test_setPyobj(-114.514, "float")
lis = [1,2,3,4]
test_setPyobj(lis, "list")
test_getPyobj("list")
lis = [1,[2.1,2.2],3,4]
test_setPyobj(lis, "list")
test_getPyobj("list")
dic = {'a+b': '30', 'a*b': '11', 'list': lis}
test_setPyobj(dic, "dict")
test_getPyobj("dict")
tup = (1,2,3,4,5)
test_setPyobj(tup, "tuple")
test_getPyobj("tuple")
# test_setPyobj("float")
# test_setPyobj("double")
# test_setPyobj("uint32")
# test_setPyobj("uint64")
# test_setPyobj("int32")
# test_setPyobj("int64")
# test_setPyobj("sint32")
# test_setPyobj("sint64")
# test_setPyobj("fixed32")
# test_setPyobj("fixed64")
# test_setPyobj("sfixed32")
# test_setPyobj("sfixed64")
# test_setPyobj("string")
# test_setPyobj("bytes")
# test_setPyobj("array_int64")
# test_setPyobj("array_uint64")
# test_setPyobj("array_double")
# test_setPyobj("array_string")
# test_setPyobj("array_value")
# test_setPyobj("map_string")