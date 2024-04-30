# # -*- coding:UTF-8 -*-
import os
import sys
import numpy
sys.path.append('.')
import data_pb2
import mymodule

if 'k' not in locals() and 'k' not in globals():
    k = 10

if __name__ == "__main__":
    print('Running script 2')

    # val = data_pb2.context_value()
    # fetch = mymodule.get_protobuf("array_int64")
    # val.ParseFromString(fetch)
    # print(val) # 一个context_value类型的数据
    # print('ref count=',sys.getrefcount(val))

    # val.array_int64.data[:] = [1,2,3]

    # mymodule.set_protobuf("array_int64", val.SerializeToString())

    # if val.HasField('bool'):
    #     print("Data type: bool")
    # elif val.HasField('float'):
    #     print("Data type: float")
    # elif val.HasField('double'):
    #     print("Data type: double")
    # elif val.HasField('string'):
    #     print('Data type: string')
    # elif val.HasField('uint32'):
    #     print('Data type: uint32')
    # else:
    #     print('Data type: other')

    # obj = mymodule.get_pyobj("bool")
    # print(obj)
    # print('ref count=',sys.getrefcount(obj))
    # print('type=',type(obj))

    # obj = mymodule.get_pyobj("int32")
    # print(obj)
    # print('ref count=',sys.getrefcount(obj))
    # print('type=',type(obj))

    # obj = mymodule.get_pyobj("string")
    # print(obj)
    # print('ref count=',sys.getrefcount(obj))
    # print('type=',type(obj))

    # obj = mymodule.get_pyobj("array_int64")
    # print(obj)
    # print('ref count=',sys.getrefcount(obj))
    # print('type=',type(obj))

    # obj = mymodule.get_pyobj("array_uint64")
    # print(obj)
    # print('ref count=',sys.getrefcount(obj))
    # print('type=',type(obj))


    # obj = mymodule.get_pyobj("array_double")
    # print(obj)
    # print('ref count=',sys.getrefcount(obj))
    # print('type=',type(obj))

    # obj = mymodule.get_pyobj("array_string")
    # print(obj)
    # print('ref count=',sys.getrefcount(obj))
    # print('type=',type(obj))


    # obj = mymodule.get_pyobj("array_value")
    # print(obj)
    # print('ref count=',sys.getrefcount(obj))
    # print('type=',type(obj))

    obj = mymodule.get_pyobj("map_string")
    print(obj)
    print('ref count=',sys.getrefcount(obj))
    print('type=',type(obj))