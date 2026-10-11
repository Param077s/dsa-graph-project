# Graph Algorithms Toolkit - DSA Major Project

A menu driven C++ program that builds a weighted graph and runs a whole set of
classic graph algorithms on it, with colored terminal output.

## Features
- **BFS** (Breadth First Search) - queue based, level by level
- **DFS** (Depth First Search) - recursive
- **Dijkstra** - shortest path from a source, shows the cost *and* the actual route
- **Minimum Spanning Tree** - Prim's algorithm with total weight
- **Cycle detection** - works for both directed and undirected graphs
- **Connected components** - finds how many separate pieces the graph has
- **Load sample graph** - one key press to load a ready made example for demos

The graph is stored as a weighted adjacency list. You can build any graph at
runtime or just load the sample, and pick directed or undirected at the start.

## Compile and run
```
g++ -std=c++17 -o graph GraphAlgorithms.cpp
./graph
```

## Interactive browser demo
For a presentation-friendly explanation without terminal input, open
[`graph-algorithms-visualizer.html`](graph-algorithms-visualizer.html) in any
browser. It uses the same sample graph and explains what each algorithm does,
when to use it, and how it compares with the other algorithms.

## Quick demo
Start it, choose undirected, then pick **1** to load the sample graph and try
the other options. On the sample graph you get:

- BFS from 0  -> `0 1 2 3 4 5`
- DFS from 0  -> `0 1 2 4 3 5`
- Dijkstra from 0 -> `0->3` costs 7 by the path `0 -> 1 -> 2 -> 4 -> 3`
- MST total weight = 9
- Cycle check -> a cycle exists
- Connected components -> 1 group

## Menu
```
1. Load sample graph
2. Add edge (from, to, weight)
3. Show graph
4. BFS traversal
5. DFS traversal
6. Dijkstra shortest path
7. Minimum spanning tree
8. Detect cycle
9. Connected components
0. Exit
```

Vertices are numbered 0 to n-1. Dijkstra and Prim's MST assume non negative
weights. MST needs an undirected graph.
