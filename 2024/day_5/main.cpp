#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include<algorithm>

using namespace std;

const int LINES_NUM = 1362;
const int MAX_PAGE_NR = 100;

void print_row(vector<int> row) {
  for (int i = 0; i < row.size(); i++) {
    cout << row[i] << " ";
  }
  cout << endl;
}

int read_rules(ifstream &file, vector<int> *rules) {
  int page_nr;
  int successor;
  char c;
  int counter = 0;
  std::string line;
  for (int i = 0; i < LINES_NUM; i++) {
    counter++;
    getline(file, line);
    if (line.empty()) {
      return counter;
    }
    istringstream iss(line);
    iss >> page_nr >> c >> successor;
    assert(page_nr < MAX_PAGE_NR && "Error: Increase MAX_PAGE_NR");
    rules[page_nr].push_back(successor);
  }
  return counter;
}

void read_updates(ifstream &file, vector<vector<int>> &updates) {
  char c;
  int page_nr;
  int idx = 0;
  while (file >> page_nr) {
    file.get(c); // use get to capture '\n'
    updates[idx].push_back(page_nr);
    if (c == '\n') {
      idx++;
    }
  }
}

vector<vector<int>> read_file(string name, vector<int> *rules) {
  ifstream file;
  file.open(name);
  if (!file.is_open()) {
    cout << "Error opening file" << endl;
    throw "Error opening file";
  }
  int spareLines = LINES_NUM - read_rules(file, rules);
  vector<vector<int>> updates(spareLines, vector<int>());
  read_updates(file, updates);

  file.close();
  return updates;
}

bool is_any_visited_in_tabu(vector<int>& tabu, vector<int>& visited) {
  for (int i= 0; i < visited.size(); i++) {
    if (find(tabu.begin(), tabu.end(), visited[i]) != tabu.end()) {
      return true;
    }
  }
  return false;
}

bool is_ordering_correct(vector<int> *rules, vector<int> &updateRow) {
  vector<int> visited = {updateRow[0]};
  for (int i = 1; i < updateRow.size(); i++) {
    vector<int> tabu = rules[updateRow[i]];
    if (is_any_visited_in_tabu(tabu, visited)) {
      return false;
    } 
    visited.push_back(updateRow[i]);
  }
  return true;
}

void run_task_1(vector<int> *rules, vector<vector<int>> &updates) {
  cout << "Task 1" << endl;
  int sumOfMiddles = 0;
  for (int i = 0; i < updates.size(); i++) {
    if (is_ordering_correct(rules, updates[i])) {
      sumOfMiddles += updates[i][updates[i].size() / 2];
    }
  }
  cout << "Sum of middle elements: " << sumOfMiddles << endl << endl;
}

int main() {
  vector<int> *rules = new vector<int>[MAX_PAGE_NR];
  vector<vector<int>> updates = read_file("./input.txt", rules);

  run_task_1(rules, updates);
  
  delete[] rules;
  return 0;
}
