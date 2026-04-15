# 🏆 Hackathon Squad Optimizer

> [cite_start]A hyper-optimized, multi-start C++ approximation engine designed to solve the NP-Hard Maximum Weight Independent Set (MWIS) problem[cite: 865].

## 📖 The Problem Statement

The goal of the "Hackathon Squad" challenge is to assemble the ultimate team of coders. [cite_start]We are given $N$ coders, each with a specific "skill rating", and $M$ "conflicting pairs" of coders who refuse to work together.

[cite_start]The objective is to select a strictly conflict-free team that mathematically maximizes the total combined skill rating[cite: 35]. [cite_start]Because this maps directly to the Maximum Weight Independent Set (MWIS) problem on general graphs, finding the absolute perfect team is computationally intractable for large datasets (e.g., $N=200,000$ coders)[cite: 247, 248, 865].

## 🧠 Algorithmic Architecture

[cite_start]To bypass the NP-Hard constraints and beat standard greedy algorithms, this engine implements the **FR14 (Feige-Reichman) Forest + Dynamic Programming algorithm**[cite: 866].

[cite_start]By weaponizing a strict 5-minute execution window[cite: 867], the engine runs a multi-start heuristic loop:

1. [cite_start]**Random Permutation:** Coders are rapidly shuffled using the `mt19937` RNG[cite: 868].
2. **Forest Generation:** The engine builds a subset graph by exclusively adding coders if at most one of their conflict-neighbors has been seen so far. [cite_start]This mathematically guarantees the resulting graph is a completely acyclic forest[cite: 303, 304].
3. [cite_start]**Linear-Time DP:** The engine runs a two-pass Depth-First Search (DFS) Dynamic Programming algorithm to extract the exact Maximum Weight Independent Set from the generated forest in $O(N)$ time[cite: 305, 520, 525].
4. [cite_start]**Time-Bound Iteration:** This process repeats millions of times, evaluating randomly generated forests to brute-force a probabilistically near-optimal team before the clock runs out[cite: 867].

## ⚡ Tech Stack & Micro-Optimizations

- [cite_start]**Language:** C++17 [cite: 868]
- **Speed Optimizations:** Compiled with `-O3` flags for maximum CPU instruction efficiency.
- **Memory Flattening:** Replaced inherently slow `vector<bool>` bit-shifting with raw 8-bit integers (`uint8_t`) to allow direct L1 Cache memory access and drastically increase the while-loop iteration speed.
- [cite_start]**Fast I/O:** Implemented `ios::sync_with_stdio(false)` for instantaneous parsing of massive input files[cite: 266, 868].

## ⚙️ Execution & Setup

### 1. Compile

To achieve the millions of iterations required for optimal scoring, compile the engine with the maximum speed optimization flag:

```bash
g++ -std=c++17 -O3 solution.cpp -o solution
```
