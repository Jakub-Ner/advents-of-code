from typing import Callable

import numpy as np

HEIGHT = 50
WIDTH = HEIGHT
EMPTY_FIELD = '.'


class Point:
  def __init__(self, x, y):
    self.x = x
    self.y = y

  def __hash__(self):
    return hash((self.x, self.y))

  def __eq__(self, other):
    return self.x == other.x and self.y == other.y

  def __repr__(self):
    return f'Point({self.x}, {self.y})'

  def __sub__(self, other):
    return Point(self.x - other.x, self.y - other.y)

  def __add__(self, other):
    return Point(self.x + other.x, self.y + other.y)


def load_data():
  board = np.genfromtxt('input.txt', delimiter=1, dtype=str, comments=None)  # type: ignore
  print('Board Shape:', board.shape)

  values = np.unique(board)
  values = values[values != EMPTY_FIELD]

  antindes = {}
  for value in values:
    antindes[value] = []
    for col, row in np.argwhere(board == value):
      antindes[value].append(Point(row, col))

  return board, antindes


def single_reflection(p_1: Point, p_2: Point) -> list[Point]:
  antinode = p_1 + p_1 - p_2

  if antinode.x < 0 or antinode.y < 0 or antinode.x >= WIDTH or antinode.y >= HEIGHT:
    return []
  return [antinode]


def find_antinodes(
  antenas: dict[str, list[Point]], strategy: Callable[[Point, Point], list[Point]]
) -> set[Point]:
  antinodes: set[Point] = set()
  for antena_name, points in antenas.items():  # type: ignore
    for point in points:
      for other_point in points:
        if point == other_point:
          continue

        antinode = strategy(point, other_point)
        if len(antinode) > 0:
          antinodes |= set(antinode)
  return antinodes


def run_task_1():
  antinodes = find_antinodes(antenas, single_reflection)
  print('Task 1 - Num of unique antinodes:', len(antinodes))


def propageted_reflection(p_1: Point, p_2: Point) -> list[Point]:
  antinodes = [p_1]

  while new_antinode := single_reflection(p_1, p_2):
    antinodes.extend(new_antinode)
    p_2 = p_1
    p_1 = new_antinode[0]

  return antinodes


def draw_board(board, antinodes):
  board = board.copy()
  for antinode in antinodes:
    board[antinode.y, antinode.x] = '#'
  print(board)


def run_task_2():
  antinodes = find_antinodes(antenas, propageted_reflection)
  print('\nTask 2 - Num of unique antinodes:', len(antinodes))
  # draw_board(board, antinodes)


board, antenas = load_data()

# run_task_1()
run_task_2()
