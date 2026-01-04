#include <climits>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const std::string DEFAULT_INPUT = "";
const std::string DEFAULT_OUTPUT = "";

const double EPS = 1e-9;

struct Machine {
  std::vector<std::vector<int>> btns;
  std::vector<int> targets;
  int light_cnt;
};

Machine parse_line(const std::string &line) {
  Machine m;
  size_t ls = line.find('['), le = line.find(']');
  m.light_cnt = le - ls - 1;
  size_t pos = le + 1;
  while (pos < line.size() && line[pos] != '{') {
    if (line[pos] == '(') {
      size_t end = line.find(')', pos);
      std::stringstream ss(line.substr(pos + 1, end - pos - 1));
      std::vector<int> btn;
      std::string token;
      while (std::getline(ss, token, ','))
        btn.push_back(std::stoi(token));
      m.btns.push_back(btn);
      pos = end + 1;
    } else
      pos++;
  }
  size_t ts = line.find('{'), te = line.find('}');
  std::stringstream ss(line.substr(ts + 1, te - ts - 1));
  std::string token;
  while (std::getline(ss, token, ','))
    m.targets.push_back(std::stoi(token));
  return m;
}

int part_one(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  int ans = 0;
  while (std::getline(file, line)) {
    if (!line.empty()) {
      Machine m = parse_line(line);
      size_t ls = line.find('[');
      std::string lights = line.substr(ls + 1, m.light_cnt);
      int nb = m.btns.size(), mini = INT_MAX;
      for (int mask = 0; mask < (1 << nb); mask++) {
        int presses = __builtin_popcount(mask);
        if (presses >= mini)
          continue;

        std::vector<bool> curr(m.light_cnt, false);
        for (int b = 0; b < nb; b++)
          if (mask & (1 << b))
            for (int idx : m.btns[b])
              if (idx < m.light_cnt)
                curr[idx] = !curr[idx];
        bool fine = true;
        for (int i = 0; i < m.light_cnt && fine; i++)
          fine = (curr[i] == (lights[i] == '#'));

        if (fine)
          mini = presses;
      }
      ans += mini;
    }
  }
  return ans;
}

long long part_two(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  long long ans = 0;
  while (std::getline(file, line)) {
    if (!line.empty()) {
      Machine m = parse_line(line);
      int nc = m.targets.size(), nb = m.btns.size();
      if (nc == 0 || nb == 0)
        return 0;
      std::vector<std::vector<double>> aug(nc, std::vector<double>(nb + 1, 0));
      for (int bi = 0; bi < nb; bi++)
        for (int idx : m.btns[bi])
          if (idx < nc)
            aug[idx][bi] = 1;
      for (int i = 0; i < nc; i++)
        aug[i][nb] = m.targets[i];

      std::vector<int> pivot_cols(nc, -1);
      int rank = 0;
      for (int col = 0; col < nb && rank < nc; col++) {
        int pivot = -1;
        for (int row = rank; row < nc; row++)
          if (std::abs(aug[row][col]) > EPS) {
            pivot = row;
            break;
          }
        if (pivot == -1)
          continue;

        std::swap(aug[rank], aug[pivot]);
        pivot_cols[rank] = col;

        double piv = aug[rank][col];
        for (int j = 0; j <= nb; j++)
          aug[rank][j] /= piv;

        for (int row = 0; row < nc; row++)
          if (row != rank && std::abs(aug[row][col]) > EPS) {
            double fact = aug[row][col];
            for (int j = 0; j <= nb; j++)
              aug[row][j] -= fact * aug[rank][j];
          }
        rank++;
      }

      bool no_sol = false;
      for (int i = rank; i < nc; i++)
        if (std::abs(aug[i][nb]) > EPS)
          no_sol = true;

      if (no_sol)
        continue;

      std::vector<bool> is_pivot(nb, false);
      for (int i = 0; i < rank; i++)
        is_pivot[pivot_cols[i]] = true;

      std::vector<int> free_cols;
      for (int j = 0; j < nb; j++)
        if (!is_pivot[j])
          free_cols.push_back(j);

      int free_cnt = free_cols.size();

      std::vector<int> pivot_row(nb, -1);
      for (int i = 0; i < rank; i++)
        pivot_row[pivot_cols[i]] = i;

      long long mini = LLONG_MAX;
      std::vector<long long> free_vals(free_cnt, 0);

      std::function<void(int)> enumerate = [&](int idx) {
        if (idx == free_cnt) {
          std::vector<long long> solution(nb, 0);
          for (int i = 0; i < free_cnt; i++)
            solution[free_cols[i]] = free_vals[i];
          long long total = 0;
          for (int i = 0; i < free_cnt; i++)
            total += free_vals[i];

          if (total >= mini)
            return;

          for (int i = 0; i < rank; i++) {
            double val = aug[i][nb];
            for (int j = 0; j < nb; j++)
              if (j != pivot_cols[i])
                val -= aug[i][j] * solution[j];
            long long ival = std::llround(val);
            if (std::abs(val - ival) > EPS || ival < 0)
              return;
            solution[pivot_cols[i]] = ival;
            total += ival;
            if (total >= mini)
              return;
          }
          mini = total;
          return;
        }
        long long max_val = LLONG_MAX;
        for (int btn_idx : m.btns[free_cols[idx]])
          if (btn_idx < nc)
            max_val = std::min(max_val, (long long)m.targets[btn_idx]);

        if (max_val > 500)
          max_val = 500;

        for (long long v = 0; v <= max_val; v++) {
          free_vals[idx] = v;
          enumerate(idx + 1);
        }
      };
      enumerate(0);
      ans += (mini == LLONG_MAX ? 0 : mini);
    }
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
