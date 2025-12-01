#!/bin/bash

# Usage:
#   ./new_day.sh <day>          (Defaults to 2025)
#   ./new_day.sh <year> <day>

# --- Configuration ---
DEFAULT_YEAR=2025
# ---------------------

# 1. Argument Parsing
if [ "$#" -eq 1 ]; then
    YEAR=$DEFAULT_YEAR
    DAY=$1
elif [ "$#" -eq 2 ]; then
    YEAR=$1
    DAY=$2
else
    echo "Usage: $0 [<year>] <day>"
    exit 1
fi

PADDED_DAY=$(printf "%02d" $DAY)
BASE_DIR="aoc-$YEAR"

# Paths
CPP_DIR="$BASE_DIR/cpp/src"
CPP_FILE="$CPP_DIR/day${PADDED_DAY}.cpp"
CMAKE_FILE="$BASE_DIR/cpp/CMakeLists.txt"

RUST_DIR="$BASE_DIR/rust/src/bin"
RUST_FILE="$RUST_DIR/day${PADDED_DAY}.rs"
CARGO_FILE="$BASE_DIR/rust/Cargo.toml"

# 2. Ensure Directory Structure Exists
mkdir -p "$CPP_DIR"
mkdir -p "$BASE_DIR/cpp/output"
mkdir -p "$RUST_DIR"
mkdir -p "$BASE_DIR/rust/output"

# ==============================================================================
# C++ SETUP
# ==============================================================================

# Initialize CMakeLists.txt if it doesn't exist (First day of the year)
if [ ! -f "$CMAKE_FILE" ]; then
    echo "Initializing C++ project for $YEAR..."
    cat <<EOT >"$CMAKE_FILE"
cmake_minimum_required(VERSION 3.10)
project(AOC_$YEAR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

EOT
fi

# Create C++ Day File
if [ ! -f "$CPP_FILE" ]; then
    echo "Creating C++ file: $CPP_FILE"
    cat <<EOT >"$CPP_FILE"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const std::string DEFAULT_INPUT = "../input/day${PADDED_DAY}.txt";
const std::string DEFAULT_OUTPUT = "output/day${PADDED_DAY}.txt";

int part_one(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return -1;
    
    std::string line;
    int ans = 0;
    while (std::getline(file, line)) {
        // Logic here
    }
    return ans;
}

int part_two(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return -1;
    
    return 0;
}

int main(int argc, char *argv[]) {
    std::string input_file = (argc > 1) ? argv[1] : DEFAULT_INPUT;
    std::cout << "Reading from: " << input_file << std::endl;

    auto p1 = part_one(input_file);
    auto p2 = part_two(input_file);

    std::cout << "Part 1: " << p1 << std::endl;
    std::cout << "Part 2: " << p2 << std::endl;

    std::ofstream oFile(DEFAULT_OUTPUT);
    if (oFile.is_open()) oFile << p1 << "\n" << p2 << "\n";
    
    return 0;
}
EOT

    # Register executable in CMake
    if ! grep -q "day${PADDED_DAY}" "$CMAKE_FILE"; then
        echo "add_executable(day${PADDED_DAY} src/day${PADDED_DAY}.cpp)" >>"$CMAKE_FILE"
    fi
else
    echo "C++ file already exists."
fi

# ==============================================================================
# RUST SETUP
# ==============================================================================

# Initialize Cargo.toml if it doesn't exist
if [ ! -f "$CARGO_FILE" ]; then
    echo "Initializing Rust project for $YEAR..."
    cat <<EOT >"$CARGO_FILE"
[package]
name = "aoc-$YEAR"
version = "0.1.0"
edition = "2021"

[dependencies]
EOT
fi

# Create Rust Day File
if [ ! -f "$RUST_FILE" ]; then
    echo "Creating Rust file: $RUST_FILE"
    cat <<EOT >"$RUST_FILE"
//! https://adventofcode.com/$YEAR/day/$DAY
use std::env;
use std::fs;
use std::path::Path;

const DEFAULT_INPUT: &str = "../input/day${PADDED_DAY}.txt";
const DEFAULT_OUTPUT: &str = "output/day${PADDED_DAY}.txt";

fn part_one(content: &str) -> i32 {
    0
}

fn part_two(content: &str) -> i32 {
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
EOT
else
    echo "Rust file already exists."
fi

# ==============================================================================
# DOWNLOAD INPUT & PUZZLE
# ==============================================================================
echo "Downloading Input..."
./get_input.sh "$YEAR" "$DAY"

echo "---------------------------------------------------"
echo "Day $DAY ($YEAR) setup complete!"
echo "---------------------------------------------------"
