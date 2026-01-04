//! https://adventofcode.com/2025/day/11
use std::collections::HashMap;
use std::env;
use std::fs;
use std::path::Path;

const DEFAULT_INPUT: &str = "";
const DEFAULT_OUTPUT: &str = "";

type Graph = HashMap<String, Vec<String>>;
type Memo = HashMap<String, u64>;

fn parse_input(content: &str) -> Graph {
    let mut graph = HashMap::new();
    for line in content.lines() {
        if line.trim().is_empty() {
            continue;
        }
        if let Some((src, dests)) = line.split_once(':') {
            let src = src.trim().to_string();
            let neighbors: Vec<String> = dests.split_whitespace().map(|s| s.to_string()).collect();
            graph.insert(src, neighbors);
        }
    }
    graph
}

fn count_paths(curr: &str, target: &str, graph: &Graph, memo: &mut Memo) -> u64 {
    if curr == target {
        return 1;
    }
    if let Some(&count) = memo.get(curr) {
        return count;
    }

    let mut total = 0;
    if let Some(neighbors) = graph.get(curr) {
        for next in neighbors {
            total += count_paths(next, target, graph, memo);
        }
    }

    memo.insert(curr.to_string(), total);
    total
}

fn get_paths(start: &str, end: &str, graph: &Graph) -> u64 {
    let mut memo = HashMap::new();
    count_paths(start, end, graph, &mut memo)
}

fn part_one(content: &str) -> u64 {
    let graph = parse_input(content);
    get_paths("you", "out", &graph)
}

fn part_two(content: &str) -> u64 {
    let graph = parse_input(content);

    let path_dac_fft = get_paths("svr", "dac", &graph)
        * get_paths("dac", "fft", &graph)
        * get_paths("fft", "out", &graph);

    let path_fft_dac = get_paths("svr", "fft", &graph)
        * get_paths("fft", "dac", &graph)
        * get_paths("dac", "out", &graph);

    path_dac_fft + path_fft_dac
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
