#include <bits/stdc++.h>

#include <chrono>

using namespace std;

// ==========================================
// GLOBALS FOR FAST DYNAMIC PROGRAMMING
// ==========================================
const int MAXN = 200005;  // Max N is 200,000
long long dp_include[MAXN];
long long dp_exclude[MAXN];
bool visited[MAXN];

// DFS 1: Calculate the maximum possible weights (from leaves to root)
void dfs_score(int u, int parent, const vector<vector<int>>& adj,
               const vector<bool>& in_F, const vector<long long>& S) {
  visited[u] = true;

  // Case 1 gets the coder's skill, Case 2 gets 0
  dp_include[u] = S[u];
  dp_exclude[u] = 0;

  for (int v : adj[u]) {
    // Only traverse neighbors that actually made it into our Forest F
    if (in_F[v] && v != parent) {
      dfs_score(v, u, adj, in_F, S);

      // Case 1 (Include u): We CANNOT include child v. Add dp_exclude[v] (which
      // inherently includes grandchildren).
      dp_include[u] += dp_exclude[v];

      // Case 2 (Exclude u): We CAN include OR exclude child v. Greedily pick
      // whichever is bigger.
      dp_exclude[u] += max(dp_include[v], dp_exclude[v]);
    }
  }
}

// DFS 2: Backtrack to extract the actual team members (from root to leaves)
void dfs_recover(int u, int parent, bool parent_included,
                 const vector<vector<int>>& adj, const vector<bool>& in_F,
                 vector<int>& current_team) {
  bool include_u = false;

  if (parent_included) {
    // If the parent is on the team, u CANNOT be on the team (Conflict rule)
    include_u = false;
  } else {
    // If parent is not on the team, pick u ONLY IF it resulted in a higher
    // score
    include_u = (dp_include[u] > dp_exclude[u]);
  }

  if (include_u) {
    current_team.push_back(u);
  }

  for (int v : adj[u]) {
    if (in_F[v] && v != parent) {
      // Pass down whether 'u' was included so 'v' knows if it's allowed to join
      dfs_recover(v, u, include_u, adj, in_F, current_team);
    }
  }
}

// ==========================================
// MAIN EXECUTION
// ==========================================
int main() {
  // Fast I/O
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  int N, M;
  if (!(cin >> N >> M)) return 0;  // Read N and M [cite: 277]

  vector<long long> S(N + 1);
  for (int i = 1; i <= N; i++) {
    cin >> S[i];  // Read skill ratings [cite: 278]
  }

  vector<vector<int>> adj(N + 1);
  for (int i = 0; i < M; i++) {
    int u, v;
    cin >> u >> v;  // Read conflicts [cite: 279]
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  // Timer setup: Stop at 4 minutes 50 seconds (290 seconds) to be safe
  auto start_time = chrono::high_resolution_clock::now();
  double time_limit = 290.0;

  // Fast Random Number Generator
  mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

  // Tracking the best ultimate team across all simulations
  long long best_total_score = -1;
  vector<int> best_team;

  // Array to hold our permutation of coders (1 to N)
  vector<int> coders(N);
  iota(coders.begin(), coders.end(), 1);

  // Pre-allocate tracking arrays outside the loop to save massive amounts of
  // time
  vector<bool> seen(N + 1, false);
  vector<bool> in_F(N + 1, false);
  vector<int> F;
  F.reserve(N);

  // === THE MULTI-START LOOP ===
  while (true) {
    // 1. Check clock
    auto current_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = current_time - start_time;
    if (elapsed.count() > time_limit) break;  // TIME IS UP!

    // 2. Compute uniform random permutation [cite: 814]
    shuffle(coders.begin(), coders.end(), rng);

    // 3. Fast Reset of state arrays
    F.clear();
    for (int v : coders) {
      seen[v] = false;
      in_F[v] = false;
    }

    // 4. Build the Forest F (The Magic Rule) [cite: 884]
    for (int v : coders) {
      int seen_neighbors_count = 0;

      for (int neighbor : adj[v]) {
        if (seen[neighbor]) {
          seen_neighbors_count++;
        }
      }

      // At most 1 neighbor seen so far [cite: 884]
      if (seen_neighbors_count <= 1) {
        F.push_back(v);
        in_F[v] = true;
      }

      seen[v] = true;
    }

    // 5. RUN DP ON 'F' TO GET THE BEST TEAM [cite: 887, 909]
    long long current_score = 0;
    vector<int> current_team;

    // Reset the visited array ONLY for nodes in F (Massive speedup)
    for (int v : F) visited[v] = false;

    // A forest can be made of multiple disconnected trees.
    // We need to run the DP on the root of each independent tree.
    for (int root : F) {
      if (!visited[root]) {
        // Calculate max scores for this specific tree [cite: 912]
        dfs_score(root, 0, adj, in_F, S);

        // Add the best possible outcome of this tree to our total run score
        current_score += max(dp_include[root], dp_exclude[root]);

        // Extract the actual coders who formed that score [cite: 972]
        dfs_recover(root, 0, false, adj, in_F, current_team);
      }
    }

    // 6. Check if this random forest yielded a better team
    if (current_score > best_total_score) {
      best_total_score = current_score;
      best_team = current_team;
    }
  }

  // === FORMAT AND PRINT OUTPUT ===
  // Sort indices in ascending order [cite: 286]
  sort(best_team.begin(), best_team.end());

  // Line 1: Total Score [cite: 284]
  cout << best_total_score << "\n";

  // Line 2: Space-separated IDs [cite: 286]
  //   for (int i = 0; i < best_team.size(); i++) {
  //     cout << best_team[i] << (i == best_team.size() - 1 ? "" : " ");
  //   }
  //   cout << "\n";

  return 0;
}