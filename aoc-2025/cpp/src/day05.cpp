#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const std::string DEFAULT_INPUT = "";
const std::string DEFAULT_OUTPUT = "";

using Range = std::pair<long long, long long>;

int part_one(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open())
    return -1;

  std::string line;
  std::vector<Range> ranges;
  bool parsing_ranges = true;
  int fresh_cnt = 0;

  while (std::getline(file, line)) {
    if (line.empty()) {
      parsing_ranges = false;
      continue;
    }
    if (parsing_ranges) {
      size_t dash_pos = line.find('-');
      if (dash_pos != std::string::npos) {
        long long start = std::stoll(line.substr(0, dash_pos));
        long long end = std::stoll(line.substr(dash_pos + 1));
        ranges.push_back({start, end});
      }
    } else {
      long long id = std::stoll(line);
      bool is_fresh = false;
      for (auto &[s, e] : ranges) {
        if (id >= s && id <= e) {
          is_fresh = true;
          break;
        }
      }
      if (is_fresh)
        fresh_cnt++;
    }
  }
  return fresh_cnt;
}

long long part_two(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open())
    return -1;

  std::string line;
  std::vector<Range> ranges;

  while (std::getline(file, line)) {
    if (line.empty()) {
      break;
    }

    size_t dash_pos = line.find('-');
    if (dash_pos != std::string::npos) {
      long long start = std::stoll(line.substr(0, dash_pos));
      long long end = std::stoll(line.substr(dash_pos + 1));
      ranges.push_back({start, end});
    }
  }
  if (ranges.empty())
    return 0;

  std::sort(ranges.begin(), ranges.end());

  std::vector<Range> merged;
  merged.push_back((ranges[0]));

  for (size_t i = 1; i < ranges.size(); i++) {
    if (ranges[i].first <= merged.back().second)
      merged.back().second = std::max(merged.back().second, ranges[i].second);
    else
      merged.push_back(ranges[i]);
  }

  long long ans = 0;
  for (auto &[s, e] : merged)
    ans += e - s + 1;
  return ans;
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
