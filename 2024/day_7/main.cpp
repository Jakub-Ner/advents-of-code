#include <array>
#include <cassert>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

const int LINES_NUM = 850;
const int OPERATIONS_NUM = 3;

struct Equation {
  long testValue;
  vector<long> products;
};
using Equations = std::array<Equation, LINES_NUM>;

istream &operator>>(std::istream &is, Equation &eq) {
  is >> eq.testValue;
  long product;
  char c;
  is.get(c);
  while (c != '\n') {
    is >> product;
    eq.products.push_back(product);
    is.get(c);
  }
  return is;
}

Equations read_file(const char *filename) {
  ifstream file;
  file.open(filename);
  if (!file.is_open()) {
    cout << "Error opening file" << endl;
    throw "Error opening file";
  }
  array<Equation, LINES_NUM> equations;
  for (int i = 0; i < LINES_NUM; i++) {
    file >> equations[i];
  }
  return equations;
}

void print_arr(Equations equations) {
  for (int i = 0; i < LINES_NUM; i++) {
    cout << equations[i].testValue << " ";
    for (int j = 0; j < equations[i].products.size(); j++) {
      cout << equations[i].products[j] << " ";
    }
    cout << endl;
  }
}

long concatenate(long a, long b) {
  long temp = b;
  while (temp > 0) {
    a *= 10;
    temp /= 10;
  }
  return a + b;
}
long sum(long a, long b) { return a + b; }
long multiply(long a, long b) { return a * b; }

using Operations = array<function<long(long, long)>, OPERATIONS_NUM>;

bool is_equal(Equation equation, int index, long sum, Operations operations){
  if (index == equation.products.size()) {
    return sum == equation.testValue;
  }
  for (int i = 0; i < OPERATIONS_NUM; i++) {
    if (is_equal(equation, index + 1, operations[i](sum, equation.products[index]), operations)) {
      return true;
    }
  }
  return false;
}

bool validate_result(Equation equation, Operations operations) {
  return is_equal(equation, 1, equation.products[0], operations); 
}

void run_task_1(Equations equations) {
  cout << "Task 1" << endl;
  long result = 0;
  Operations operations = {multiply, sum, concatenate};
  for (int i = 0; i < LINES_NUM; i++) {
    Equation equation = equations[i];
    if (validate_result(equation, operations)) {
      result += equation.testValue;
    }
  }

  cout << "Total calibration result: " << result << endl;
}
int main() {
  Equations equations = read_file("./input.txt");
  print_arr(equations);
  run_task_1(equations);
  return 0;
}
