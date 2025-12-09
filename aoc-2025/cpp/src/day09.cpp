#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string DEFAULT_INPUT = "../input/day09.txt";
const string DEFAULT_OUTPUT = "output/day09.txt";

const double EPS = 1e-9;

struct Point {
  long long x;
  long long y;
};

bool is_between(long long v, long long a, long long b) {
  return v > min(a, b) && v < max(a, b);
}

bool intervals_overlap(long long a, long long b, long long c, long long d) {
  return max(min(a, b), min(c, d)) < min(max(a, b), max(c, d));
}

bool on_segment(double px, double py, Point a, Point b) {
  if (a.x == b.x)
    return abs(px - a.x) < EPS && py >= min(a.y, b.y) && py <= max(a.y, b.y);
  else
    return abs(py - a.y) < EPS && px >= min(a.x, b.x) && px <= max(a.x, b.x);
}

int main(int argc, char *argv[]) {
  string filename = (argc > 1) ? argv[1] : "../input/day09.txt";
  ifstream file(filename);
  if (!file) {
    cerr << "Error: Could not open file " << filename << endl;
    return 1;
  }

  vector<Point> pts;
  string line;
  while (getline(file, line)) {
    size_t c = line.find(',');
    if (c != string::npos)
      pts.push_back({stoll(line.substr(0, c)), stoll(line.substr(c + 1))});
  }

  long long p1 = 0, p2 = 0;
  int n = pts.size();

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      long long x1 = pts[i].x, y1 = pts[i].y;
      long long x2 = pts[j].x, y2 = pts[j].y;

      long long w = abs(x1 - x2) + 1;
      long long h = abs(y1 - y2) + 1;
      long long area = w * h;

      if (area > p1)
        p1 = area;

      if (area <= p2)
        continue;

      long long min_x = min(x1, x2), max_x = max(x1, x2);
      long long min_y = min(y1, y2), max_y = max(y1, y2);

      bool cuts_through = false;
      for (int k = 0; k < n; ++k) {
        Point a = pts[k], b = pts[(k + 1) % n];
        if (a.x == b.x) {
          if (is_between(a.x, min_x, max_x) &&
              intervals_overlap(a.y, b.y, min_y, max_y))
            cuts_through = true;
        } else {
          if (is_between(a.y, min_y, max_y) &&
              intervals_overlap(a.x, b.x, min_x, max_x))
            cuts_through = true;
        }
        if (cuts_through)
          break;
      }
      if (cuts_through)
        continue;

      double cx = (x1 + x2) / 2.0;
      double cy = (y1 + y2) / 2.0;

      bool on_boundary = false;
      int intersections = 0;

      for (int k = 0; k < n; ++k) {
        Point a = pts[k], b = pts[(k + 1) % n];

        if (on_segment(cx, cy, a, b)) {
          on_boundary = true;
          break;
        }

        if ((a.y > cy) != (b.y > cy)) {
          double edge_x = (double)(b.x - a.x) * (cy - a.y) / (b.y - a.y) + a.x;
          if (cx < edge_x)
            intersections++;
        }
      }

      if (on_boundary || (intersections % 2 == 1))
        p2 = area;
    }
  }

  cout << "Part 1: " << p1 << endl;
  cout << "Part 2: " << p2 << endl;

  ofstream oFile(DEFAULT_OUTPUT);
  if (oFile.is_open())
    oFile << p1 << "\n" << p2 << "\n";

  return 0;
}
