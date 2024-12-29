class Point:
    def __init__(self, y, x):
        self.y = y
        self.x = x
    def __add__(self, other):
        return Point(self.y + other.y, self.x + other.x)
    def __sub__(self, other):
        return Point(self.y - other.y, self.x - other.x)
    def __str__(self):
        return f"({self.y}, {self.x})"
    def __eq__(self, other):
        return self.y == other.y and self.x == other.x
    def turn_around(self):
        return Point(-self.y, -self.x)


directions = {
    "^": Point(-1, 0),
    "v": Point(1, 0),
    "<": Point(0, -1),
    ">": Point(0, 1)
}

def move_robot(_warehouse: list[list[str]], init_pos: Point, _move: str) -> Point:
    _move = directions[_move]
    target_pos = init_pos + _move
    # get to the closest non-box position
    while _warehouse[target_pos.y][target_pos.x] not in ["#", "."]:
        target_pos += _move
    if _warehouse[target_pos.y][target_pos.x] == "#":
        return init_pos
    back_direction = _move.turn_around()
    while target_pos != init_pos:
        next_pos = target_pos + back_direction
        # swap the two positions
        (_warehouse[target_pos.y][target_pos.x],
         _warehouse[next_pos.y][next_pos.x]) = (
            _warehouse[next_pos.y][next_pos.x],
            _warehouse[target_pos.y][target_pos.x])
        target_pos = next_pos
    return init_pos+_move


def move_robot2(_warehouse: list[list[str]], init_pos: Point, _move: str, ) -> Point:
    if _move in "<>":
        return move_robot(_warehouse, robot_pos, _move)

    y_move = directions[_move].y
    curr_layer = [init_pos]
    y = init_pos.y
    moved_points = [curr_layer]
    while len(curr_layer):
        affected_points = []
        new_y = y + y_move
        for x in range(len(_warehouse[0])):
            if _warehouse[new_y][x] == "[":
                if Point(y, x) in curr_layer or Point(y, x+1) in curr_layer:
                    affected_points.append(Point(new_y, x))
            if _warehouse[new_y][x] == "]":
                if Point(y, x) in curr_layer or Point(y, x-1) in curr_layer:
                    affected_points.append(Point(new_y, x))
            if _warehouse[new_y][x] == "#":
                if Point(y, x) in curr_layer:
                    return init_pos
        moved_points.append(affected_points)
        y = new_y
        curr_layer = affected_points

    for layer in moved_points[::-1]:
        for point in layer:
            #print(f"Moving {point} to {point.y+y_move}, {point.x}. Value: {_warehouse[point.y][point.x]}")
            _warehouse[point.y+y_move][point.x] = _warehouse[point.y][point.x]
            _warehouse[point.y][point.x] = "."

    return init_pos + directions[_move]

with open("bob.txt" , "r") as f:
    input15 = f.read().split("\n\n")

warehouse = input15[0].split("\n")
temp= "".join(warehouse).find("@")
robot_pos = Point(temp // len(warehouse[0]), temp % len(warehouse[0]))
warehouse = [list(i) for i in warehouse]

moves = "".join(input15[1].split("\n"))
# for move in moves:
#     robot_pos = move_robot(warehouse, robot_pos, move)

output: int = 0
for index_i, i in enumerate(warehouse):
    for index_j, j in enumerate(i):
        if warehouse[index_i][index_j] == "O":
            output += 100*index_i + index_j

print("Part 1:", output)

warehouse2 = []
for i, line in enumerate(input15[0].split("\n")):
    row = []
    for j, char in enumerate(line):
        if char == "#":
            row.append("#")
            row.append("#")
        if char == ".":
            row.append(".")
            row.append(".")
        if char == "O":
            row.append("[")
            row.append("]")
        if char == "@":
            robot_pos = Point(i, j*2)
            row.append("@")
            row.append(".")
    warehouse2.append(row)

for move in moves:
    robot_pos = move_robot2(warehouse2, robot_pos, move)

output: int = 0
for index_i, i in enumerate(warehouse2):
    for index_j, j in enumerate(i):
        if warehouse2[index_i][index_j] == "[":
            output += 100*index_i + index_j

print("Part 2:", output)