//! https://adventofcode.com/2025/day/10
use std::env;
use std::fs;
use std::path::Path;

const DEFAULT_INPUT: &str = "../input/day10.txt";
const DEFAULT_OUTPUT: &str = "output/day10.txt";
const EPS: f64 = 1e-9;

#[derive(Debug, Clone)]
struct Machine {
    buttons: Vec<Vec<usize>>,
    targets: Vec<i64>,
    light_count: usize,
}

struct EnumerateContext<'a> {
    free_cols: &'a [usize],
    aug: &'a [Vec<f64>],
    pivot_cols: &'a [i32],
    rank: usize,
    nb: usize,
    nc: usize,
    targets: &'a [i64],
    buttons: &'a [Vec<usize>],
}

fn parse_line(line: &str) -> Machine {
    let mut m = Machine {
        buttons: Vec::new(),
        targets: Vec::new(),
        light_count: 0,
    };

    let ls = line.find('[').unwrap();
    let le = line.find(']').unwrap();
    m.light_count = le - ls - 1;

    let mut pos = le + 1;
    let chars: Vec<char> = line.chars().collect();
    while pos < chars.len() && chars[pos] != '{' {
        if chars[pos] == '(' {
            let end = line[pos..].find(')').unwrap() + pos;
            let btn_str = &line[pos + 1..end];
            let btn: Vec<usize> = btn_str
                .split(',')
                .filter(|s| !s.is_empty())
                .map(|s| s.trim().parse().unwrap())
                .collect();
            m.buttons.push(btn);
            pos = end + 1;
        } else {
            pos += 1;
        }
    }

    let ts = line.find('{').unwrap();
    let te = line.find('}').unwrap();
    let targets_str = &line[ts + 1..te];
    m.targets = targets_str
        .split(',')
        .filter(|s| !s.is_empty())
        .map(|s| s.trim().parse().unwrap())
        .collect();

    m
}

fn solve_part1(line: &str, m: &Machine) -> i32 {
    let ls = line.find('[').unwrap();
    let lights: Vec<bool> = line[ls + 1..ls + 1 + m.light_count]
        .chars()
        .map(|c| c == '#')
        .collect();

    let nb = m.buttons.len();
    let mut mini = i32::MAX;

    for mask in 0..(1 << nb) {
        let presses = (mask as u32).count_ones() as i32;
        if presses >= mini {
            continue;
        }

        let mut curr = vec![false; m.light_count];
        for (b, button) in m.buttons.iter().enumerate() {
            if (mask & (1 << b)) != 0 {
                for &idx in button {
                    if idx < m.light_count {
                        curr[idx] = !curr[idx];
                    }
                }
            }
        }

        let fine = curr.iter().zip(lights.iter()).all(|(&c, &l)| c == l);
        if fine {
            mini = presses;
        }
    }

    if mini == i32::MAX {
        0
    } else {
        mini
    }
}

fn solve_part2(m: &Machine) -> i64 {
    let nc = m.targets.len();
    let nb = m.buttons.len();

    if nc == 0 || nb == 0 {
        return 0;
    }

    let mut aug: Vec<Vec<f64>> = vec![vec![0.0; nb + 1]; nc];
    for (bi, btn) in m.buttons.iter().enumerate() {
        for &idx in btn {
            if idx < nc {
                aug[idx][bi] = 1.0;
            }
        }
    }
    for (i, row) in aug.iter_mut().enumerate().take(nc) {
        row[nb] = m.targets[i] as f64;
    }

    let mut pivot_cols: Vec<i32> = vec![-1; nc];
    let mut rank = 0;

    for col in 0..nb {
        if rank >= nc {
            break;
        }

        let mut pivot: Option<usize> = None;
        for (row, aug_row) in aug.iter().enumerate().take(nc).skip(rank) {
            if aug_row[col].abs() > EPS {
                pivot = Some(row);
                break;
            }
        }

        let pivot = match pivot {
            Some(p) => p,
            None => continue,
        };

        aug.swap(rank, pivot);
        pivot_cols[rank] = col as i32;

        let piv = aug[rank][col];
        for val in aug[rank].iter_mut().take(nb + 1) {
            *val /= piv;
        }

        for row in 0..nc {
            if row != rank && aug[row][col].abs() > EPS {
                let fact = aug[row][col];
                for j in 0..=nb {
                    aug[row][j] -= fact * aug[rank][j];
                }
            }
        }
        rank += 1;
    }

    for row in aug.iter().take(nc).skip(rank) {
        if row[nb].abs() > EPS {
            return 0;
        }
    }

    let mut is_pivot = vec![false; nb];
    for i in 0..rank {
        if pivot_cols[i] >= 0 {
            is_pivot[pivot_cols[i] as usize] = true;
        }
    }

    let free_cols: Vec<usize> = (0..nb).filter(|&j| !is_pivot[j]).collect();
    let free_cnt = free_cols.len();

    let mut mini: i64 = i64::MAX;
    let mut free_vals: Vec<i64> = vec![0; free_cnt];

    let ctx = EnumerateContext {
        free_cols: &free_cols,
        aug: &aug,
        pivot_cols: &pivot_cols,
        rank,
        nb,
        nc,
        targets: &m.targets,
        buttons: &m.buttons,
    };

    enumerate(0, free_cnt, &mut free_vals, &ctx, &mut mini);

    if mini == i64::MAX {
        0
    } else {
        mini
    }
}

fn enumerate(
    idx: usize,
    free_cnt: usize,
    free_vals: &mut Vec<i64>,
    ctx: &EnumerateContext,
    mini: &mut i64,
) {
    if idx == free_cnt {
        let mut solution: Vec<i64> = vec![0; ctx.nb];
        for (i, &col) in ctx.free_cols.iter().enumerate() {
            solution[col] = free_vals[i];
        }

        let mut total: i64 = free_vals.iter().sum();
        if total >= *mini {
            return;
        }

        for i in 0..ctx.rank {
            let pcol = ctx.pivot_cols[i] as usize;
            let mut val = ctx.aug[i][ctx.nb];
            for (j, &sol_j) in solution.iter().enumerate() {
                if j != pcol {
                    val -= ctx.aug[i][j] * sol_j as f64;
                }
            }
            let ival = val.round() as i64;
            if (val - ival as f64).abs() > EPS || ival < 0 {
                return;
            }
            solution[pcol] = ival;
            total += ival;
            if total >= *mini {
                return;
            }
        }
        *mini = total;
        return;
    }

    let mut max_val: i64 = i64::MAX;
    for &btn_idx in &ctx.buttons[ctx.free_cols[idx]] {
        if btn_idx < ctx.nc {
            max_val = max_val.min(ctx.targets[btn_idx]);
        }
    }
    if max_val == i64::MAX {
        max_val = 0;
    } else if max_val > 500 {
        max_val = 500;
    }

    for v in 0..=max_val {
        free_vals[idx] = v;
        enumerate(idx + 1, free_cnt, free_vals, ctx, mini);
    }
}

fn part_one(content: &str) -> i32 {
    let mut ans = 0;
    for line in content.lines() {
        if !line.is_empty() {
            let m = parse_line(line);
            ans += solve_part1(line, &m);
        }
    }
    ans
}

fn part_two(content: &str) -> i64 {
    let mut ans: i64 = 0;
    for line in content.lines() {
        if !line.is_empty() {
            let m = parse_line(line);
            ans += solve_part2(&m);
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
