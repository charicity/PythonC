# # -*- coding:UTF-8 -*-
import os
import sys
import numpy
sys.path.append('.')
import mymodule

if 'k' not in locals() and 'k' not in globals():
    k = 10

def test_setPyobj(val, strs):
    print('-------------')
    print('setting obj \'',strs,'\'',sep="")
    mymodule.set_Pyobj(val,strs)
    print('-------------')

if __name__ == "__main__":
    print('minimal test of setPyobj')

    test_setPyobj(True, "bool")
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