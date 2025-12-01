//! Advent of Code 2025 - Day 1
//! https://adventofcode.com/2025/day/1

use std::env;
use std::fs;
use std::path::Path;

const DEFAULT_INPUT: &str = "../input/day01.txt";
const DEFAULT_OUTPUT: &str = "output/day01.txt";
const START_POS: i32 = 50;
const DIAL_SIZE: i32 = 100;

fn safe_mod(a: i32, b: i32) -> i32 {
    ((a % b) + b) % b
}

fn parse_line(line: &str) -> (char, i32) {
    let line = line.trim();
    let (dir_str, num_str) = line.split_at(1);
    let dir = dir_str.chars().next().unwrap();
    let num = num_str.parse::<i32>().unwrap_or(0);
    (dir, num)
}

fn part_one(filename: &str) -> i32 {
    let content = match fs::read_to_string(filename) {
        Ok(c) => c,
        Err(_) => {
            eprintln!("[Part 1] Error: Could not open file {}", filename);
            return -1;
        }
    };

    let mut curr = START_POS;
    let mut ans = 0;

    for line in content.lines().filter(|l| !l.is_empty()) {
        let (dir, num) = parse_line(line);

        if dir == 'R' {
            curr = safe_mod(curr + num, DIAL_SIZE);
        } else {
            curr = safe_mod(curr - num, DIAL_SIZE);
        }

        if curr == 0 {
            ans += 1;
        }
    }
    ans
}

fn part_two(filename: &str) -> i32 {
    let content = match fs::read_to_string(filename) {
        Ok(c) => c,
        Err(_) => {
            eprintln!("[Part 2] Error: Could not open file {}", filename);
            return -1;
        }
    };
    let mut curr = START_POS;
    let mut ans = 0;

    for line in content.lines().filter(|l| !l.is_empty()) {
        let (dir, num) = parse_line(line);
        let dist_to_zero;

        if dir == 'R' {
            dist_to_zero = safe_mod(DIAL_SIZE - curr, DIAL_SIZE);
            let actual_dist = if dist_to_zero == 0 {
                DIAL_SIZE
            } else {
                dist_to_zero
            };

            if num >= actual_dist {
                ans += 1 + (num - actual_dist) / DIAL_SIZE;
            }
            curr = safe_mod(curr + num, DIAL_SIZE);
        } else {
            dist_to_zero = if curr == 0 { DIAL_SIZE } else { curr };
            if num >= dist_to_zero {
                ans += 1 + (num - dist_to_zero) / DIAL_SIZE;
            }
            curr = safe_mod(curr - num, DIAL_SIZE);
        }
    }
    ans
}

fn main() {
    let cli_args: Vec<String> = env::args().collect();
    let input_path = cli_args.get(1).map(|s| s.as_str()).unwrap_or(DEFAULT_INPUT);

    println!("Reading from {}", input_path);

    let p1 = part_one(input_path);
    let p2 = part_two(input_path);

    println!("Part 1: {}", p1);
    println!("Part 2: {}", p2);

    if let Some(parent) = Path::new(DEFAULT_OUTPUT).parent() {
        let _ = fs::create_dir_all(parent);
    }

    fs::write(DEFAULT_OUTPUT, format!("{}\n{}\n", p1, p2)).expect("Could not write to output file");

    println!("Results saved to {}", DEFAULT_OUTPUT);
}
