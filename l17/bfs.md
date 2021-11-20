# Breadth-First Search
Simplest algorithm for graph searching. It works by calculating the smallest edge distance of every vertex reachable from a source vertex.

The BFS algorithm generates a BF-Tree that contains all reachables vertices. Works on **directed** and **undirected** graphs.

### What is Breadth
Breadth is the *width* or *broadness* a graph. A algorithm based on breath will prioritize reaching each vtx in a same distance from the base.

```
    (0)
   / | \
(1) (1) (1)
 |   |
(2) (2)
 |
(3)
```

### How the algorithm works?
BFS works by building a **BF-tree**.

**Discovery** is tracked by *colloring* each vertex with white, gray or black.
```
White: undiscovered
Gray:  discovered - adjacent to black or white vertices (frontier between black and white)
Black: discovered - adjacent to black vertices
```
*Is actually unecessary to distinguish between black and gray: only one bit is necessary to keep track of discovery*

Each time a *white vertex v* is discovered, it is added to the tree with the edge (u,v) that connects it with it's predecessor u.
The BFS algorithm uses a Queue (FIFO) to manage discovered (gray) vertices.

### Pseudo-code
| Property | Description              | Default Value |
|----------|--------------------------|---------------|
| color    | discovery status         | white         |
| pi       | predecessor vertex       | NULL          |
| d        | distance from source vtx | INFINITE      |

*A predecessor is represented by the greek letter PI*

```
// initialization
for each vtx in Graph G except source s
    u.color = w
    u.pi = null
    u.d = 0

// start procedure
s.color = g
s.pi = null
s.d = 0

Queue Q
Enqueue(Q,s)

// traversing
while Q is not empty
    // removes u from traversing queue
    u = dequeue Q

    // visits u decendents
    for each v adjacent to u
        if v.color == w
            v.color = g
            v.pi = u
            v.d = u.d + 1
            Enqueue(Q,v)

    u.color = b // can be removed
```

## Single Source Shortest Path Problem
BFS can be used to evaluate the shortest paths to each reachable vertex from source vertex s.

> Shortest-Path = minimum number of edges from s to v = gamma(s,v)

BFS properly calculates the shortest path between s and v as the property d to each vertex.

## Breadth-First Tree
BFS calculates the BF-tree as the property pi to each vertex = predecessor subgraph.