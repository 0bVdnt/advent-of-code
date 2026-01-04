#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

const std::string DEFAULT_INPUT = "";
const std::string DEFAULT_OUTPUT = "";

struct Point {
  long long x, y, z;
};

struct Edge {
  int u, v;
  long long dist_sq;
};

struct DSU {
  std::vector<int> parent;
  std::vector<long long> sz;
  int comp_cnt;

  DSU(int n) {
    parent.resize(n);
    std::iota(parent.begin(), parent.end(), 0);
    sz.assign(n, 1);
    comp_cnt = n;
  }

  int find(int i) {
    if (parent[i] == i)
      return i;
    return parent[i] = find(parent[i]);
  }

  bool unite(int i, int j) {
    int root_i = find(i);
    int root_j = find(j);
    if (root_i != root_j) {
      if (sz[root_i] < sz[root_j])
        std::swap(root_i, root_j);
      parent[root_j] = root_i;
      sz[root_i] += sz[root_j];
      comp_cnt--;
      return true;
    }
    return false;
  }
};

std::vector<Point> read_points(const std::string &filename) {
  std::ifstream file(filename);
  std::vector<Point> points;
  if (!file.is_open())
    return points;

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty())
      continue;
    for (char &c : line)
      if (c == ',')
        c = ' ';
    std::stringstream ss(line);
    long long x, y, z;
    if (ss >> x >> y >> z)
      points.push_back({x, y, z});
  }
  return points;
}

std::vector<Edge> generate_edges(const std::vector<Point> &points) {
  std::vector<Edge> edges;
  int n = points.size();
  edges.reserve((size_t)n * (n - 1) / 2);

  for (int i = 0; i < n; ++i)
    for (int j = i + 1; j < n; ++j) {
      long long dx = points[i].x - points[j].x;
      long long dy = points[i].y - points[j].y;
      long long dz = points[i].z - points[j].z;
      long long dist_sq = dx * dx + dy * dy + dz * dz;
      edges.push_back({i, j, dist_sq});
    }
  return edges;
}

long long part_one(const std::string &filename) {
  auto points = read_points(filename);
  if (points.empty())
    return -1;

  auto edges = generate_edges(points);

  std::sort(edges.begin(), edges.end(),
            [](const Edge &a, const Edge &b) { return a.dist_sq < b.dist_sq; });

  DSU dsu(points.size());

  int k = std::min(1000, (int)edges.size());
  for (size_t i = 0; i < k; ++i)
    dsu.unite(edges[i].u, edges[i].v);

  std::vector<long long> component_sizes;
  for (int i = 0; i < points.size(); ++i)
    if (dsu.parent[i] == i)
      component_sizes.push_back(dsu.sz[i]);

  std::sort(component_sizes.rbegin(), component_sizes.rend());

  long long ans = 1;
  for (int i = 0; i < 3 && i < (int)component_sizes.size(); ++i)
    ans *= component_sizes[i];
  return ans;
}

long long part_two(const std::string &filename) {
  auto points = read_points(filename);
  if (points.empty())
    return -1;

  auto edges = generate_edges(points);

  std::sort(edges.begin(), edges.end(),
            [](const Edge &a, const Edge &b) { return a.dist_sq < b.dist_sq; });

  DSU dsu(points.size());

  for (const auto &edge : edges)
    if (dsu.unite(edge.u, edge.v))
      if (dsu.comp_cnt == 1)
        return points[edge.u].x * points[edge.v].x;

  return 0;
}

int main(int argc, char *argv[]) {
  std::string input_file = (argc > 1) ? argv[1] : DEFAULT_INPUT;
  std::cout << "Reading from: " << input_file << std::endl;

  long long p1 = part_one(input_file);
  long long p2 = part_two(input_file);

  std::cout << "Part 1: " << p1 << std::endl;
  std::cout << "Part 2: " << p2 << std::endl;

  std::ofstream oFile(DEFAULT_OUTPUT);
  if (oFile.is_open())
    oFile << p1 << "\n" << p2 << "\n";

  return 0;
}
