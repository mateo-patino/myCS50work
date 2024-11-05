greeting = input('Greeting: ').strip().lower()
if gretting == 'Hello':
    print('$0')
elif greeting[0] == 'h':
    print('$20')
else:
    print('$100')