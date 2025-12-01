#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

const std::string DEFAULT_INPUT = "../input/day01.txt";
const std::string DEFAULT_OUTPUT = "../cpp/output/day01.txt";
const int START_POS = 50;
const int DIAL_SIZE = 100;

int safe_mod(int a, int b) { return (a % b + b) % b; }

int part_one(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "[Part 1] Error: Could not open file " << filename << '\n';
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

int part_two(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "[Part 2] Error: Could not open the file " << filename << '\n';
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

int main(int argc, char *argv[]) {
  std::string input_file = (argc > 1) ? argv[1] : DEFAULT_INPUT;

  std::cout << "Reading from: " << input_file << std::endl;

  int p1 = part_one(input_file);
  int p2 = part_two(input_file);

  std::cout << "Part 1: " << p1 << std::endl;
  std::cout << "Part 2: " << p2 << std::endl;

  std::ofstream oFile(DEFAULT_OUTPUT);
  if (oFile.is_open()) {
    oFile << p1 << '\n' << p2 << '\n';
    std::cout << "Saved to " << DEFAULT_OUTPUT << std::endl;
  } else {
    std::cerr << "Error writing to " << DEFAULT_OUTPUT << std::endl;
  }
  return 0;
}
