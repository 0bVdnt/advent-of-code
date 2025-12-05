#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const std::string DEFAULT_INPUT = "../input/day04.txt";
const std::string DEFAULT_OUTPUT = "output/day04.txt";

const int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

int count_neighbors(std::vector<std::string> &grid, int r, int c) {
  int cnt = 0;
  for (int i = 0; i < 8; i++) {
    int nr = r + dr[i];
    int nc = c + dc[i];
    if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[0].size())
      if (grid[nr][nc] == '@')
        cnt++;
  }
  return cnt;
}

int part_one(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open())
    return -1;

  std::string line;
  std::vector<std::string> grid;
  int ans = 0;
  while (std::getline(file, line)) {
    if (!line.empty())
      grid.push_back(line);
  }
  if (grid.empty())
    return 0;
  for (int r = 0; r < grid.size(); r++) {
    for (int c = 0; c < grid[0].size(); c++) {
      if (grid[r][c] == '@') {
        int nbrs = count_neighbors(grid, r, c);
        if (nbrs < 4)
          ans++;
      }
    }
  }
  return ans;
}

int part_two(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open())
    return -1;

  std::string line;
  std::vector<std::string> grid;
  int ans = 0;
  while (std::getline(file, line)) {
    if (!line.empty())
      grid.push_back(line);
  }
  if (grid.empty())
    return 0;

  while (true) {
    std::vector<std::pair<int, int>> to_del;
    for (int r = 0; r < grid.size(); r++) {
      for (int c = 0; c < grid[0].size(); c++) {
        if (grid[r][c] == '@')
          if (count_neighbors(grid, r, c) < 4)
            to_del.push_back({r, c});
      }
    }
    if (to_del.empty())
      break;

    ans += to_del.size();
    for (auto &[y, x] : to_del)
      grid[y][x] = '~';
  }

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
