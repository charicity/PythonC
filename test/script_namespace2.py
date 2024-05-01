print('in script 2')
if 'k' not in locals() and 'k' not in globals():
    k = 1000
k += 1000
print('k=', k)

shared = pythonc.get_Pyobj("shared")
print('shared MatchRuleReq=',shared)
shared += 1
pythonc.set_Pyobj(shared, "shared")