//! https://adventofcode.com/2025/day/2
use std::env;
use std::fs;
use std::path::Path;

const DEFAULT_INPUT: &str = "";
const DEFAULT_OUTPUT: &str = "";

fn parse_ranges(content: &str) -> Vec<(i64, i64)> {
    let clean_content: String = content.chars().filter(|c| !c.is_whitespace()).collect();

    let mut ranges = Vec::new();

    for segment in clean_content.split(',') {
        if segment.is_empty() {
            continue;
        }
        if let Some((s_str, e_str)) = segment.split_once('-') {
            if let (Ok(s), Ok(e)) = (s_str.parse::<i64>(), e_str.parse::<i64>()) {
                ranges.push((s, e));
            }
        }
    }
    ranges
}

fn is_valid_pt1(n: i64) -> bool {
    let s = n.to_string();
    let len = s.len();
    if len % 2 != 0 {
        return false;
    }
    let mid = len / 2;
    s[..mid] == s[mid..]
}

fn is_valid_pt2(n: i64) -> bool {
    let s = n.to_string();
    let bytes = s.as_bytes();
    let len = bytes.len();

    for l in 1..=len / 2 {
        if len % l == 0 {
            let mut matches = true;
            for i in l..len {
                if bytes[i] != bytes[i - l] {
                    matches = false;
                    break;
                }
            }
            if matches {
                return true;
            }
        }
    }
    false
}

fn part_one(content: &str) -> i64 {
    let ranges = parse_ranges(content);
    let mut ans: i64 = 0;

    for (start, end) in ranges {
        for i in start..=end {
            if is_valid_pt1(i) {
                ans += i;
            }
        }
    }
    ans
}

fn part_two(content: &str) -> i64 {
    let ranges = parse_ranges(content);
    let mut ans: i64 = 0;

    for (start, end) in ranges {
        for i in start..=end {
            if is_valid_pt2(i) {
                ans += i;
            }
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
