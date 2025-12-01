#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

const std::string INPUT_FILE = "../input/01.txt";
const std::string OUTPUT_FILE = "../output/01.txt";
const int START_POS = 50;
const int DIAL_SIZE = 100;

int safe_mod(int a, int b) { return (a % b + b) % b; }

int part_one() {
  std::ifstream file(INPUT_FILE);
  if (!file.is_open()) {
    std::cerr << "[Part 1] Error: Could not open file " << INPUT_FILE << '\n';
    return -1;
  }
  char dir;
  int num;
  int curr_pos = START_POS;
  int pass = 0;

  while (file >> dir >> num) {
    if (dir == 'R')
      curr_pos = safe_mod(curr_pos + num, DIAL_SIZE);
    else if (dir == 'L')
      curr_pos = safe_mod(curr_pos - num, DIAL_SIZE);

    if (curr_pos == 0)
      pass++;
  }
  return pass;
}

int part_two() {
  std::ifstream file(INPUT_FILE);
  if (!file.is_open()) {
    std::cerr << "[Part 2] Error: Could not open the file " << INPUT_FILE
              << '\n';
    return -1;
  }
  char dir;
  int num;
  int curr = START_POS;
  int ans = 0;
  while (file >> dir >> num) {
    int dist_to_zero = 0;
    if (dir == 'R') {
      dist_to_zero = safe_mod(DIAL_SIZE - curr, DIAL_SIZE);
      if (dist_to_zero == 0)
        dist_to_zero = DIAL_SIZE;

      if (num >= dist_to_zero)
        ans += 1 + (num - dist_to_zero) / DIAL_SIZE;

      curr = safe_mod(curr + num, DIAL_SIZE);
    } else if (dir == 'L') {
      dist_to_zero = curr == 0 ? DIAL_SIZE : curr;
      if (num >= dist_to_zero)
        ans += 1 + (num - dist_to_zero) / DIAL_SIZE;

      curr = safe_mod(curr - num, DIAL_SIZE);
    }
  }
  return ans;
}

int main() {
  std::ofstream oFile(OUTPUT_FILE);
  if (!oFile.is_open()) {
    std::cerr << "Error: Could not open the output file " << OUTPUT_FILE
              << '\n';
    std::cerr << "Make sure the directory '../output/' exists.\n";
    return 1;
  }
  int p1 = part_one();
  int p2 = part_two();

  std::cout << "Part 1: " << p1 << std::endl;
  std::cout << "Part 2: " << p2 << std::endl;

  oFile << p1 << '\n';
  oFile << p2 << '\n';

  std::cout << "Results have been stored to " << OUTPUT_FILE << std::endl;
  return 0;
}
