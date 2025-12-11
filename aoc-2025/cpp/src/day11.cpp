#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

const std::string DEFAULT_INPUT = "../input/day11.txt";
const std::string DEFAULT_OUTPUT = "output/day11.txt";

using Graph = std::unordered_map<std::string, std::vector<std::string>>;
using Memo = std::unordered_map<std::string, long long>;

Graph parse_input(const std::string &filename) {
  std::ifstream file(filename);
  Graph adj;
  if (!file.is_open())
    return adj;

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty())
      continue;

    size_t colon_pos = line.find(':');
    if (colon_pos == std::string::npos)
      continue;

    std::string u = line.substr(0, colon_pos);
    std::string rem = line.substr(colon_pos + 1);

    std::stringstream ss(rem);
    std::string v;
    while (ss >> v)
      adj[u].push_back(v);
  }

  return adj;
}

long long count_paths(const std::string &curr, const std::string &target,
                      Graph &adj, Memo &memo) {
  if (curr == target)
    return 1;

  if (memo.count(curr))
    return memo[curr];

  long long ans = 0;

  if (adj.count(curr))
    for (auto &nbr : adj[curr])
      ans += count_paths(nbr, target, adj, memo);

  return memo[curr] = ans;
}

long long get_paths(const std::string &start, const std::string &end,
                    Graph &adj) {
  Memo memo;
  return count_paths(start, end, adj, memo);
}

long long part_one(const std::string &filename) {
  Graph adj = parse_input(filename);
  if (adj.empty())
    return -1;
  return get_paths("you", "out", adj);
}

long long part_two(const std::string &filename) {
  Graph adj = parse_input(filename);
  if (adj.empty())
    return -1;

  long long p1_leg1 = get_paths("svr", "dac", adj);
  long long p1_leg2 = get_paths("dac", "fft", adj);
  long long p1_leg3 = get_paths("fft", "out", adj);
  long long path1 = p1_leg1 * p1_leg2 * p1_leg3;

  long long p2_leg1 = get_paths("svr", "fft", adj);
  long long p2_leg2 = get_paths("fft", "dac", adj);
  long long p2_leg3 = get_paths("dac", "out", adj);
  long long path2 = p2_leg1 * p2_leg2 * p2_leg3;

  return path1 + path2;
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
