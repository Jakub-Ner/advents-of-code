#include <fstream>
#include <iostream>

using namespace std;

const int N_COLUMNS = 1000;
int iters = 0;

void read_columns(int *leftColumn, int *rightColumn) {
  ifstream file("input.txt");
  if (!file.is_open()) {
    cout << "Error opening file" << endl;
    return;
  }

  for (int i = 0; i < N_COLUMNS; i++) {
    file >> leftColumn[i] >> rightColumn[i];
  }

  file.close();
}

void swap(int *arr, int left, int right) {
  int tmp = arr[left];
  iters++;
  arr[left] = arr[right];
  arr[right] = tmp;
}

void print_arr(int *arr, int n) {
  for (int i = 0; i < n; i++)
    cout << arr[i] << " ";
  cout << endl;
}

int partition(int *arr, int low, int high) {
  int pivot = arr[low];
  int i = low - 1;
  int j = high + 1;

  while (true) {
    do { i++; }
    while (arr[i] < pivot);

    do { j--; }
    while (arr[j] > pivot);

    if (i >= j)
      break;
    swap(arr, i, j);

  }
  return j;
}

void quick_sort(int *arr, int low, int high) {
  if (low >= high)
    return;

  int partitionIdx = partition(arr, low, high);
  quick_sort(arr, low, partitionIdx);
  quick_sort(arr, partitionIdx + 1, high);
}

int distance(int a, int b){
  if (a < b) return b - a;
  return a - b;
}

int sum_distances(int *leftColumn, int *rightColumn) {
  int dists = 0;
  for (int i=0; i < N_COLUMNS; i++){
    dists += distance(leftColumn[i], rightColumn[i]);
  }
  return dists;
}

void run_task_1(int *leftColumn, int *rightColumn) {
  quick_sort(leftColumn, 0, N_COLUMNS - 1);
  quick_sort(rightColumn, 0, N_COLUMNS - 1);
  cout << "\nIters by avg: " << iters/2 << " To sort " << N_COLUMNS << " elements" << endl;

  int dists = sum_distances(leftColumn, rightColumn);
  cout<< "Sum: " << dists << endl;
}


void run_task_2(int *leftColumn, int *rightColumn) {
  int size = leftColumn[N_COLUMNS - 1] + 1;
  int occuranes[size]; 

  for (int &element: occuranes)
    element = 0;

  for (int i=0; i < N_COLUMNS; i++){
    if (rightColumn[i] < size)
      occuranes[rightColumn[i]]++;
  }

  int sum = 0;
  for (int i=0; i < N_COLUMNS; i++){
    sum += occuranes[leftColumn[i]] * leftColumn[i];
  }
  cout<<"Similarity score: " << sum << endl;
}

int main() {
  int leftColumn[N_COLUMNS];
  int rightColumn[N_COLUMNS];

  read_columns(leftColumn, rightColumn);
  run_task_1(rightColumn, leftColumn);
  run_task_2(rightColumn, leftColumn);
  return 0;
}
