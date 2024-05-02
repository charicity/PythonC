# # -*- coding:UTF-8 -*-
if 'sys' not in globals():
    print('sys no')

if 'pythonc' not in globals():
    print('pythonc no')

if '__MatchRuleReq__' not in globals():
    print('__MatchRuleReq__ no')

if '__ReqInfo__' not in globals():
    print('__ReqInfo__ no')

def test_getPyobj(strs):
    print('-------------')
    obj = pythonc.get_Pyobj(strs)
    # obj = 1
    
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