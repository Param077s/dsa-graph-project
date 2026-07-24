# Graph Algorithms - DSA Major Project

A menu driven C++ program to build a weighted graph and run three graph
algorithms on it.

## Algorithms
- **BFS** (Breadth First Search) - uses a queue, visits level by level
- **DFS** (Depth First Search) - recursive
- **Dijkstra** - shortest path from a source vertex using a min heap

The graph is stored as a weighted adjacency list. You can build any graph at
runtime and pick directed or undirected at the start.

## Compile and run
```
g++ -std=c++17 -o graph GraphAlgorithms.cpp
./graph
```

## Example
A small 5 vertex undirected graph:

```
edges: 0-1 (5), 0-2 (3), 1-2 (1), 1-3 (7), 2-4 (2)
```

Output:
- BFS from 3  -> `3 1 0 2 4`
- DFS from 4  -> `4 2 0 1 3`
- Dijkstra from 0 -> `0->1 = 4`, `0->2 = 3`, `0->3 = 11`, `0->4 = 5`

## Menu
```
1. Add edge (from, to, weight)
2. Show graph
3. BFS
4. DFS
5. Dijkstra shortest path
0. Exit
```

Vertices are numbered 0 to n-1. Dijkstra assumes non negative weights.
