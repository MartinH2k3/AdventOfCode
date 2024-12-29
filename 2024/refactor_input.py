def refactor(item) -> str:
    output: str = ""
    if item[:5] == "#####":
        output += "L "
    else:
        output += "K "

    item = item.split("\n")
    for column in range(5):
        temp = 0
        for row in range(7):
            if item[row][column] == "#":
                temp += 1
        output += str(temp) + " "
    return output


with open("og_input.txt", "r") as f:
    items = f.read().split("\n\n")

items = [refactor(item) for item in items]

items = "\n".join(items)

with open("input.txt", "w") as f:
    f.write(items)
