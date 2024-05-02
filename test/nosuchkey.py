k = pythonc.get_Pyobj("notexist")
print(type(k)) # should be none

pythonc.set_Pyobj(29223372036854775808, "toooolong")