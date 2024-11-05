from cs50 import get_int

# Use check to see if the requirements for height have been satisfied
check = False
height = 0

while (check == False):
    height = get_int("Height: ")
    if (height >= 1 and height <= 8 and isinstance(height, int)):
        check = True
for i in range(height + 1):
    for j in range(height - i):
        # If we're on the first row of spaces, don't print
        if (i != 0):
            print(" ", end="")
    print("#" * i)