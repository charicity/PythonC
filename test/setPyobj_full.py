# # -*- coding:UTF-8 -*-
if 'sys' not in globals():
    print('sys no')

if 'pythonc' not in globals():
    print('pythonc no')

if '__MatchRuleReq__' not in globals():
    print('__MatchRuleReq__ no')

def test_set_then_get(val, strs):
    print('in call')
    pythonc.set_Pyobj(val,strs)
    obj = pythonc.get_Pyobj(strs)
    
    # print('ref count=',sys.getrefcount(obj))



print('full test of setPyobj')
test_set_then_get(True, "bool")
test_set_then_get(False, "bool")
test_set_then_get(-2147483648, "long")
test_set_then_get(2147483647, "long")
test_set_then_get(-9223372036854775808, "long")
test_set_then_get(9223372036854775807, "long")
test_set_then_get(9223372036854775808, "long")
# test_set_then_get(29223372036854775808, "long") # overflow
strs="asdf"
test_set_then_get(strs.encode(), "bytes")
test_set_then_get(strs, "string")
test_set_then_get(0.0, "float")
test_set_then_get(0.1, "float")
test_set_then_get(-0.1, "float")
test_set_then_get(114.514, "float")
test_set_then_get(-114.514, "float")
lis = [1,2,3,4]
test_set_then_get(lis, "list")
lis = [1,[2.1,2.2],3,4]
test_set_then_get(lis, "list")
dic = {'a+b': '30', 'a*b': '11', 'list': lis}
test_set_then_get(dic, "dict")
tup = (1,2,3,4,5)
test_set_then_get(tup, "tuple")