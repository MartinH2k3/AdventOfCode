import re
with open("cmake-build-debug/input.txt", "r") as f:
 input = f.read()
 regex = input.split("\n\n")[0]
 rows = input.split("\n\n")[1].split("\n")

words = regex.split(", ")
regex = "(" + "|".join(words) + ")+"
matching = 0
for row in rows:
    if re.fullmatch(regex, row):
        matching += 1
        print("yes")
    else:
        print("no")

print(matching)