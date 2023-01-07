import re
import cs50 as cs

input = cs.get_string("Text: ")

list = []
list = re.split(r"\s+", input)
words = len(list)

list_sntnc = []
list_sntnc = re.split('[.!?]', input)
sentences = len(list_sntnc)
sentences -= 1


letters = 0
for i in range(len(list)):
    if list[i].isalpha():
        letters = letters + len(list[i])
    else:
        letters = letters + len(list[i])
        letters -= 1

L = letters / words * 100
S = sentences / words * 100
Luhn = round(0.0588 * L - 0.296 * S - 15.8)

if Luhn > 16:
    print("Grade 16+")
elif Luhn < 1:
    print("Before Grade 1")
else:
    print("Grade " + str(Luhn))
