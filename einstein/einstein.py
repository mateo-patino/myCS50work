def energy(mass):
    return str(mass * pow(300000000, 2))

m = int(input('Mass: '))
print('E: ' + energy(m))