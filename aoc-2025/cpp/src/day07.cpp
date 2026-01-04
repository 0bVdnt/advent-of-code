#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const std::string DEFAULT_INPUT = "";
const std::string DEFAULT_OUTPUT = "";

struct GridInfo {
  std::vector<std::string> grid;
  int st_r = -1;
  int st_c = -1;
  int ht = 0;
  int wd = 0;
};

GridInfo load_grid(const std::string &filename) {
  GridInfo info;
  std::ifstream file(filename);

  std::string line;
  while (std::getline(file, line)) {
    if (!line.empty() && line.back() == '\r')
      line.pop_back();
    if (!line.empty())
      info.grid.push_back(line);
  }

  if (info.grid.empty())
    return info;

  info.ht = (info.grid.size());
  info.wd = (info.grid[0].size());

  for (int r = 0; r < info.ht; r++) {
    size_t pos = info.grid[r].find('S');
    if (pos != std::string::npos) {
      info.st_r = r;
      info.st_c = pos;
      break;
    }
  }
  return info;
}

long long part_one(const std::string &filename) {
  auto info = load_grid(filename);
  if (info.grid.empty() || info.st_r == -1)
    return 0;

  std::vector<int> curr;
  curr.push_back(info.st_c);

  long long ans = 0;

  for (int r = info.st_r; r < info.ht; r++) {
    if (curr.empty())
      break;

    std::vector<int> next;
    next.reserve(curr.size() * 2);

    for (int c : curr) {
      if (c < 0 || c >= info.wd)
        continue;
      char cell = info.grid[r][c];
      if (cell == '^') {
        ans++;
        if (c - 1 >= 0)
          next.push_back(c - 1);
        if (c + 1 < info.wd)
          next.push_back(c + 1);
      } else {
        next.push_back(c);
      }
    }

    std::sort(next.begin(), next.end());
    next.erase(std::unique(next.begin(), next.end()), next.end());
    curr = next;
  }

  return ans;
}

long long part_two(const std::string &filename) {
  auto info = load_grid(filename);
  if (info.grid.empty() || info.st_r == -1)
    return 0;

  std::vector<long long> cnt(info.wd, 0);
  cnt[info.st_c] = 1;
  for (int r = info.st_r; r < info.ht; r++) {
    std::vector<long long> next(info.wd, 0);

    for (int c = 0; c < info.wd; c++) {
      if (cnt[c] == 0)
        continue;
      char cell = info.grid[r][c];

      if (cell == '^') {
        if (c - 1 >= 0) {
          next[c - 1] += cnt[c];
        }
        if (c + 1 < info.wd) {
          next[c + 1] += cnt[c];
        }
      } else {
        next[c] += cnt[c];
      }
    }
    cnt = next;
  }

  long long ans = 0;
  for (long long count : cnt)
    ans += count;
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
