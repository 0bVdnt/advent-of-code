use std::env;
use std::fs;
use std::path::Path;

const DEFAULT_INPUT: &str = "../input/day04.txt";
const DEFAULT_OUTPUT: &str = "output/day04.txt";
const DR: [i32; 8] = [-1, -1, -1, 0, 0, 1, 1, 1];
const DC: [i32; 8] = [-1, 0, 1, -1, 1, -1, 0, 1];

fn parse_grid(content: &str) -> Vec<Vec<char>> {
    content
        .lines()
        .filter(|line| !line.is_empty())
        .map(|line| line.chars().collect())
        .collect()
}

fn count_neighbors(grid: &[Vec<char>], r: usize, c: usize) -> i32 {
    let rows = grid.len() as i32;
    let cols = grid[0].len() as i32;
    let mut cnt = 0;

    for i in 0..8 {
        let nr = (r as i32) + DR[i];
        let nc = (c as i32) + DC[i];

        if nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr as usize][nc as usize] == '@' {
            cnt += 1;
        }
    }
    cnt
}

fn part_one(content: &str) -> i32 {
    let grid = parse_grid(content);
    if grid.is_empty() {
        return 0;
    }

    let mut ans = 0;
    for r in 0..grid.len() {
        for c in 0..grid[0].len() {
            if grid[r][c] == '@' && count_neighbors(&grid, r, c) < 4 {
                ans += 1;
            }
        }
    }
    ans
}

fn part_two(content: &str) -> i32 {
    let mut grid = parse_grid(content);
    if grid.is_empty() {
        return 0;
    }
    let mut ans = 0;
    loop {
        let mut to_del = Vec::new();

        for r in 0..grid.len() {
            for c in 0..grid[0].len() {
                if grid[r][c] == '@' && count_neighbors(&grid, r, c) < 4 {
                    to_del.push((r, c));
                }
            }
        }
        if to_del.is_empty() {
            break;
        }
        ans += to_del.len() as i32;
        for (r, c) in to_del {
            grid[r][c] = '~';
        }
    }
    ans
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
