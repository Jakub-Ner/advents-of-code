import numpy as np

HEIGHT = 50
WIDTH = HEIGHT
EMPTY_FIELD = "."


class Point:
  def __init__(self, x, y):
    self.x = x
    self.y = y

  def __hash__(self):
    return hash((self.x, self.y))

  def __eq__(self, other):
    return self.x == other.x and self.y == other.y

  def __repr__(self):
    return f"Point({self.x}, {self.y})"

  def __sub__(self, other):
    return Point(self.x - other.x, self.y - other.y)

  def __add__(self, other):
    return Point(self.x + other.x, self.y + other.y)

def load_data():
  board = np.genfromtxt("input.txt", delimiter=1, dtype=str, comments=None)  # noqa
  print("Board Shape:", board.shape)

  values = np.unique(board)
  values = values[values != EMPTY_FIELD]

  antindes = {}
  for value in values:
    antindes[value] = []
    for col, row in np.argwhere(board == value):
      antindes[value].append(Point(row, col))

  return board, antindes


def find_antinode(p_1: Point, p_2: Point) -> Point|None:
  antinode = p_1 +p_1 - p_2

  if antinode.x < 0 or antinode.y < 0 or antinode.x >= WIDTH or antinode.y >= HEIGHT:
    return None
  return antinode

board, antenas = load_data()

antinodes: set[Point] = set() 
for antena_name, points in antenas.items():
  for point in points:
    for other_point in points:
      if point != other_point:
        antinode = find_antinode( point, other_point)
        if antinode:
          antinodes.add(antinode)

print("Task 1 - Num of unique antinodes:", len(antinodes))


