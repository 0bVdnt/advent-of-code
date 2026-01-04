#include <chrono>
#include <climits>
#include <fstream>
#include <iostream>
#include <ratio>
#include <sstream>
#include <string>

const std::string DEFAULT_INPUT = "";
const std::string DEFAULT_OUTPUT = "";

int part_one(std::stringstream &file) {

  std::string line;
  int ans = 0;
  char del = '\n';
  while (std::getline(file, line, del)) {
    int tens = INT_MIN, units = INT_MIN, tensIdx = -1;
    for (int i = 0; i < line.size() - 1; i++) {
      if (tens < (int)(line[i] - '0')) {
        tens = (int)(line[i] - '0');
        tensIdx = i;
      }
    }
    for (int i = tensIdx + 1; i < line.size(); i++) {
      if (units < (int)(line[i] - '0')) {
        units = (int)(line[i] - '0');
      }
    }
    ans += (tens * 10 + units);
  }
  return ans;
}

long long part_two(std::stringstream &file) {
  std::string line;
  long long ans = 0;
  while (std::getline(file, line)) {
    int lastMaxiIdx = -1;
    long long currNum = 0;
    for (int i = 11; i >= 0; i--) {
      long long lastDig = LLONG_MIN;
      for (int j = lastMaxiIdx + 1; j < line.length() - i; j++) {
        if (lastDig < (int)(line[j] - '0')) {
          lastDig = (int)(line[j] - '0');
          lastMaxiIdx = j;
        }
      }
      currNum = currNum * 10 + lastDig;
    }
    ans += currNum;
  }
  return ans;
}

int main(int argc, char *argv[]) {
  std::string input_file = (argc > 1) ? argv[1] : DEFAULT_INPUT;
  std::cout << "Reading from: " << input_file << std::endl;

  std::ifstream file(input_file);
  if (!file.is_open())
    return -1;

  std::stringstream ss1, ss2;
  ss1 << file.rdbuf();
  file.seekg(0);
  ss2 << file.rdbuf();

  auto start = std::chrono::high_resolution_clock::now();

  auto p1 = part_one(ss1);
  auto p2 = part_two(ss2);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::micro> duration_us = end - start;

  std::cout << "Part 1: " << p1 << std::endl;
  std::cout << "Part 2: " << p2 << std::endl;
  std::cout << "Execution time for the algorithm: " << duration_us.count()
            << " us" << std::endl;

  std::ofstream oFile(DEFAULT_OUTPUT);
  if (oFile.is_open())
    oFile << p1 << "\n" << p2 << "\n";

  return 0;
}
