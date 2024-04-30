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

    val = data_pb2.context_value()
    fetch = mymodule.get_shared_variable("key1")
    val.ParseFromString(fetch)
    print(val) # 一个context_value类型的数据

    val.array_int64.data[:] = [1,2,3]
    print(val)

    mymodule.set_shared_variable("key1", val.SerializeToString())

    if val.HasField('bool'):
        print("Data type: bool")
    elif val.HasField('float'):
        print("Data type: float")
    elif val.HasField('double'):
        print("Data type: double")
    elif val.HasField('string'):
        print('Data type: string')
    elif val.HasField('uint32'):
        print('Data type: uint32')
    else:
        print('Data type: other')
    