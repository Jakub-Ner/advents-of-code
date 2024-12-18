#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

const int N = 19999;
const int FREE_SPACE_SIGN = -1;

enum SpaceType { USED = 0, FREE = 1 };

void decode_space(vector<int> &decodedDisk, SpaceType type, int spaceLength,
                  int &fileIndex) {
  int spaceValue = FREE_SPACE_SIGN;
  if (type == USED) {
    spaceValue = fileIndex;
    fileIndex++;
  }

  for (int i = 0; i < spaceLength; i++) {
    decodedDisk.push_back(spaceValue);
  }
}

void print(vector<int> &disk) {
  for (int i = 0; i < disk.size(); i++) {
    if (disk[i] == FREE_SPACE_SIGN) {
      cout << ". ";
    } else
      cout << disk[i] << " ";
  }
  cout << endl;
}

vector<int> load_data(string filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    // cout << "Error opening file" << endl;
    throw "Error opening file";
  }

  vector<int> decodedDisk;
  decodedDisk.reserve(N);
  char c;
  int spaceIndex = 0;
  for (int i = 0; i < N; i++) {
    file.get(c);
    decode_space(decodedDisk, static_cast<SpaceType>(i % 2), c - '0',
                 spaceIndex);
  }
  // print(decodedDisk);
  file.close();
  return decodedDisk;
}

long calculate_checksum(vector<int> &disk) {
  long checksum = 0;
  for (int i = 0; i < disk.size(); i++) {
    if (disk[i] != FREE_SPACE_SIGN) {
      checksum += i * disk[i];
    }
  }
  return checksum;
}

void naively_defragment_disk(vector<int> &disk) {
  int right = disk.size() - 1;
  int left = 0;
  while (left < right) {
    while (disk[left] != FREE_SPACE_SIGN)
      left++;

    while (disk[right] == FREE_SPACE_SIGN)
      right--;

    if (left > right)
      break;

    swap(disk[left], disk[right]);
  }
  // cout << "\nDefragmented disk: " << endl;
  // print(disk);
}

struct Fragment {
  int startIdx;
  int endIdx;
};

int get_size(Fragment *fragment) {
  if (fragment->startIdx > fragment->endIdx) {
    return fragment->startIdx - fragment->endIdx + 1;
  }
  return fragment->endIdx - fragment->startIdx + 1;
}
Fragment *find_fragment(vector<int> &disk, int start, int step) {
  Fragment *fragment = new Fragment();
  int repeatedValue = disk[start];
  fragment->startIdx = start;
  while (disk[start] == repeatedValue) {
    start += step;
  }
  fragment->endIdx = start - step;
  return fragment;
}
void print_block(vector<int> &disk, int start, int end) {
  for (int i = start; i <= end; i++) {
    if (disk[i] == FREE_SPACE_SIGN) {
      cout << ". ";
    } else
      cout << disk[i] << " ";
  }
  cout << endl;
}
void efficiently_defragment_disk(vector<int> &disk) {
  int left = 0;
  int right = disk.size() - 1;

  while (left < right) {
    Fragment *freeSpace;
    Fragment *file;
    do {
      file = find_fragment(disk, right, -1);
      right = file->endIdx - 1;
      if (right < 0) {
        delete file;
        return;
      }
    } while (disk[file->startIdx] == FREE_SPACE_SIGN);

    while (left <= right) {
      freeSpace = find_fragment(disk, left, 1);
      left = freeSpace->endIdx + 1;
      if (disk[freeSpace->endIdx] != FREE_SPACE_SIGN) {
        delete freeSpace;
        continue;
      }
      if (get_size(file) <= get_size(freeSpace)) {
        assert(freeSpace->startIdx + get_size(file) <= disk.size() &&
               "File out of disk bounds");

        swap_ranges(disk.begin() + file->endIdx,
                    disk.begin() + file->startIdx + 1,
                    disk.begin() + freeSpace->startIdx);
        break;
      }
      delete freeSpace;
    }
    // print(disk);
    left = 0;
    delete file;
  }
}

void run_task_2(const char *filename) {
  cout << "Task 2" << endl;
  vector<int> decodedDisk = load_data(filename);
  efficiently_defragment_disk(decodedDisk);
  long checksum = calculate_checksum(decodedDisk);

  cout << "Checksum: " << checksum << endl;
}

void run_task_1(const char *filename) {
  cout << "Task 1" << endl;
  vector<int> decodedDisk = load_data(filename);
  naively_defragment_disk(decodedDisk);
  long checksum = calculate_checksum(decodedDisk);

  cout << "Checksum: " << checksum << endl;
}

int main() {
  run_task_1("./input.txt");
  run_task_2("./input.txt");
  return 0;
}
