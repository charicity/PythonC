# # -*- coding:UTF-8 -*-
import os
import sys
import numpy
sys.path.append('.')
import data_pb2 as pb
import mymodule

if 'k' not in locals() and 'k' not in globals():
    k = 10

if __name__ == "__main__":
    print('Running script 3')

    k += 1
    print('kk=', k)
    