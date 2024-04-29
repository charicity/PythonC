# # -*- coding:UTF-8 -*-
import os
import sys
import numpy
sys.path.append('.')
import data_pb2 as pb
import adds as ads


if __name__ == "__main__":
    a=1
    b=2
    c=ads.adds(a,b)
    print('c=',c)
    # print(sys.version)
    # print(os.getcwd())

    # # 创建一个 Message 对象
    # message = pb.ArrayInt64()

    # # 使用 ParseFromString 方法从字符串中解析 Protobuf 数据
    # with open("output.txt", "rb") as f:
    #     message.ParseFromString(f.read())
    
    # for data in message.data:
    #     print(data)
