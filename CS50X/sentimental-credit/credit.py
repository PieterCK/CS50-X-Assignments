# TODO
import re
import cs50 as cs


try:
    input = cs.get_int("Number: ")
except ValueError:
    print("Enter numbers only!")
    exit()

input2 = input
list = []
while input > 0:
    list.insert(0, input % 10)
    input = (input // 10)

if len(list) > 16 or len(list) < 13:
    print("INVALID")
    exit()

underlined = 0
not_u = 0
i = len(list)-1
bool = False
while i >= 0:
    if bool == True:
        if list[i]*2 > 9:
            underlined = underlined + (list[i] * 2) // 10
            underlined = underlined + (list[i] * 2) % 10
            i -= 1
            bool = False
        else:
            underlined = underlined + list[i] * 2
            i -= 1
            bool = False

    elif bool == False:
        not_u = not_u + list[i]
        i -= 1
        bool = True

final = underlined + not_u
if final % 10 != 0:
    print("INVALID")
    exit()
else:
    if re.match("4", str(input2)):
        print("VISA")
    elif re.match('[5][0-5]', str(input2)):
        print("MASTERCARD")
    elif re.match("34", str(input2)) or re.match("37", str(input2)):
        print("AMEX")
    else:
        print("INVALID")
