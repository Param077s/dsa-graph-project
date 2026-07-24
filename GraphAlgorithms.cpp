// Graph Algorithms - DSA Major Project
// Menu driven program to build a weighted graph and run BFS, DFS and Dijkstra on it.
//
// compile: g++ -std=c++17 -o graph GraphAlgorithms.cpp
// run:     ./graph

#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

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

    void addEdge(int u, int v, int w) {
        if (u < 0 || u >= n || v < 0 || v >= n) {
            cout << "  invalid vertex (must be 0 to " << n - 1 << ")\n";
            return;
        }
        adj[u].push_back({v, w});
        if (!directed)
            adj[v].push_back({u, w});   // undirected -> add both directions
        cout << "  added edge " << u << " -> " << v << " (weight " << w << ")\n";
    }

    void display() {
        cout << "\n  Graph:\n";
        for (int v = 0; v < n; v++) {
            cout << "  " << v << " -> ";
            if (adj[v].empty()) cout << "-";
            for (size_t i = 0; i < adj[v].size(); i++) {
                cout << adj[v][i].to << "(w" << adj[v][i].weight << ")";
                if (i + 1 < adj[v].size()) cout << ", ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    // BFS - use a queue, visit level by level
    void bfs(int start) {
        if (start < 0 || start >= n) { cout << "  invalid vertex\n"; return; }

        vector<bool> visited(n, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);

        cout << "\n  BFS from " << start << ": ";
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            cout << cur << " ";
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
        if (start < 0 || start >= n) { cout << "  invalid vertex\n"; return; }

        vector<bool> visited(n, false);
        cout << "\n  DFS from " << start << ": ";
        dfsUtil(start, visited);
        cout << "\n\n";
    }

    // Dijkstra - shortest distance from src to every vertex using a min heap
    void dijkstra(int src) {
        if (src < 0 || src >= n) { cout << "  invalid vertex\n"; return; }

        int INF = numeric_limits<int>::max();
        vector<int> dist(n, INF);
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
                    pq.push({dist[e.to], e.to});
                }
            }
        }

        cout << "\n  Shortest distance from " << src << ":\n";
        for (int v = 0; v < n; v++) {
            cout << "  " << src << " -> " << v << " = ";
            if (dist[v] == INF) cout << "not reachable\n";
            else cout << dist[v] << "\n";
        }
        cout << "\n";
    }

private:
    void dfsUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";
        for (Edge e : adj[v]) {
            if (!visited[e.to])
                dfsUtil(e.to, visited);
        }
    }
};

// read an int, ask again if the user types something wrong
int readInt(string msg) {
    int x;
    while (true) {
        cout << msg;
        if (cin >> x) return x;
        cout << "  please enter a number\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    cout << "=== Graph Algorithms (BFS / DFS / Dijkstra) ===\n\n";

    int n = readInt("Enter number of vertices: ");
    while (n <= 0) {
        cout << "  must be positive\n";
        n = readInt("Enter number of vertices: ");
    }

    int t = readInt("Type (0 = undirected, 1 = directed): ");
    Graph g(n, t == 1);

    cout << "\nVertices are numbered 0 to " << n - 1 << ".\n\n";

    int choice;
    do {
        cout << "-------------------------\n";
        cout << " 1. Add edge\n";
        cout << " 2. Show graph\n";
        cout << " 3. BFS\n";
        cout << " 4. DFS\n";
        cout << " 5. Dijkstra shortest path\n";
        cout << " 0. Exit\n";
        choice = readInt(" choice: ");

        if (choice == 1) {
            int u = readInt("   from: ");
            int v = readInt("   to: ");
            int w = readInt("   weight: ");
            g.addEdge(u, v, w);
        }
        else if (choice == 2) g.display();
        else if (choice == 3) g.bfs(readInt("   start vertex: "));
        else if (choice == 4) g.dfs(readInt("   start vertex: "));
        else if (choice == 5) g.dijkstra(readInt("   source vertex: "));
        else if (choice == 0) cout << "\n  bye\n";
        else cout << "  wrong option\n";

    } while (choice != 0);

    return 0;
}
