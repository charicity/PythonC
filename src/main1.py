# # -*- coding:UTF-8 -*-
import os
import sys
import numpy
sys.path.append('.')
import data_pb2 as pb
import mymodule
import binascii


if __name__ == "__main__":
    print('Running script 1')
    
    val = mymodule.get_shared_variable("key1")
    print('type of val=',type(val))
    print('refcount of val=',sys.getrefcount(val))

    message = pb.context_value()
    message.ParseFromString(val)

    print('message=',message)

    if message.HasField('bool'):
        print("Data type: bool")
    elif message.HasField('float'):
        print("Data type: float")
    elif message.HasField('double'):
        print("Data type: double")
    elif message.HasField('string'):
        print('Data type: string')
    elif message.HasField('uint32'):
        print('Data type: uint32')
    else:
        print('Data type: other')
