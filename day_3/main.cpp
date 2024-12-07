#include <csignal>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <unordered_map>

using namespace std;

const int HEIGHT = 1;
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
bool validate_order(initializer_list<int> order) {
  int sign = order.begin()[0] > order.begin()[1] ? 1 : -1;
  for (int i = 1; i < order.size() - 1; i++)
    if ((order.begin()[i] - order.begin()[i + 1]) * sign < 0) {
      return false;
    }
  return true;
}
int search_vertical(int **encoding, int requiredValue) {
  int counter = 0;
  cout << "Required value: " << requiredValue << endl;
  for (int i = 0; i < HEIGHT; i++) {
    int j = 0;
    int value = 1;
    int usedElements = 0;
    while (j < WIDTH) {
      for (int k = 0; usedElements < TEXT_SIZE; k++) {
        usedElements++;
        value *= encoding[i][j + k];
      }
      cout << "Value: " << value << " For: " << encoding[i][j] << " "
           << encoding[i][j + 1] << " " << encoding[i][j + 2] << " "
           << encoding[i][j + 3] << endl;

      if (validate_order({encoding[i][j], encoding[i][j + 1],
                          encoding[i][j + 2], encoding[i][j + 3]})) {

        // cout<< "Found "<< encoding[i][j] << " " << encoding[i][j + 1] << " "
        // << encoding[i][j + 2] << " " << encoding[i][j + 3] << " val=" <<
        // value << endl;
        counter++;
        j += TEXT_SIZE;
        usedElements = 0;
        value = 1;
      } else {
        value /= encoding[i][j];
        usedElements--;
        j++;
      }
    }
  }
  return counter;
}

int search_horizontal(int **encoding, int requiredValue) {
  int counter = 0;
  return counter;
}

int search_diagonal(int **encoding, int requiredValue) {
  int counter = 0;
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

  int numOfOccurences = search_vertical(encoding, requiredValue);
  numOfOccurences += search_horizontal(encoding, requiredValue);
  numOfOccurences += search_diagonal(encoding, requiredValue);

  cout << "Number of occurences: " << numOfOccurences << endl;
}

int main() {
  int **encoding = create_encoding_from_file("./input.txt");
  run_task_1(encoding);

  delete_encoding(encoding);
  return 0;
}
