#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string DEFAULT_INPUT = "../input/day02.txt";
const std::string DEFAULT_OUTPUT = "output/day02.txt";

using ll = long long;
using Range = std::pair<ll, ll>;

std::vector<Range> getRanges(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return {};
  }

  std::string content, line;
  while (file >> line)
    content += line;
  std::vector<Range> ranges;
  std::stringstream ss(content);
  std::string segment;
  while (std::getline(ss, segment, ',')) {
    if (segment.empty())
      continue;
    size_t sep = segment.find("-");
    if (sep != std::string::npos) {
      ll s = std::stoll(segment.substr(0, sep));
      ll e = std::stoll(segment.substr(sep + 1));
      ranges.push_back({s, e});
    }
  }
  return ranges;
}

bool isValidPt1(ll n) {
  std::string s = std::to_string(n);
  ll len = s.length();
  if (len % 2 != 0)
    return false;
  ll mid = len / 2;
  for (ll i = 0; i < mid; i++) {
    if (s[i] != s[mid + i])
      return false;
  }
  return true;
}

bool isValidPt2(ll n) {
  std::string s = std::to_string(n);
  ll len = s.length();
  ll mid = len / 2;
  for (ll l = 1; l <= mid; l++) {
    if (len % l == 0) {
      bool match = true;
      for (ll i = l; i < len; i++) {
        if (s[i] != s[i - l]) {
          match = false;
          break;
        }
      }
      if (match)
        return true;
    }
  }
  return false;
}

ll part_one(const std::string &filename) {
  std::vector<Range> ranges = getRanges(filename);
  ll ans = 0;
  for (auto &[s, e] : ranges)
    for (ll i = s; i <= e; i++)
      if (isValidPt1(i))
        ans += i;
  return ans;
}

ll part_two(const std::string &filename) {
  std::vector<Range> ranges = getRanges(filename);
  ll ans = 0;
  for (auto &[s, e] : ranges)
    for (ll i = s; i <= e; i++)
      if (isValidPt2(i))
        ans += i;
  return ans;
}

int main(int argc, char *argv[]) {
  std::string input_file = (argc > 1) ? argv[1] : DEFAULT_INPUT;
  std::cout << "Reading from: " << input_file << std::endl;

  ll p1 = part_one(input_file);
  ll p2 = part_two(input_file);

  std::cout << "Part 1: " << p1 << std::endl;
  std::cout << "Part 2: " << p2 << std::endl;

  std::ofstream oFile(DEFAULT_OUTPUT);
  if (oFile.is_open())
    oFile << p1 << "\n" << p2 << "\n";

  return 0;
}
