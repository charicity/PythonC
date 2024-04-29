# # -*- coding:UTF-8 -*-
import os
import sys
import numpy
sys.path.append('.')
import data_pb2 as pb
import mymodule


if __name__ == "__main__":
    # 获取共享变量的值
    value = mymodule.get_shared_variable()
    print("Python: Shared variable =", value)

    # 修改共享变量的值
    mymodule.set_shared_variable(20)

    # 再次获取共享变量的值，确认是否修改成功
    value = mymodule.get_shared_variable()
    print("Python: Modified shared variable =", value)
