answer = input('What is the Answer to the Great Question of Life, the Universe and Everything? ')
treated_ans=. answer.lower().strip()
if answer == '42' or treated_ans == 'forty-two' or treated_ans == 'forty two':
    print('Yes')
else:
    print('No')
