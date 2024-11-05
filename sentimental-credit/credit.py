from cs50 import get_int

cards = ['AMEX', 'MASTERCARD', 'VISA']
# If CC number is valid, function will return True, else false. Takes in integer.


def check(num):
    str_int = str(num)
    length = len(str_int)

    # Check length, then first and second digits
    if length == 15 and str_int[0] == '3':
        if str_int[1] == '4' or str_int[1] == '7':
            return "AMEX"
    elif length == 16 or length == 13:
        if str_int[0] == '5' and (str_int[1] >= '1' and str_int[1] <= '5'):
            return "MASTERCARD"
        elif str_int[0] == '4':
            return "VISA"
    else:
        return 'N'

# Checksum function. Returns string name if CC valid or 1 if CC is invalid


def luhn(num):
    str_int = str(num)
    length = len(str_int)
    first = list()

    # First argument says we'll begin our loop at the second to last digit. # ZERO IS NOT INLCUDED
    for i in range(length - 2, -1, -2):
        # Convert to int, then into a string again
        first.append(str(2 * int(str_int[i])))

    # Learning note: for i in list loop iterates through all the elements in list, not through the numbers representing the indexes; for that
    # I'd have to use for i in range(n). Recall range returns a list with ints from 0 to n, so with that loop we're pretty much also iterating through
    # the elements themselves.
    second = list()
    for num in first:
        for d in num:
            # the second list will store all the digits of the products obtained in first
            second.append(d)
    sum1 = 0
    for num in second:
        sum1 = sum1 + int(num)
    sum2 = 0
    for i in range(length - 1, -1, -2):
        sum2 = sum2 + int(str_int[i])

    if (sum1 + sum2) % 10 == 0:
        return 0
    else:
        return 1


# Check integer only numbers
c = False
number = 0
while c == False:
    number = get_int("Number: ")
    if isinstance(number, int) == True:
        c = True

# Check general structure with check(number) (beginning digits + length)

name = check(number)
if name in cards:
    result = luhn(number)
    if result == 0:
        print(name)
    else:
        print("INVALID")
else:
    print("INVALID")