#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <vector>

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

void print_visited(vector<Position> visitedPositions) {
  cout << "Visited positions: ";
  for (auto position : visitedPositions) {
    cout << "(" << position.y << ", " << position.x << ") ";
  }
  cout << endl;
}

void draw_board(char **matrix, Pawn &pawn) {
  system("clear");
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

enum Action { STOP, GO, ROTATE };

Action move(Pawn &pawn, char **matrix) {
  Position *nextPosition = find_next_position(pawn);
  Action action;
  if (is_out_of_bounds(*nextPosition)) {
    action = STOP;
  } else if (is_obstacle(matrix, *nextPosition)) {
    rotate(pawn);
    action = ROTATE;
  } else {
    pawn.position = *nextPosition;
    action = GO;
  }
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
  throw invalid_argument("Guard not found");
}

bool mark_guard_position(char **matrix, Pawn &pawn) {
  bool wasPositionUnvisited = false;
  if (matrix[pawn.position.y][pawn.position.x] == UNVISITED_ICON) {
    matrix[pawn.position.y][pawn.position.x] = VISITED_ICON;
    wasPositionUnvisited = true;
  }
  return wasPositionUnvisited;
}

void add_obstruction(char **matrix, Position &position) {
  matrix[position.y][position.x] = OBSTACLE_ICON;
}

void remove_obstruction(char **matrix, Position &position) {
  matrix[position.y][position.x] = UNVISITED_ICON;
}

bool find_guard_positions(char **originalMatrix,
                          function<void(const Pawn &)> onNewPositionCallback) {

  char** matrix = new char*[HEIGHT];
  for (int i = 0; i < HEIGHT; i++) {
    matrix[i] = new char[WIDTH];
    strcpy(matrix[i], originalMatrix[i]);
  }
  Pawn *pawn = find_pawn(matrix);
  Position startPosition = pawn->position;
  mark_guard_position(matrix, *pawn);
  onNewPositionCallback(*pawn);

  int distincPlacesNum = 1;
  int countDown = 1; 
  while (countDown > 0) {
    Action action = move(*pawn, matrix);
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // draw_board(matrix, *pawn);
    if (action == STOP)
      break;

    if (mark_guard_position(matrix, *pawn)) {
      distincPlacesNum++;
      countDown++;
      onNewPositionCallback(*pawn);
    } else if (action == GO) {
      countDown--;
    }
  }
  clear_memory(matrix);
  delete pawn;
  return countDown > 0;
}

void run_task_2(char **matrix, vector<Position> visitedPositions) {
  cout << "Task 2" << endl;

  int possibleObstructions = 0;
  for (int i = 1; i < visitedPositions.size(); i++) {
    add_obstruction(matrix, visitedPositions[i]);
    if (!find_guard_positions(matrix, [](const Pawn &pawn) {})) {
      possibleObstructions++;
    }
    remove_obstruction(matrix, visitedPositions[i]);
  }
  cout << "Number of possible obstruction placements: " << possibleObstructions
       << endl
       << endl;
}

vector<Position> run_task_1(char **matrix) {
  cout << "Task 1" << endl;

  vector<Position> visitedPositions;
  find_guard_positions(matrix, [&](const Pawn &pawn) {
    visitedPositions.push_back(pawn.position);
  });

  cout << "Number of distint positions guard visited: "
       << visitedPositions.size() << endl
       << endl;

  return visitedPositions;
}

int main() {
  char **matrix = new char *[HEIGHT];
  read_file(matrix, "./input.txt");

  auto visitedPositions = run_task_1(matrix);
  // print_visited(visitedPositions);

  run_task_2(matrix, visitedPositions);
  clear_memory(matrix);
  return 0;
}
