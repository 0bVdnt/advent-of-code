//! https://adventofcode.com/2025/day/6
use std::env;
use std::fs;
use std::path::Path;

const DEFAULT_INPUT: &str = "";
const DEFAULT_OUTPUT: &str = "";

fn is_col_empty(grid: &[Vec<char>], col: usize) -> bool {
    grid.iter()
        .all(|row| row.get(col).copied().unwrap_or(' ') == ' ')
}

fn solve(content: &str, is_pt_two: bool) -> i64 {
    let lines: Vec<&str> = content.lines().collect();
    if lines.is_empty() {
        return 0;
    }

    let max_width = lines.iter().map(|l| l.len()).max().unwrap_or(0);

    let grid: Vec<Vec<char>> = lines
        .iter()
        .map(|l| {
            let mut chars: Vec<char> = l.chars().collect();
            while chars.len() < max_width {
                chars.push(' ');
            }
            chars
        })
        .collect();
    let mut ans: i64 = 0;
    let mut st = None;

    for x in 0..=max_width {
        let empty = if x == max_width {
            true
        } else {
            is_col_empty(&grid, x)
        };

        if !empty {
            if st.is_none() {
                st = Some(x);
            }
        } else if let Some(start) = st {
            if is_pt_two {
                ans += part_two(&grid, start, x);
            } else {
                ans += part_one(&grid, start, x);
            }
            st = None;
        }
    }
    ans
}

fn part_one(grid: &[Vec<char>], start: usize, end: usize) -> i64 {
    let mut tokens = Vec::new();
    for row in grid {
        let s: String = row[start..end].iter().collect();
        for part in s.split_whitespace() {
            tokens.push(part.to_string());
        }
    }

    if tokens.len() < 2 {
        return 0;
    }

    let op_str = tokens.pop().unwrap();
    let op = op_str.chars().next().unwrap_or(' ');

    let nums: Vec<i64> = tokens
        .iter()
        .filter_map(|t| t.parse::<i64>().ok())
        .collect();

    if nums.is_empty() {
        return 0;
    }

    let mut ans = nums[0];
    for &n in &nums[1..] {
        match op {
            '+' => ans += n,
            '*' => ans *= n,
            _ => {}
        }
    }
    ans
}

fn part_two(grid: &[Vec<char>], start: usize, end: usize) -> i64 {
    let mut nums = Vec::new();
    let mut op = ' ';

    for col in (start..end).rev() {
        let mut s = String::new();
        for row in grid {
            let c = row[col];
            if c.is_ascii_digit() {
                s.push(c);
            } else if c == '+' || c == '*' {
                op = c;
            }
        }

        if !s.is_empty() {
            if let Ok(n) = s.parse::<i64>() {
                nums.push(n);
            }
        }
    }

    if nums.is_empty() || (op != '+' && op != '*') {
        return 0;
    }

    let mut ans = nums[0];
    for &n in &nums[1..] {
        match op {
            '+' => ans += n,
            '*' => ans *= n,
            _ => {}
        }
    }
    ans
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let input_path = args.get(1).map(|s| s.as_str()).unwrap_or(DEFAULT_INPUT);
    println!("Reading from: {}", input_path);

    let content = fs::read_to_string(input_path).expect("Could not read input file");

    let p1 = solve(&content, false);
    let p2 = solve(&content, true);

    println!("Part 1: {}", p1);
    println!("Part 2: {}", p2);

    if let Some(parent) = Path::new(DEFAULT_OUTPUT).parent() {
        let _ = fs::create_dir_all(parent);
    }
    let _ = fs::write(DEFAULT_OUTPUT, format!("{}\n{}\n", p1, p2));
}
