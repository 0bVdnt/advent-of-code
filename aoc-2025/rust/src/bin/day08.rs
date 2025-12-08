//! https://adventofcode.com/2025/day/8
use std::env;
use std::fs;
use std::path::Path;

const DEFAULT_INPUT: &str = "../input/day08.txt";
const DEFAULT_OUTPUT: &str = "output/day08.txt";

#[derive(Clone, Copy, Debug)]
struct Point {
    x: i64,
    y: i64,
    z: i64,
}

#[derive(Clone, Copy, Debug)]
struct Edge {
    u: usize,
    v: usize,
    dist_sq: i64,
}

struct Dsu {
    parent: Vec<usize>,
    size: Vec<usize>,
    comp_cnt: usize,
}

impl Dsu {
    fn new(n: usize) -> Self {
        Dsu {
            parent: (0..n).collect(),
            size: vec![1; n],
            comp_cnt: n,
        }
    }

    fn find(&mut self, i: usize) -> usize {
        let mut root = i;
        while root != self.parent[root] {
            root = self.parent[root];
        }
        let mut curr = i;
        while curr != root {
            let next = self.parent[curr];
            self.parent[curr] = root;
            curr = next;
        }
        root
    }

    fn unite(&mut self, i: usize, j: usize) -> bool {
        let root_i = self.find(i);
        let root_j = self.find(j);
        if root_i != root_j {
            if self.size[root_i] < self.size[root_j] {
                self.parent[root_i] = root_j;
                self.size[root_j] += self.size[root_i];
            } else {
                self.parent[root_j] = root_i;
                self.size[root_i] += self.size[root_j];
            }
            self.comp_cnt -= 1;
            true
        } else {
            false
        }
    }
}

fn parse_points(content: &str) -> Vec<Point> {
    content
        .lines()
        .filter(|line| !line.trim().is_empty())
        .map(|line| {
            let parts: Vec<i64> = line
                .replace(',', " ")
                .split_whitespace()
                .filter_map(|s| s.parse().ok())
                .collect();

            Point {
                x: parts[0],
                y: parts[1],
                z: parts[2],
            }
        })
        .collect()
}

fn generate_edges(points: &[Point]) -> Vec<Edge> {
    let n = points.len();
    if n == 0 {
        return Vec::new();
    }
    let mut edges = Vec::with_capacity(n * (n - 1) / 2);
    for i in 0..n {
        for j in (i + 1)..n {
            let dx = points[i].x - points[j].x;
            let dy = points[i].y - points[j].y;
            let dz = points[i].z - points[j].z;
            let dist_sq = dx * dx + dy * dy + dz * dz;
            edges.push(Edge {
                u: i,
                v: j,
                dist_sq,
            });
        }
    }
    edges
}

fn part_one(content: &str) -> i64 {
    let points = parse_points(content);
    if points.is_empty() {
        return 0;
    }

    let mut edges = generate_edges(&points);
    edges.sort_unstable_by_key(|e| e.dist_sq);

    let mut dsu = Dsu::new(points.len());

    let k = 1000.min(edges.len());
    for edge in edges.iter().take(k) {
        dsu.unite(edge.u, edge.v);
    }

    let mut sizes: Vec<i64> = (0..points.len())
        .filter(|&i| dsu.parent[i] == i)
        .map(|i| dsu.size[i] as i64)
        .collect();

    sizes.sort_unstable_by(|a, b| b.cmp(a));

    sizes.iter().take(3).product()
}

fn part_two(content: &str) -> i64 {
    let points = parse_points(content);
    if points.is_empty() {
        return 0;
    }

    let mut edges = generate_edges(&points);
    edges.sort_unstable_by_key(|e| e.dist_sq);

    let mut dsu = Dsu::new(points.len());

    for edge in edges {
        if dsu.unite(edge.u, edge.v) && dsu.comp_cnt == 1 {
            return points[edge.u].x * points[edge.v].x;
        }
    }

    0
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let input_path = args.get(1).map(|s| s.as_str()).unwrap_or(DEFAULT_INPUT);
    println!("Reading from: {}", input_path);

    let content = fs::read_to_string(input_path).expect("Could not read input file");

    let p1 = part_one(&content);
    let p2 = part_two(&content);

    println!("Part 1: {}", p1);
    println!("Part 2: {}", p2);

    if let Some(parent) = Path::new(DEFAULT_OUTPUT).parent() {
        let _ = fs::create_dir_all(parent);
    }
    let _ = fs::write(DEFAULT_OUTPUT, format!("{}\n{}\n", p1, p2));
}
