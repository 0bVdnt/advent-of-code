//! https://adventofcode.com/2025/day/7
use std::env;
use std::fs;
use std::path::Path;

const DEFAULT_INPUT: &str = "";
const DEFAULT_OUTPUT: &str = "";

fn part_one(content: &str) -> i32 {
    let grid: Vec<Vec<char>> = content
        .lines()
        .filter(|l| !l.is_empty())
        .map(|l| l.chars().collect())
        .collect();

    if grid.is_empty() {
        return 0;
    }
    let ht = grid.len();
    let wd = grid[0].len();
    let mut st = None;

    for r in 0..ht {
        for c in 0..wd {
            if grid[r][c] == 'S' {
                st = Some((r, c));
                break;
            }
        }
        if st.is_some() {
            break;
        }
    }

    let (st_r, st_c) = match st {
        Some(p) => p,
        None => return 0,
    };

    let mut curr = vec![st_c];
    let mut ans = 0;
    for r in st_r..ht {
        if curr.is_empty() {
            break;
        }
        let mut next = Vec::new();
        for c in curr {
            if c >= wd {
                continue;
            }

            let cell = grid[r][c];

            if cell == '^' {
                ans += 1;
                if c > 0 {
                    next.push(c - 1);
                }
                if c + 1 < wd {
                    next.push(c + 1);
                }
            } else {
                next.push(c);
            }
        }
        next.sort_unstable();
        next.dedup();
        curr = next;
    }
    ans
}

fn part_two(content: &str) -> i64 {
    let grid: Vec<Vec<char>> = content
        .lines()
        .filter(|l| !l.is_empty())
        .map(|l| l.chars().collect())
        .collect();

    if grid.is_empty() {
        return 0;
    }
    let ht = grid.len();
    let wd = grid[0].len();
    let mut st = None;

    for r in 0..ht {
        for c in 0..wd {
            if grid[r][c] == 'S' {
                st = Some((r, c));
                break;
            }
        }
        if st.is_some() {
            break;
        }
    }

    let (st_r, st_c) = match st {
        Some(p) => p,
        None => return 0,
    };

    let mut cnt = vec![0i64; wd];
    cnt[st_c] = 1;

    for r in st_r..ht {
        let mut next = vec![0i64; wd];
        for c in 0..wd {
            if cnt[c] == 0 {
                continue;
            }

            let cell = grid[r][c];

            if cell == '^' {
                if c > 0 {
                    next[c - 1] += cnt[c];
                }
                if c + 1 < wd {
                    next[c + 1] += cnt[c];
                }
            } else {
                next[c] += cnt[c];
            }
        }
        cnt = next;
    }
    cnt.iter().sum()
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
