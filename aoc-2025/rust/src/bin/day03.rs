//! https://adventofcode.com/2025/day/3
use std::env;
use std::fs;
use std::path::Path;
use std::time::Instant;

const DEFAULT_INPUT: &str = "../input/day03.txt";
const DEFAULT_OUTPUT: &str = "output/day03.txt";

fn part_one(content: &str) -> i64 {
    content
        .lines()
        .filter(|line| !line.is_empty())
        .map(|line| {
            let bytes = line.as_bytes();
            if bytes.len() < 2 {
                return 0;
            }
            let mut current_idx = 0;
            let mut num: i64 = 0;

            for i in (0..2).rev() {
                let mut max_val = -1;
                let mut found_idx = 0;

                let search_limit = bytes.len() - i;

                for (idx, &byte) in bytes
                    .iter()
                    .enumerate()
                    .take(search_limit)
                    .skip(current_idx)
                {
                    let val = byte as i64 - b'0' as i64;
                    if max_val < val {
                        max_val = val;
                        found_idx = idx;
                    }
                }
                num = num * 10 + max_val;
                current_idx = found_idx + 1;
            }
            num
        })
        .sum()
}

fn part_two(content: &str) -> i64 {
    content
        .lines()
        .filter(|line| !line.is_empty())
        .map(|line| {
            let bytes = line.as_bytes();
            if bytes.len() < 2 {
                return 0;
            }
            let mut current_idx = 0;
            let mut num: i64 = 0;

            for i in (0..12).rev() {
                let mut max_val = -1;
                let mut found_idx = 0;

                let search_limit = bytes.len() - i;

                for (idx, &byte) in bytes
                    .iter()
                    .enumerate()
                    .take(search_limit)
                    .skip(current_idx)
                {
                    let val = byte as i64 - b'0' as i64;
                    if max_val < val {
                        max_val = val;
                        found_idx = idx;
                    }
                }
                num = num * 10 + max_val;
                current_idx = found_idx + 1;
            }
            num
        })
        .sum()
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let input_path = args.get(1).map(|s| s.as_str()).unwrap_or(DEFAULT_INPUT);
    println!("Reading from: {}", input_path);

    let content = fs::read_to_string(input_path).expect("Could not read input file");

    let start = Instant::now();
    let p1 = part_one(&content);
    let p2 = part_two(&content);

    let duration = start.elapsed();

    println!("Part 1: {}", p1);
    println!("Part 2: {}", p2);

    println!("Average time: {:?}", duration);
    if let Some(parent) = Path::new(DEFAULT_OUTPUT).parent() {
        let _ = fs::create_dir_all(parent);
    }
    let _ = fs::write(DEFAULT_OUTPUT, format!("{}\n{}\n", p1, p2));
}
