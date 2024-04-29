print('cleaning')
for key in globals().copy(): 
    if not key.startswith("__"):
        globals().pop(key)