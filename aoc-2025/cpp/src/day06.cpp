#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string DEFAULT_INPUT = "";
const std::string DEFAULT_OUTPUT = "";

bool is_col_empty(const std::vector<std::string> &lines, int col) {
  for (const auto &line : lines)
    if (col < (int)line.size() && line[col] != ' ')
      return false;
  return true;
}

long long part_one(const std::vector<std::string> &lines, int start, int end) {
  std::vector<std::string> tokens;

  for (const auto &line : lines) {
    if ((int)line.size() <= start)
      continue;
    int len = std::min((int)line.size(), end) - start;
    if (len <= 0)
      continue;

    std::string segment = line.substr(start, len);
    std::stringstream ss(segment);
    std::string token;
    while (ss >> token)
      tokens.push_back(token);
  }

  if (tokens.size() < 2)
    return 0;

  std::string opStr = tokens.back();
  tokens.pop_back();

  char op = opStr[0];
  if (op != '+' && op != '*')
    return 0;

  std::vector<long long> nums;
  for (const auto &t : tokens) {
    try {
      nums.push_back(std::stoll(t));
    } catch (...) {
    }
  }

  if (nums.empty())
    return 0;

  long long result = nums[0];
  for (size_t i = 1; i < nums.size(); ++i) {
    if (op == '+')
      result += nums[i];
    else
      result *= nums[i];
  }

  return result;
}

long long part_two(const std::vector<std::string> &lines, int start, int end) {
  std::vector<long long> nums;
  char op = 0;

  for (int x = end - 1; x >= start; --x) {
    std::string numStr;
    for (const auto &line : lines) {
      if (x < (int)line.size()) {
        char c = line[x];
        if (isdigit(c))
          numStr += c;
        else if (c == '+' || c == '*')
          op = c;
      }
    }
    if (!numStr.empty()) {
      try {
        nums.push_back(std::stoll(numStr));
      } catch (...) {
      }
    }
  }

  if (nums.empty() || (op != '+' && op != '*'))
    return 0;

  long long result = nums[0];
  for (size_t i = 1; i < nums.size(); ++i) {
    if (op == '+')
      result += nums[i];
    else
      result *= nums[i];
  }

  return result;
}

int main(int argc, char *argv[]) {
  std::string input_file = (argc > 1) ? argv[1] : DEFAULT_INPUT;
  std::cout << "Reading from: " << input_file << std::endl;

  std::ifstream file(input_file);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << input_file << std::endl;
    return -1;
  }

  std::vector<std::string> lines;
  std::string line;
  size_t max_width = 0;

  while (std::getline(file, line)) {
    lines.push_back(line);
    if (line.size() > max_width)
      max_width = line.size();
  }

  long long total_p1 = 0;
  long long total_p2 = 0;

  int start = -1;

  for (int x = 0; x <= (int)max_width; ++x) {
    bool empty = (x == (int)max_width) || is_col_empty(lines, x);

    if (!empty) {
      if (start == -1)
        start = x;
    } else {
      if (start != -1) {
        total_p1 += part_one(lines, start, x);
        total_p2 += part_two(lines, start, x);
        start = -1;
      }
    }
  }

  std::cout << "Part 1: " << total_p1 << std::endl;
  std::cout << "Part 2: " << total_p2 << std::endl;

  std::ofstream oFile(DEFAULT_OUTPUT);
  if (oFile.is_open())
    oFile << total_p1 << "\n" << total_p2 << "\n";

  return 0;
}
