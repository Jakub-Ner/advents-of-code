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
  int pageNr;
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
    iss >> pageNr >> c >> successor;
    assert(pageNr < MAX_PAGE_NR && "Error: Increase MAX_PAGE_NR");
    rules[pageNr].push_back(successor);
  }
  return counter;
}

void read_updates(ifstream &file, vector<vector<int>> &updates) {
  char c;
  int pageNr;
  int idx = 0;
  while (file >> pageNr) {
    file.get(c); // use get to capture '\n'
    updates[idx].push_back(pageNr);
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

int find_invalid_page_id(vector<int>& tabu, vector<int>& visited) {
  for (int i= 0; i < visited.size(); i++) {
    if (find(tabu.begin(), tabu.end(), visited[i]) != tabu.end()) {
      return i;
    }
  }
  return -1;
}

struct InvalidPage {
  int pageNr;
  int successor;
};

InvalidPage find_invalid_ordering(vector<int> *rules, vector<int> &updateRow) {
  vector<int> visited = {updateRow[0]};
  for (int i = 1; i < updateRow.size(); i++) {
    vector<int> tabu = rules[updateRow[i]];
    int idx = find_invalid_page_id(tabu, visited);
    if (idx != -1) {
      return InvalidPage{idx, i};
    } 
    visited.push_back(updateRow[i]);
  }
  return InvalidPage{-1, -1};
}

void swap(int &a, int &b) {
  int tmp = a;
  a = b;
  b = tmp;
}

void run_task_2(vector<int> *rules, vector<vector<int>> &updates) {
  cout << "Task 2" << endl;
  int sumOfMiddles = 0;
  for (int i = 0; i < updates.size(); i++) {
    InvalidPage invalid = find_invalid_ordering(rules, updates[i]);
    if (invalid.pageNr != -1) {

      while (invalid.pageNr != -1){
        // cout << "Incorrect ordering: Should be page=" << updates[i][invalid.successor] << " successor=" << updates[i][invalid.pageNr] << endl;
        swap(updates[i][invalid.pageNr], updates[i][invalid.successor]);
        invalid = find_invalid_ordering(rules, updates[i]);
      }

      sumOfMiddles += updates[i][updates[i].size() / 2];
      // print_row(updates[i]);
    }
  }
  cout << "Sum of middle elements: " << sumOfMiddles << endl << endl;
}

void run_task_1(vector<int> *rules, vector<vector<int>> &updates) {
  cout << "Task 1" << endl;
  int sumOfMiddles = 0;
  for (int i = 0; i < updates.size(); i++) {
    if (find_invalid_ordering(rules, updates[i]).pageNr == -1) {
      sumOfMiddles += updates[i][updates[i].size() / 2];
    }
  }
  cout << "Sum of middle elements: " << sumOfMiddles << endl << endl;
}

int main() {
  vector<int> *rules = new vector<int>[MAX_PAGE_NR];
  vector<vector<int>> updates = read_file("./input.txt", rules);

  run_task_1(rules, updates);
  run_task_2(rules, updates); 
  delete[] rules;
  return 0;
}
