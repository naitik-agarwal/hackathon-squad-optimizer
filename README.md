## About the Project

[cite_start]The **Hackathon Squad Optimizer** is a high-performance C++ implementation designed to tackle the NP-Hard Maximum Weight Independent Set (MWIS) problem[cite: 859].
[cite_start]Because finding an exact solution for MWIS on general graphs is computationally intractable for large datasets, this engine employs a highly optimized heuristic approach[cite: 859]. [cite_start]It implements the **FR14 Forest + Dynamic Programming algorithm**[cite: 860].

[cite_start]By rigorously executing millions of random permutations, the algorithm rapidly constructs acyclic subsets (forests) and computes their optimal independent sets in linear time[cite: 860]. [cite_start]It is specifically engineered to maximize the selected node weights within a strict 5-minute execution window [cite: 861][cite_start], utilizing C++17, Fast I/O, and `mt19937` Random Number Generation[cite: 862].
