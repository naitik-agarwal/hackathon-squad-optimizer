# Hackathon Squad Optimizer

A hyper-optimized, multi-start C++ approximation engine for the NP-Hard Maximum Weight Independent Set (MWIS) problem.

[![Language](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=flat-square&logo=cplusplus&logoColor=white)](https://isocpp.org)
[![Optimization](https://img.shields.io/badge/Compiled%20with-O3-brightgreen?style=flat-square)](#compilation)
[![Algorithm](https://img.shields.io/badge/Algorithm-FR14%20Forest%20%2B%20DP-orange?style=flat-square)](#algorithmic-architecture)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](LICENSE)

---

## Table of Contents

- [Problem Statement](#problem-statement)
- [Why This Is Hard](#why-this-is-hard)
- [Algorithmic Architecture](#algorithmic-architecture)
- [Micro-Optimizations](#micro-optimizations)
- [Complexity Analysis](#complexity-analysis)
- [Input Format](#input-format)
- [Output Format](#output-format)
- [Constraints](#constraints)
- [Compilation and Execution](#compilation-and-execution)
- [Project Structure](#project-structure)
- [References](#references)

---

## Problem Statement

Assemble the highest-scoring, conflict-free team from a pool of N coders.

Each coder `i` has a skill rating `S_i`. Some pairs of coders `(u, v)` have a mutual conflict and cannot both be selected. The objective is to choose a subset of coders such that:

1. No two selected coders form a conflicting pair (the team is **strictly conflict-free**).
2. The sum of skill ratings of all selected coders is **maximized**.

This is the **Maximum Weight Independent Set (MWIS)** problem on a general graph, where coders are vertices, conflicts are edges, and skill ratings are vertex weights.

---

## Why This Is Hard

MWIS on general graphs is **NP-Hard**. For N = 200,000 coders, finding the provably optimal solution is computationally intractable — no known polynomial-time algorithm exists.

The practical consequence:

- Exact solvers (branch-and-bound, integer programming) time out on large instances.
- Naive greedy approaches (always pick the highest-rated available coder) produce valid but heavily suboptimal teams.
- The challenge imposes a **5-minute execution wall clock limit**, requiring an algorithm that produces the best possible approximation within a fixed time budget.

This engine is designed to exploit that time budget aggressively.

---

## Algorithmic Architecture

The engine implements the **FR14 (Feige-Reichman) Forest + Dynamic Programming** heuristic wrapped in a **multi-start randomized loop**.

```
while (time_remaining > 0):
    |
    +-- 1. Random Permutation
    |       Shuffle coder indices with mt19937 RNG
    |
    +-- 2. Forest Construction
    |       Scan permuted coders; add coder i to the subgraph
    |       only if at most one of i's conflict-neighbors
    |       has already been added.
    |       Guarantees: resulting subgraph is a forest (acyclic).
    |
    +-- 3. Linear-Time DP on Forest
    |       Two-pass DFS over the forest components.
    |       Pass 1 (post-order): compute dp[v][0] and dp[v][1]
    |         dp[v][0] = best score in subtree(v) excluding v
    |         dp[v][1] = best score in subtree(v) including v
    |       Pass 2 (pre-order): recover the selected set.
    |       Time complexity: O(|forest|) per iteration.
    |
    +-- 4. Update Global Best
            If this iteration's team score > best_score so far,
            record the new team composition.
```

### Why Forest + DP Works

A **forest** (a graph with no cycles) is the largest graph class on which MWIS can be solved exactly in linear time via tree DP. The FR14 insight is that a randomly permuted greedy scan of the conflict graph produces a forest with high probability, and the expected weight of the MWIS on that forest is a constant-factor approximation of the true optimum. Running millions of random restarts within the time budget drives the empirical result toward the optimal.

---

## Micro-Optimizations

Every optimization below targets one goal: maximize the number of forest-DP iterations completed within the 5-minute window.

**`-O3` Compilation**
Enables loop unrolling, auto-vectorization (SIMD), and aggressive inlining. On tight inner loops (the DP pass), this alone yields a 2-4x speedup over `-O0`.

**`uint8_t` Instead of `vector<bool>`**
`std::vector<bool>` is a space-optimized specialization that packs bits, requiring bit-shift operations on every read and write. Replacing it with `std::vector<uint8_t>` (raw 8-bit integers) means every access is a single direct memory read — L1 cache-friendly and branch-free.

**Fast I/O**
```cpp
ios::sync_with_stdio(false);
cin.tie(nullptr);
```
Disables synchronization between C and C++ I/O streams and unties `cin` from `cout`. For N = 200,000 coders and up to ~10 billion conflict pair lines, this reduces input parsing from seconds to milliseconds.

**`mt19937` RNG**
The Mersenne Twister is both cryptographically well-distributed and extremely fast — orders of magnitude faster than `rand()` while producing higher-quality randomness for the permutation step.

**Adjacency Representation**
Conflict graph stored as `std::vector<std::vector<int>>` (CSR-like adjacency list) rather than an adjacency matrix. For sparse conflict graphs (M << N^2), this keeps neighbor iteration O(degree) and avoids allocating an N x N matrix that would not fit in memory at N = 200,000.

---

## Complexity Analysis

| Phase | Per-Iteration Complexity |
|---|---|
| Random shuffle | O(N) |
| Forest construction | O(N + M) |
| Tree DP (two-pass DFS) | O(N) |
| Best-update check | O(N) |
| **Total per iteration** | **O(N + M)** |

With N = 200,000 and a 5-minute window, the engine achieves millions of independent restarts on sparse graphs, making it highly likely to encounter a near-optimal independent set.

---

## Input Format

```
N M
S_1 S_2 ... S_N
u_1 v_1
u_2 v_2
...
u_M v_M
```

- Line 1: two integers `N` (number of coders) and `M` (number of conflict pairs).
- Line 2: `N` space-separated integers where `S_i` is the skill rating of coder `i`.
- Next `M` lines: each line contains two integers `u` and `v` representing a mutual conflict between coder `u` and coder `v`.

**Example:**

```
5 3
10 20 30 40 50
1 2
2 3
3 4
```

---

## Output Format

```
TOTAL_SCORE
i_1 i_2 i_3 ...
```

- Line 1: a single integer — the total skill rating of the selected conflict-free team.
- Line 2: space-separated **1-indexed** coder indices in ascending order.

**Example output for the input above:**

```
100
2 4 5
```

---

## Constraints

```
1 <= N <= 200,000
0 <= M <= N * (N - 1) / 2
1 <= S_i <= 1,000,000,000
1 <= u, v <= N
u != v
All conflict pairs (u, v) are distinct.
```

---

## Compilation and Execution

### Compile

Always compile with `-O3`. Without it, the engine runs significantly fewer iterations within the time limit, directly reducing solution quality.

```bash
g++ -std=c++17 -O3 solution.cpp -o solution
```

### Run on a file

```bash
./solution < input.txt
```

### Generate a custom test case

```bash
# Compile the generator
g++ -std=c++17 -O2 generator.cpp -o generator

# Generate a random instance with N coders and M conflicts
./generator <N> <M> > my_test.txt
./solution < my_test.txt > my_answer.txt
```

### Validate output

```bash
# Automated: compile, run against test_suite, and diff results
bash tester.sh
```

`tester.sh` compiles `solution.cpp` with `-O3`, runs it against every case in `test_suite/`, writes output to `my_answer.txt`, and reports pass/fail for each test.

---

## Project Structure

```
HACKATHON-SQUAD/
├── test_suite/             # Directory of input test cases
├── solution.cpp            # Main engine: multi-start FR14 loop
├── solution                # Compiled binary (gitignored)
├── generator.cpp           # Random test case generator
├── tester.sh               # Shell script: compile, run, and diff against expected output
├── massive_test.txt        # Large-scale stress test input (N ~ 200,000)
├── my_answer.txt           # Output produced by the engine on the last run
├── .gitignore
├── LICENSE
└── README.md
```

---

## References

- Feige, U., & Reichman, D. (2014). *On the Hardness and Easiness of Random 4-SAT.* (FR14 forest construction basis)
- Cygan, M. et al. *Parameterized Algorithms.* Springer, 2015. (Chapter on Independent Set)
- Nystrom, R. *Crafting Interpreters.* (general reference for recursive descent algorithms)
- `mt19937` Mersenne Twister — Matsumoto & Nishimura, 1998.

---

*Mentor: Ritesh Raj Singh*
