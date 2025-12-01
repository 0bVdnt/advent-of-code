# 🎄 Advent of Code Solutions

This repository contains my personal solutions for [Advent of Code](https://adventofcode.com/), implemented in **C++** and **Rust**. It is designed as a scalable engine to manage multiple years of puzzles with automated setup and downloading.

## Repository Structure

The project is structured as a monorepo. Root-level scripts manage generation and downloading, while year-specific folders contain the code.

```text
advent-of-code/
├── .env                 # Session cookie (IGNORED by Git)
├── get_input.sh         # Script to download Inputs & Puzzles
├── new_day.sh           # Script to generate boilerplate code & download
├── README.md
├── aoc-2025/            # Year 2025
│   ├── input/           # Puzzle inputs (IGNORED by Git)
│   ├── puzzles/         # Puzzle descriptions (IGNORED by Git)
│   ├── cpp/             # C++ Project (CMake)
│   └── rust/            # Rust Project (Cargo)
└── aoc-2024/            # Year 2024 (Created automatically)
    └── ...
```

---

## Quick Start (Automation)

This repository includes scripts to automate the boring stuff.

### 1. Configure Session

To download inputs automatically, you need your Advent of Code session cookie.

1.  Log in to [Advent of Code](https://adventofcode.com/).
2.  Open Browser DevTools (F12) -> **Application** -> **Cookies**.
3.  Copy the value of the `session` cookie.
4.  Create a file named `.env` in the root directory:
    ```text
    SESSION=53616c74...your_cookie_here...
    ```

### 2. Generate a New Day

Use the generator script to create files for C++ and Rust, update build configurations, and download the input/puzzle text in one go.

```bash
# Make scripts executable (first time only)
chmod +x new_day.sh get_input.sh

# Download today's puzzle (Defaults to 2025) and create a structure for running programs in C++ and Rust
./new_day.sh 1

# Downlaod a specific year and day
./new_day.sh 2024 2
```

### 3. Download Inputs Only

If you only need to fetch the data without generating code:

```bash
# Download Day 5 for 2025
./get_input.sh 5

# Download Day 5 for 2024
./get_input.sh 2024 5
```

---

## Rust Solutions

### Prerequisites

- **Rust Toolchain**: Install via [rustup.rs](https://rustup.rs).

### How to Run

Rust solutions are located in `aoc-YYYY/rust/src/bin/`. Each day is a separate binary.

1.  Navigate to the specific year:

    ```bash
    cd aoc-2025/rust
    ```

2.  Run a solution:

    ```bash
    # Run Day 1 (Uses default input: ../input/day01.txt)
    cargo run --bin day01

    # Run with a custom input file
    cargo run --bin day01 ../input/test.txt
    ```

---

## C++ Solutions

### Prerequisites

- **C++ Compiler**: GCC, Clang, or MSVC (Must support **C++20**).
- **CMake**: Version 3.10 or higher.

### How to Build & Run

C++ solutions use CMake for building.

1.  Navigate to the specific year:

    ```bash
    cd aoc-2025/cpp
    ```

2.  Build the project (only needed once or when adding new days):

    ```bash
    mkdir -p build
    cd build
    cmake ..
    make
    ```

3.  Run a solution:
    _Note: Run from the `cpp` root folder so relative paths work._

    ```bash
    # Go back to the cpp source root
    cd ..

    # Run Day 1
    ./build/day01

    # Run with custom input
    ./build/day01 ../input/test.txt
    ```

---

## Disclaimer & Copyright

This repository is for educational purposes.

- **Puzzle Inputs:** Not included in the repository.
- **Puzzle Text:** Not republished in the repository.

(These inputs can be directly downloaded to your local repositories using the shell scripts mentioned above or visit the official website directly.)

Please visit [Advent of Code](https://adventofcode.com/) to support the creator, Eric Wastl.
