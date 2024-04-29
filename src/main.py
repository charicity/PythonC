# # -*- coding:UTF-8 -*-
import os
import sys
import numpy
sys.path.append('.')
import data_pb2 as pb
import mymodule
import binascii

if __name__ == "__main__":
    val = mymodule.get_shared_variable("useless")
    # print('exit safely')
    print('type of val=',type(val))
    # print(val)

    # byte_data = val.encode()
    # print('type of byte_data=',type(byte_data))

    message = pb.context_value()
    # with open("output.txt", "rb") as f:
    #     message.ParseFromString(f.read())

    print('len=',len(val))

    message.ParseFromString(val)

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
