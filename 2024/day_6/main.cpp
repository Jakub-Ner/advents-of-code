#include <fstream>
#include <iostream>

using namespace std;

const int WIDTH = 130;
const int HEIGHT = 130;
const char GUARD_ICON_N = '^';
const char GUARD_ICON_E = '>';
const char GUARD_ICON_S = 'v';
const char GUARD_ICON_W = '<';
const char OBSTACLE_ICON = '#';
const char VISITED_ICON = 'X';
const char UNVISITED_ICON = '.';

void read_file(char **matrix, string name) {
  ifstream file;
  file.open(name);
  if (!file.is_open()) {
    cout << "Error opening file" << endl;
    throw "Error opening file";
  }
  for (int i = 0; i < HEIGHT; i++) {
    matrix[i] = new char[WIDTH];
    for (int j = 0; j < WIDTH; j++) {
      file >> matrix[i][j];
    }
  }
}

void clear_memory(char **matrix) {
  for (int i = 0; i < HEIGHT; i++) {
    delete[] matrix[i];
  }
  delete[] matrix;
}

struct Position {
  int y;
  int x;

  Position &operator=(const Position &other) {
    if (this == &other) {
      return *this;
    }
    y = other.y;
    x = other.x;
    return *this;
  }
};
struct Pawn {
  Position position;
  enum Direction { N = 0, E, S, W } direction;
};

void draw_board(char **matrix, Pawn &pawn) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (i == pawn.position.y && j == pawn.position.x) {
        switch (pawn.direction) {
        case Pawn::N:
          cout << GUARD_ICON_N << " ";
          break;
        case Pawn::E:
          cout << GUARD_ICON_E << " ";
          break;
        case Pawn::S:
          cout << GUARD_ICON_S << " ";
          break;
        case Pawn::W:
          cout << GUARD_ICON_W << " ";
          break;
        default:
          throw "Invalid direction";
        }
      } else {
        cout << matrix[i][j] << " ";
      }
    }
    cout << endl;
  }
  cout << endl << endl;
}

void rotate(Pawn &pawn) {
  pawn.direction = static_cast<Pawn::Direction>((pawn.direction + 1) % 4);
}

Position *find_next_position(Pawn &pawn) {
  switch (pawn.direction) {
  case Pawn::N:
    return new Position{.y = pawn.position.y - 1, .x = pawn.position.x};
  case Pawn::E:
    return new Position{.y = pawn.position.y, .x = pawn.position.x + 1};
  case Pawn::S:
    return new Position{.y = pawn.position.y + 1, .x = pawn.position.x};
  case Pawn::W:
    return new Position{.y = pawn.position.y, .x = pawn.position.x - 1};
  default:
    throw "Invalid direction";
  }
}

bool is_obstacle(char **matrix, Position &position) {
  return matrix[position.y][position.x] == OBSTACLE_ICON;
}
bool is_out_of_bounds(Position &position) {
  return position.y < 0 || position.y >= HEIGHT || position.x < 0 ||
         position.x >= WIDTH;
}

enum Action { STOP, CONTINUE };

Action move(Pawn &pawn, char **matrix) {
  Position *nextPosition = find_next_position(pawn);
  Action action = CONTINUE;
  if (is_out_of_bounds(*nextPosition)) {
    action = STOP;
  } else if (is_obstacle(matrix, *nextPosition)) {
    rotate(pawn);
  } else {
    pawn.position = *nextPosition;
  }
  // draw_board(matrix, pawn);
  delete nextPosition;
  return action;
}

Pawn *find_pawn(char **matrix) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      switch (matrix[i][j]) {
      case GUARD_ICON_N:
        return new Pawn{.position{.y = i, .x = j}, .direction = Pawn::N};
      case GUARD_ICON_E:
        return new Pawn{.position{.y = i, .x = j}, .direction = Pawn::E};
      case GUARD_ICON_S:
        return new Pawn{.position{.y = i, .x = j}, .direction = Pawn::S};
      case GUARD_ICON_W:
        return new Pawn{.position{.y = i, .x = j}, .direction = Pawn::W};
      default:
        break;
      }
    }
  }
  throw "Guard not found";
}
bool mark_guard_position(char **matrix, Pawn &pawn) {
  bool wasPositionUnvisited = false;
  if (matrix[pawn.position.y][pawn.position.x] != VISITED_ICON) {
    matrix[pawn.position.y][pawn.position.x] = VISITED_ICON;
    wasPositionUnvisited = true;
  }
  return wasPositionUnvisited;
}

void run_task_1(char **matrix) {
  cout << "Task 1" << endl;

  Pawn *pawn = find_pawn(matrix);
  mark_guard_position(matrix, *pawn);

  int distincPlacesNum = 1;
  const int INITIAL_COUNTDOWN = WIDTH * HEIGHT;
  int countDown = INITIAL_COUNTDOWN;
  while (countDown > 0) {
    Action action = move(*pawn, matrix);
    if (action == STOP)
      break;

    if (mark_guard_position(matrix, *pawn)) {
      distincPlacesNum++;
      countDown = INITIAL_COUNTDOWN;
    } else {
      countDown--;
    }
  }

  delete pawn;
  cout << "Number of distint positions guard visited: " << distincPlacesNum
       << endl << endl;
}

int main() {
  char **matrix = new char *[HEIGHT];
  read_file(matrix, "./input.txt");
  run_task_1(matrix);
  clear_memory(matrix);
  return 0;
}
