//! https://adventofcode.com/2025/day/12
use std::env;
use std::fs;
use std::path::Path;

const DEFAULT_INPUT: &str = "";
const DEFAULT_OUTPUT: &str = "";

fn part_one(content: &str) -> i32 {
    let mut ans = 0i32;

    for line in content.lines().skip(30) {
        let line = line.trim();
        if line.is_empty() {
            continue;
        }

        let col_pos = match line.find(':') {
            Some(p) => p,
            None => continue,
        };

        let dims = &line[..col_pos];
        let mut dim_iter = dims.split('x');
        let w = match dim_iter.next().and_then(|s| s.trim().parse::<i32>().ok()) {
            Some(v) => v,
            None => continue,
        };
        let h = match dim_iter.next().and_then(|s| s.trim().parse::<i32>().ok()) {
            Some(v) => v,
            None => continue,
        };

        let pieces_str = &line[col_pos + 1..];
        let mut total_pieces = 0i32;
        for token in pieces_str.split_whitespace() {
            if let Ok(v) = token.parse::<i32>() {
                total_pieces += v;
            }
        }

        if w * h >= total_pieces * 9 {
            ans += 1;
        }
    }
    ans
}

fn part_two(_content: &str) -> i32 {
    println!("Merry Christmas 2025 !!!");
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
