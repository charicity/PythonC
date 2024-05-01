print("in script 4")

k = pythonc.get_Pyobj("1")
k += 1
pythonc.set_Pyobj(k, "1")

k = pythonc.get_Pyobj("2")
k += 1
pythonc.set_Pyobj(k, "2")

print("make map['1']++")
print("make map['2']++")