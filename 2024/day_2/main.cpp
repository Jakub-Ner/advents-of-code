#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

const int N_SAMPLES = 1000;
const int MIN_DISTANCE = 1;
const int MAX_DISTANCE = 3;

void clean_memory(int **samples) {
  for (int i = 0; i < N_SAMPLES; i++) {
    delete[] samples[i];
  }
  delete samples;
}

void print_line(int *row, int n) {
  for (int i = 0; i < n; i++)
    cout << row[i] << " ";
  cout << endl;
}

int *read_row(ifstream &file) {
  const int MAX = 10;
  int tmpRow[MAX];
  int counter = 0;
  std::string line;
  getline(file, line);

  std::istringstream iss(line);
  while (iss >> tmpRow[counter]) {
    counter++;
    assert(counter < MAX && "Error: Increase MAX");
  }
  int *row = new int[counter + 1];
  row[0] = counter;
  memcpy(&row[1], tmpRow, counter * sizeof(int));
  return row;
}

void read_file(int **raports) {
  ifstream file("./input.txt");
  if (!file.is_open()) {
    cout << "Unable to open file" << endl;
    return;
  }

  for (int i = 0; i < N_SAMPLES; i++) {
    raports[i] = read_row(file);
  }
  file.close();
}

int get_sign(int prev, int current) {
  if (prev > current) {
    return -1;
  } else if (prev < current) {
    return 1;
  }
  return 0;
}

bool validate_distance(int prev, int current, int sign) {
  int distance = (current - prev) * sign;
  if (distance >= MIN_DISTANCE && distance <= MAX_DISTANCE) {
    return true;
  }
  return false;
}

tuple<bool, int> check_conditions(int *row) {
  // false is accompanied by index of prev
  int prev = row[1];
  int current = row[2];
  int sign = get_sign(prev, current);
  if (sign == 0) {
    return make_tuple(false, 1);
  }

  for (int i = 2; i <= row[0]; i++) {
    current = row[i];
    if (!validate_distance(prev, current, sign)) {
      return make_tuple(false, i - 1);
    }
    prev = current;
  }
  return make_tuple(true, row[0]);
}
void rm_value(int *row, int index, int *out) {
  memcpy(&out[0], &row[0], (index + 1) * sizeof(int));
  out[0]--;
  if (index >= row[0]) {
    return;
  }
  memcpy(&out[index], &row[index + 1], (row[0] - index) * sizeof(int));
}

int *drop_from(int *row, int index) {
  int *newRow = new int[row[0]];
  rm_value(row, index, newRow);
  return newRow;
}

void run_task_2(int **raports) {
  int safeCount = 0;
  int *raport;
  cout << "Task 2" << endl;
  for (int i = 0; i < N_SAMPLES; i++) {
    raport = raports[i];
    bool deleteFlag = false;

    for (int j = 1; j <= raports[i][0] + 1; j++) {
      auto [isSafe, index] = check_conditions(raport);

      //cout << j - 1 << ": Trying for: ";
      //print_line(raport, raport[0] + 1);
      //cout << "With success=" << isSafe << endl << endl;
      if (deleteFlag) {
        //cout << "Deleting" << endl;
        delete raport;
        deleteFlag = false;
      }
      if (isSafe) {
        safeCount++;
        break;
      }
      if (j == raports[i][0] + 1) {
        break;
      }
      raport = drop_from(raports[i], j);
      deleteFlag = true;
    }
  }
  cout << "Safe count: " << safeCount << endl;
}

void run_task_1(int **raports) {
  int safeCount = 0;

  cout << "Task 1" << endl;
  for (int i = 0; i < N_SAMPLES; i++) {
    auto [isSafe, index] = check_conditions(raports[i]);
    if (isSafe) {
      safeCount++;
    }
  }
  cout << "Safe count: " << safeCount << endl << endl;
}

int main() {
  int **raports = new int *[N_SAMPLES];

  read_file(raports);
  run_task_1(raports);
  run_task_2(raports);
  clean_memory(raports);
  return 0;
}
