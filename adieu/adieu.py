import inflect

inf = inflect.engine()
names = list()

while True:
    try:
        name = input("")
        names.append(name)
    except EOFError:
        break

print("Adieu, adieu, to", inf.join(names))
