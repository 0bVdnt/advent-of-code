#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

const std::string DEFAULT_INPUT = "../input/day12.txt";
const std::string DEFAULT_OUTPUT = "output/day12.txt";

int part_one(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open())
    return -1;

  std::string line;

  for (int i = 0; i < 30; i++)
    std::getline(file, line);

  int ans = 0;

  while (std::getline(file, line)) {
    if (line.empty())
      continue;

    int w, h;
    sscanf(line.c_str(), "%dx%d:", &w, &h);

    int totalPieces = 0;
    std::istringstream iss(line.substr(line.find(':') + 1));

    int pieces;
    while (iss >> pieces)
      totalPieces += pieces;

    if (w * h >= totalPieces * 9)
      ans++;
  }
  return ans;
}

int part_two(const std::string &filename) {
  std::cout << "Merry Christmas 2025 !!!" << std::endl;
  return 0;
}

int main(int argc, char *argv[]) {
  std::string input_file = (argc > 1) ? argv[1] : DEFAULT_INPUT;
  std::cout << "Reading from: " << input_file << std::endl;

  auto p1 = part_one(input_file);
  auto p2 = part_two(input_file);

  std::cout << "Part 1: " << p1 << std::endl;
  std::cout << "Part 2: " << p2 << std::endl;

  std::ofstream oFile(DEFAULT_OUTPUT);
  if (oFile.is_open())
    oFile << p1 << "\n" << p2 << "\n";

  return 0;
}
