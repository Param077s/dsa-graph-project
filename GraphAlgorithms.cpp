// Graph Algorithms - DSA Major Project
// Build a weighted graph and run several classic graph algorithms on it:
//   BFS, DFS, Dijkstra (with path), Minimum Spanning Tree, cycle detection,
//   and connected components. Uses colored output to make things readable.
//
// compile: g++ -std=c++17 -o graph GraphAlgorithms.cpp
// run:     ./graph

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

using namespace std;

// terminal colors (ANSI escape codes)
const string RESET = "\033[0m";
const string BOLD   = "\033[1m";
const string CYAN   = "\033[36m";
const string GREEN  = "\033[32m";
const string YELLOW = "\033[33m";
const string RED    = "\033[31m";
const string BLUE   = "\033[34m";
const string GRAY   = "\033[90m";

// one edge = neighbour vertex + weight
struct Edge {
    int to;
    int weight;
};

class Graph {
    int n;                          // number of vertices
    bool directed;
    vector<vector<Edge>> adj;       // adjacency list

public:
    Graph(int vertices, bool isDirected) {
        n = vertices;
        directed = isDirected;
        adj.resize(n);
    }

    int size() const { return n; }

    void addEdge(int u, int v, int w, bool quiet = false) {
        if (u < 0 || u >= n || v < 0 || v >= n) {
            cout << RED << "  invalid vertex (must be 0 to " << n - 1 << ")\n" << RESET;
            return;
        }
        adj[u].push_back({v, w});
        if (!directed)
            adj[v].push_back({u, w});   // undirected -> add both directions
        if (!quiet)
            cout << GREEN << "  added edge " << u << " -> " << v
                 << " (weight " << w << ")\n" << RESET;
    }

    // wipes the current graph and loads a ready made example so it is easy to demo
    void loadSample() {
        n = 6;
        directed = false;
        adj.clear();
        adj.resize(n);
        addEdge(0, 1, 2, true);
        addEdge(0, 2, 4, true);
        addEdge(1, 2, 1, true);
        addEdge(1, 3, 7, true);
        addEdge(2, 4, 3, true);
        addEdge(3, 4, 1, true);
        addEdge(3, 5, 5, true);
        addEdge(4, 5, 2, true);
        cout << GREEN << "  loaded sample graph (6 vertices, undirected)\n" << RESET;
    }

    void display() {
        cout << "\n" << BOLD << "  Graph" << RESET
             << GRAY << "  (" << (directed ? "directed" : "undirected") << ")\n" << RESET;
        for (int v = 0; v < n; v++) {
            cout << "  " << CYAN << v << RESET << " -> ";
            if (adj[v].empty()) cout << GRAY << "-" << RESET;
            for (size_t i = 0; i < adj[v].size(); i++) {
                cout << adj[v][i].to << YELLOW << "(w" << adj[v][i].weight << ")" << RESET;
                if (i + 1 < adj[v].size()) cout << ", ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    // BFS - use a queue, visit level by level
    void bfs(int start) {
        if (!valid(start)) return;

        vector<bool> visited(n, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);

        cout << "\n  " << BOLD << "BFS" << RESET << " from " << CYAN << start << RESET << ": ";
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            cout << GREEN << cur << " " << RESET;
            for (Edge e : adj[cur]) {
                if (!visited[e.to]) {
                    visited[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        cout << "\n\n";
    }

    // DFS - recursive
    void dfs(int start) {
        if (!valid(start)) return;

        vector<bool> visited(n, false);
        cout << "\n  " << BOLD << "DFS" << RESET << " from " << CYAN << start << RESET << ": ";
        dfsUtil(start, visited);
        cout << "\n\n";
    }

    // Dijkstra - shortest distance AND the actual path from src to every vertex
    void dijkstra(int src) {
        if (!valid(src)) return;

        int INF = numeric_limits<int>::max();
        vector<int> dist(n, INF);
        vector<int> parent(n, -1);      // to rebuild the path later
        dist[src] = 0;

        // min heap of (distance, vertex)
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (d > dist[u]) continue;  // old outdated entry, skip it

            for (Edge e : adj[u]) {
                if (dist[u] + e.weight < dist[e.to]) {
                    dist[e.to] = dist[u] + e.weight;
                    parent[e.to] = u;
                    pq.push({dist[e.to], e.to});
                }
            }
        }

        cout << "\n  " << BOLD << "Dijkstra" << RESET << " shortest paths from "
             << CYAN << src << RESET << ":\n";
        for (int v = 0; v < n; v++) {
            cout << "  " << src << " -> " << v << " : ";
            if (dist[v] == INF) {
                cout << RED << "not reachable\n" << RESET;
            } else {
                cout << GREEN << "cost " << dist[v] << RESET << GRAY << "   path " << RESET;
                printPath(parent, v);
                cout << "\n";
            }
        }
        cout << "\n";
    }

    // Minimum Spanning Tree using Prim's algorithm (undirected graphs)
    void mst() {
        if (directed) {
            cout << RED << "  MST needs an undirected graph\n" << RESET;
            return;
        }

        vector<bool> inTree(n, false);
        int total = 0, edgesUsed = 0;

        // min heap of (weight, to, from)
        priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>,
                       greater<tuple<int,int,int>>> pq;
        pq.push({0, 0, -1});    // start from vertex 0, no parent

        cout << "\n  " << BOLD << "Minimum Spanning Tree" << RESET << " (Prim):\n";
        while (!pq.empty() && edgesUsed < n) {
            auto [w, u, from] = pq.top();
            pq.pop();

            if (inTree[u]) continue;
            inTree[u] = true;

            if (from != -1) {
                cout << "    " << from << " - " << u
                     << YELLOW << "  (weight " << w << ")\n" << RESET;
                total += w;
                edgesUsed++;
            }

            for (Edge e : adj[u])
                if (!inTree[e.to])
                    pq.push({e.weight, e.to, u});
        }

        if (edgesUsed == n - 1)
            cout << GREEN << "  total weight = " << total << "\n\n" << RESET;
        else
            cout << RED << "  graph is not connected, no full spanning tree\n\n" << RESET;
    }

    // does the graph contain a cycle?
    void detectCycle() {
        vector<bool> visited(n, false);
        bool found = false;

        if (directed) {
            vector<bool> inStack(n, false);
            for (int v = 0; v < n && !found; v++)
                if (!visited[v] && cycleDirected(v, visited, inStack))
                    found = true;
        } else {
            for (int v = 0; v < n && !found; v++)
                if (!visited[v] && cycleUndirected(v, -1, visited))
                    found = true;
        }

        cout << "\n  " << BOLD << "Cycle check" << RESET << ": ";
        if (found) cout << YELLOW << "a cycle exists\n\n" << RESET;
        else       cout << GREEN << "no cycle (graph is acyclic)\n\n" << RESET;
    }

    // how many separate pieces is the graph made of?
    void components() {
        vector<bool> visited(n, false);
        int count = 0;

        cout << "\n  " << BOLD << "Connected components" << RESET << ":\n";
        for (int v = 0; v < n; v++) {
            if (!visited[v]) {
                count++;
                cout << "    group " << count << ": ";
                // simple BFS to collect this piece
                queue<int> q;
                q.push(v);
                visited[v] = true;
                while (!q.empty()) {
                    int cur = q.front();
                    q.pop();
                    cout << CYAN << cur << " " << RESET;
                    for (Edge e : adj[cur])
                        if (!visited[e.to]) {
                            visited[e.to] = true;
                            q.push(e.to);
                        }
                }
                cout << "\n";
            }
        }
        cout << GREEN << "  total groups = " << count << "\n\n" << RESET;
    }

private:
    bool valid(int v) {
        if (v < 0 || v >= n) {
            cout << RED << "  invalid vertex\n" << RESET;
            return false;
        }
        return true;
    }

    void dfsUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << GREEN << v << " " << RESET;
        for (Edge e : adj[v])
            if (!visited[e.to])
                dfsUtil(e.to, visited);
    }

    void printPath(vector<int>& parent, int v) {
        vector<int> path;
        for (int at = v; at != -1; at = parent[at])
            path.push_back(at);
        reverse(path.begin(), path.end());
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i + 1 < path.size()) cout << " -> ";
        }
    }

    bool cycleUndirected(int v, int parent, vector<bool>& visited) {
        visited[v] = true;
        for (Edge e : adj[v]) {
            if (!visited[e.to]) {
                if (cycleUndirected(e.to, v, visited)) return true;
            } else if (e.to != parent) {
                return true;    // reached an already visited node that is not where we came from
            }
        }
        return false;
    }

    bool cycleDirected(int v, vector<bool>& visited, vector<bool>& inStack) {
        visited[v] = true;
        inStack[v] = true;
        for (Edge e : adj[v]) {
            if (!visited[e.to]) {
                if (cycleDirected(e.to, visited, inStack)) return true;
            } else if (inStack[e.to]) {
                return true;    // back edge -> cycle
            }
        }
        inStack[v] = false;
        return false;
    }
};

// read an int, ask again if the user types something wrong
int readInt(string msg) {
    int x;
    while (true) {
        cout << msg;
        if (cin >> x) return x;
        cout << RED << "  please enter a number\n" << RESET;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    cout << BOLD << CYAN
         << "\n  ===== Graph Algorithms Toolkit =====\n" << RESET
         << GRAY << "  BFS . DFS . Dijkstra . MST . cycles . components\n\n" << RESET;

    int n = readInt("Enter number of vertices: ");
    while (n <= 0) {
        cout << RED << "  must be positive\n" << RESET;
        n = readInt("Enter number of vertices: ");
    }

    int t = readInt("Type (0 = undirected, 1 = directed): ");
    Graph g(n, t == 1);

    cout << GRAY << "\n  tip: pick option 1 to load a ready made example graph.\n" << RESET;

    int choice;
    do {
        cout << BLUE << "\n  ---------------------------------\n" << RESET;
        cout << "   1. Load sample graph\n";
        cout << "   2. Add edge\n";
        cout << "   3. Show graph\n";
        cout << "   4. BFS traversal\n";
        cout << "   5. DFS traversal\n";
        cout << "   6. Dijkstra shortest path\n";
        cout << "   7. Minimum spanning tree\n";
        cout << "   8. Detect cycle\n";
        cout << "   9. Connected components\n";
        cout << "   0. Exit\n";
        cout << BLUE << "  ---------------------------------\n" << RESET;
        choice = readInt("  choice: ");

        switch (choice) {
            case 1: g.loadSample(); break;
            case 2: {
                int u = readInt("   from: ");
                int v = readInt("   to: ");
                int w = readInt("   weight: ");
                g.addEdge(u, v, w);
                break;
            }
            case 3: g.display(); break;
            case 4: g.bfs(readInt("   start vertex: ")); break;
            case 5: g.dfs(readInt("   start vertex: ")); break;
            case 6: g.dijkstra(readInt("   source vertex: ")); break;
            case 7: g.mst(); break;
            case 8: g.detectCycle(); break;
            case 9: g.components(); break;
            case 0: cout << GRAY << "\n  bye\n\n" << RESET; break;
            default: cout << RED << "  wrong option\n" << RESET;
        }
    } while (choice != 0);

    return 0;
}
