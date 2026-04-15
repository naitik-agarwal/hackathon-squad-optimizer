#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  ofstream out("massive_test.txt");

  int traps = 10000;
  int nodes_per_trap = 20;      // 5 in Group A, 15 in Group B
  int edges_per_trap = 5 * 15;  // 75 edges

  int N = traps * nodes_per_trap;
  int M = traps * edges_per_trap;

  out << N << " " << M << "\n";

  // Print Skill Ratings
  for (int t = 0; t < traps; t++) {
    // Group A (The Greedy Bait - 5 nodes)
    out << "1000000 900000 900000 900000 900000 ";
    // Group B (The Optimal Choice - 15 nodes)
    for (int i = 0; i < 15; i++) {
      out << "310000" << (t == traps - 1 && i == 14 ? "" : " ");
    }
  }
  out << "\n";

  // Print Conflicts (Bipartite connection: Every A connects to every B)
  int current_node = 1;
  for (int t = 0; t < traps; t++) {
    int start_A = current_node;
    int start_B = current_node + 5;

    for (int a = 0; a < 5; a++) {
      for (int b = 0; b < 15; b++) {
        out << (start_A + a) << " " << (start_B + b) << "\n";
      }
    }
    current_node += nodes_per_trap;
  }

  out.close();
  cout << "Generated massive_test.txt with N=" << N << " and M=" << M << "\n";
  return 0;
}