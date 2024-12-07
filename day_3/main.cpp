#include <csignal>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <unordered_map>

using namespace std;

const int HEIGHT = 140;
const int WIDTH = 140;

const unordered_map<char, int> encoding_map = {
    {'x', 2}, {'m', 3}, {'a', 5}, {'s', 7},

    {'X', 2}, {'M', 3}, {'A', 5}, {'S', 7},
};

const int TEXT_SIZE = encoding_map.size() / 2;

int **create_encoding_from_file(const char *filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    cout << "Unable to open file" << endl;
    return nullptr;
  }

  int **encoding = new int *[HEIGHT];
  for (int i = 0; i < HEIGHT; i++) {
    encoding[i] = new int[WIDTH];
    for (int j = 0; j < WIDTH; j++) {
      char c;
      file >> c;
      try {
        encoding[i][j] = encoding_map.at(c);
      } catch (const out_of_range &e) {
        cout << "Error: Invalid character in file" << endl;
        raise(SIGINT);
      }
    }
  }
  file.close();
  return encoding;
}

void print_arr(int **arr) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      cout << arr[i][j] << " ";
    }
    cout << endl;
  }
}
bool validate_order(array<int, 4> order) {
  if (order.at(0) == order.begin()[1])
    return false;

  int sign = order.begin()[0] > order.begin()[1] ? 1 : -1;
  for (int i = 1; i < order.size() - 1; i++)
    if ((order.begin()[i] - order.begin()[i + 1]) * sign <= 0) {
      return false;
    }
  return true;
}

class Updater {
public:
  virtual tuple<int, int> shift(int row, int col) = 0;
  virtual tuple<int, int> updateAfterEpoch(int tmpRow, int tmpCol) = 0;
  virtual bool ShouldContinue(int row, int col) = 0;
  virtual array<int, 4> get_elements(int **encoding, int i, int j) const = 0;
};

array<int, 4> left_diagonal_elements(int **encoding, int i, int j) {
  return {encoding[i][j], encoding[i + 1][j + 1], encoding[i + 2][j + 2],
          encoding[i + 3][j + 3]};
}

array<int, 4> right_diagonal_elements(int **encoding, int i, int j) {
  return {encoding[i][j], encoding[i + 1][j - 1], encoding[i + 2][j - 2],
          encoding[i + 3][j - 3]};
}

class LeftDiagonalShiftRight : public Updater {
public:
  tuple<int, int> shift(int row, int col) { return make_tuple(row, col); }

  tuple<int, int> updateAfterEpoch(int tmpRow, int tmpCol) {
    return make_tuple(0, tmpCol + 1);
  }

  bool ShouldContinue(int row, int col) { return col + 3 < WIDTH; }

  array<int, 4> get_elements(int **encoding, int i, int j) const {
    return left_diagonal_elements(encoding, i, j);
  }
};

class RightDiagonalShiftLeft : public Updater {
public:
  tuple<int, int> shift(int row, int col) { return make_tuple(row, -col); }

  tuple<int, int> updateAfterEpoch(int tmpRow, int tmpCol) {
    return make_tuple(0, tmpCol - 1);
  }

  bool ShouldContinue(int row, int col) { return col - 3 >= 0; }

  array<int, 4> get_elements(int **encoding, int i, int j) const {
    return right_diagonal_elements(encoding, i, j);
  }
};

class LeftDiagonalShiftDown : public Updater {
public:
  tuple<int, int> shift(int row, int col) { return make_tuple(row, col); }

  tuple<int, int> updateAfterEpoch(int tmpRow, int tmpCol) {
    return make_tuple(tmpRow + 1, 0);
  }

  bool ShouldContinue(int row, int col) { return row + 3 < HEIGHT; }

  array<int, 4> get_elements(int **encoding, int i, int j) const {
    return left_diagonal_elements(encoding, i, j);
  }
};

class RightDiagonalShiftDown : public Updater {
public:
  tuple<int, int> shift(int row, int col) { return make_tuple(row, -col); }

  tuple<int, int> updateAfterEpoch(int tmpRow, int tmpCol) {
    return make_tuple(tmpRow + 1, WIDTH - 1);
  }

  bool ShouldContinue(int row, int col) { return row + 3 < HEIGHT; }

  array<int, 4> get_elements(int **encoding, int i, int j) const {
    return right_diagonal_elements(encoding, i, j);
  }
};

int search_diagonal_left_helper(int **encoding, Updater *updater, int i,
                                int j) {
  int counter = 0;
  while (updater->ShouldContinue(i, j)) {
    int tmpRow = i;
    int tmpCol = j;
    while (updater->ShouldContinue(i, j)) {
      if (validate_order(updater->get_elements(encoding, i, j))) {
        counter++;
        auto [rowShift, colShift] = updater->shift(TEXT_SIZE - 1, TEXT_SIZE - 1);
        i += rowShift;
        j += colShift;
      } else {
        auto [rowShift, colShift] = updater->shift(1, 1);
        i += rowShift;
        j += colShift;
      }
    }
    tie(i, j) = updater->updateAfterEpoch(tmpRow, tmpCol);
  }
  return counter;
}

int search_right_diagonal(int **encoding) {
  Updater *updater = new RightDiagonalShiftLeft();
  int counter = search_diagonal_left_helper(encoding, updater, 0, WIDTH - 1);

  updater = new RightDiagonalShiftDown();
  counter += search_diagonal_left_helper(encoding, updater, 1, WIDTH - 1);
  cout << "Diagonal right: " << counter << endl;
  return counter;
}

int search_left_diagonal(int **encoding) {
  Updater *updater = new LeftDiagonalShiftRight();
  int counter = search_diagonal_left_helper(encoding, updater, 0, 0);

  updater = new LeftDiagonalShiftDown();
  counter += search_diagonal_left_helper(encoding, updater, 1, 0);
  cout << "Diagonal left: " << counter << endl;
  return counter;
}

int search_vertical(int **encoding) {
  int counter = 0;
  for (int i = 0; i < HEIGHT; i++) {
    int j = 0;
    while (j + 3 < WIDTH) {
      if (validate_order(array<int, 4>{encoding[i][j], encoding[i][j + 1],
                                       encoding[i][j + 2],
                                       encoding[i][j + 3]})) {
        counter++;
        j += TEXT_SIZE - 1;
      } else {
        j++;
      }
    }
  }
  cout << "Vertical: " << counter << endl;
  return counter;
}

int search_horizontal(int **encoding) {
  int counter = 0;
  for (int j = 0; j < WIDTH; j++) {
    int i = 0;
    while (i + 3 < HEIGHT) {
      if (validate_order(array<int, 4>{encoding[i][j], encoding[i + 1][j],
                                       encoding[i + 2][j],
                                       encoding[i + 3][j]})) {
        counter++;
        i += TEXT_SIZE - 1;
      } else {
        i++;
      }
    }
  }
  cout << "Horizontal: " << counter << endl;
  return counter;
}

int search_diagonal(int **encoding) {
  int counter = search_left_diagonal(encoding);
  counter += search_right_diagonal(encoding);
  return counter;
}

void delete_encoding(int **encoding) {
  for (int i = 0; i < HEIGHT; i++) {
    delete[] encoding[i];
  }
  delete[] encoding;
}

void run_task_1(int **encoding) {
  cout << "Task 1" << endl;
  int requiredValue = encoding_map.at('x') * encoding_map.at('m') *
                      encoding_map.at('a') * encoding_map.at('s');

  int numOfOccurences = search_vertical(encoding);
  numOfOccurences += search_horizontal(encoding);
  numOfOccurences += search_diagonal(encoding);

  cout << "Number of occurences: " << numOfOccurences << endl;
}

int main() {
  int **encoding = create_encoding_from_file("./input.txt");
  run_task_1(encoding);

  delete_encoding(encoding);
  return 0;
}
