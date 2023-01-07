# TODO
import cs50 as cs

while True:
    input = cs.get_int("Height: ")
    if input >= 1 and input <= 8:
        break

for i in range(input):
    z = input
    while z > i+1:
        print(" ", end="")
        z -= 1
    for i in range(z):
        print("#", end="")
    print("  ", end="")
    for i in range(z):
        print("#", end="")
    print("")
