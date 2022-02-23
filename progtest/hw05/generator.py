import os

from random import choice
from string import ascii_lowercase, ascii_uppercase

for i in range(20):
    f = open(f"{i}_in.txt", "w")
    f.write("120\n")
    idk = []
    for p in range(i+2):
        idk.append(choice(ascii_uppercase) + "".join(choice(ascii_lowercase) for k in range(5)))
    print(i)
    for p in range(4, i+10):
        f.write("+ " + choice(idk) + "\n")
        if p % 5 == 0:
            f.write("?\n")
        if p % 2 == 0:
            f.write("#\n")
    f.close()
    os.system(f"./a.out < {i}_in.txt > {i}_out.txt")
