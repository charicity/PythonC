k = 0
print()
for i in range(1, 1000, 1):
    # k = i # filler
    # print(i)
    k = pythonc.get_Pyobj("val")
    k = k + i
    pythonc.set_Pyobj(k, "val")
print("result: ", k)