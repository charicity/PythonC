print('in script 1')
if 'k' not in locals() and 'k' not in globals():
    k = 0
k += 1
print('k=', k)

shared = pythonc.get_Pyobj("shared")
print('shared MatchRuleReq=',shared)
shared += 1
pythonc.set_Pyobj(shared, "shared")