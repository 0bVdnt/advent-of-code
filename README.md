# Advent of Code Solutions

This repository contains my solutions for [Advent of Code](https://adventofcode.com/), organized by year and language.

## Repository Structure

The project is structured as a monorepo:

```text
advent-of-code/
└── aoc-2025/            # Year 2025
    ├── input/           # Shared input text files
    ├── puzzles/         # Puzzle descriptions (Markdown)
    ├── cpp/             # C++ Solutions (CMake)
    └── rust/            # Rust Solutions (Cargo)
```

---

## Rust Solutions

### Prerequisites

- **Rust Toolchain** (Cargo & rustc). If missing, install via [rustup.rs](https://rustup.rs).

### How to Run

Rust solutions are located in `aoc-2025/rust/src/bin/`. You can run any specific day using the binary name (e.g., `day01`).

1. Navigate to the Rust directory:

   ```bash
   cd aoc-2025/rust
   ```

2. Run a specific day:

   ```bash
   # Run Day 1 (Uses default input: ../input/day01.txt)
   cargo run --bin day01

   # OR run with a custom input file
   cargo run --bin day01 ../input/custom_test.txt
   ```

3. **To add a new day:** Simply create `src/bin/day02.rs`. Cargo detects it automatically.

---

## C++ Solutions

### Prerequisites

- **C++ Compiler** (g++, clang, or MSVC) supporting C++20.
- **CMake** (Version 3.10+).

### How to Build & Run

C++ solutions use CMake for building.

1. Navigate to the C++ directory:

   ```bash
   cd aoc-2025/cpp
   ```

2. Create a build directory and compile:

   ```bash
   mkdir -p build
   cd build
   cmake ..
   make
   ```

3. Run the executable:
   _Note: Run from the `cpp` directory so relative paths to `input/` work correctly, or pass the input file path explicitly._

   ```bash
   # Go back to the cpp root
   cd ..

   # Run Day 1 (Uses default input: ../input/day01.txt)
   ./build/day01

   # OR run with a custom input file
   ./build/day01 ../input/custom_test.txt
   ```

4. **To add a new day:**
   1. Create `src/day02.cpp`.
   2. Add the following line to `CMakeLists.txt`:
      ```cmake
      add_executable(day02 src/day02.cpp)
      ```
   3. Re-run `make` in the build folder.

---

## Inputs & Outputs

- **Inputs:** Place your puzzle inputs in `aoc-2025/input/` named as `day01.txt`, `day02.txt`, etc.
- **Outputs:** Solutions will generate answers in:
  - Rust: `aoc-2025/rust/output/`
  - C++: `aoc-2025/cpp/output/`
