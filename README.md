# Hackathon Squad Optimizer

> A high-performance C++ approximation engine designed to tackle the NP-Hard Maximum Weight Independent Set (MWIS) problem on general graphs.

## About the Project

Finding an exact solution for the Maximum Weight Independent Set on general graphs is mathematically intractable for large datasets. This project was built to bypass that limitation under strict competitive constraints (e.g., $N=200,000$ nodes, 5-minute execution limit).

This engine employs a highly optimized heuristic approach, specifically implementing the **FR14 (Feige-Reichman) Forest + Dynamic Programming algorithm**. By rigorously executing millions of random permutations, the algorithm rapidly constructs acyclic subsets (forests) and computes their optimal independent sets in linear time, guaranteeing a mathematically proven lower bound score.

## Key Features

- **Multi-Start Heuristic Loop:** Weaponizes time limits by executing thousands of random permutations to brute-force probabilistic advantages.
- **Forest Generation:** Dynamically filters graph nodes to guarantee acyclic (tree) structures.
- **Linear-Time DP:** Extracts the true Maximum Weight Independent Set from the generated forests in $O(N)$ time.
- **Hyper-Optimized:** Built with C++17, Fast I/O, `mt19937` RNG, and flattened data structures to ensure maximum loop iterations within the time window.

## Execution & Setup

### 1. Compile

To achieve the millions of iterations required for optimal scoring, compile the engine with the maximum speed optimization flag (`-O3`):

```bash
g++ -std=c++17 -O3 solution.cpp -o solution
```
