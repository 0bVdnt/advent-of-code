//! https://adventofcode.com/2025/day/5
use std::env;
use std::fs;
use std::path::Path;

const DEFAULT_INPUT: &str = "";
const DEFAULT_OUTPUT: &str = "";

fn part_one(content: &str) -> i32 {
    let mut ranges: Vec<(i64, i64)> = Vec::new();
    let mut parsing_ranges = true;
    let mut fresh_cnt = 0;

    for line in content.lines() {
        let line = line.trim();
        if line.is_empty() {
            parsing_ranges = false;
            continue;
        }
        if parsing_ranges {
            if let Some((start_str, end_str)) = line.split_once('-') {
                if let (Ok(start), Ok(end)) = (start_str.parse::<i64>(), end_str.parse::<i64>()) {
                    ranges.push((start, end));
                }
            }
        } else if let Ok(id) = line.parse::<i64>() {
            let mut is_fresh = false;
            for &(s, e) in &ranges {
                if id >= s && id <= e {
                    is_fresh = true;
                    break;
                }
            }
            if is_fresh {
                fresh_cnt += 1;
            }
        }
    }
    fresh_cnt
}

fn part_two(content: &str) -> i64 {
    let mut ranges: Vec<(i64, i64)> = Vec::new();

    for line in content.lines() {
        let line = line.trim();
        if line.is_empty() {
            break;
        }

        if let Some((start_str, end_str)) = line.split_once('-') {
            if let (Ok(start), Ok(end)) = (start_str.parse::<i64>(), end_str.parse::<i64>()) {
                ranges.push((start, end));
            }
        }
    }
    if ranges.is_empty() {
        return 0;
    }
    ranges.sort_unstable_by_key(|k| k.0);

    let mut merged: Vec<(i64, i64)> = Vec::new();
    merged.push(ranges[0]);

    for &(s, e) in &ranges[1..] {
        let last_idx = merged.len() - 1;
        let last = &mut merged[last_idx];

        if s <= last.1 {
            last.1 = last.1.max(e);
        } else {
            merged.push((s, e));
        }
    }
    let mut ans = 0;
    for (s, e) in merged {
        ans += e - s + 1;
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
