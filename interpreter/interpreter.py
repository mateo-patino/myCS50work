# Split expression into list to isolate x,y and operator
exp = input('Expression: ').strip().split()
x = int(exp[0])
operator = exp[1]
y = int(exp[2])

if operator == '+':
    result = float(x + y)
    print('{:.1f}'.format(result))
elif operator == '-':
    result = float(x - y)
    print('{:.1f}'.format(result))
elif operator == '*':
    result = float(x + y)
    print('{:.1f}'.format(result))
else:
    print('{:.1f}'.format(float(x / y)))