# Single-Source Shortest Path
Starting from a single source vertex, whats the shortest path to a arbitrary reachable vertex?

*Input:* Weighted directed graph G = (V, E).

This is an **Optimization Problem**

## Definition

Weight w(E) maps E edges to Real numbers. The weight of a path p w(p) is given by the sum of the weights of its edges.

The *shortest-path weight del(u,v)* is the `min(w(p))` shortest weight from the set od paths from u to v. If none exists, its infinite.

The *shortest-path from u to v* is defined as the path such as w(p) = del(u,v

> Finding the shortest path is minimizing the edge weight

## Useful Properties
- **Subpaths of shortest paths are shortest paths**
- **Shortest paths contains no cycles**

## Representation
Shortest Paths are represented by a Shortest-Path Tree, a subgraph from G in which:
- Every vertex is reachable;
- Every vertex is connected to the source at the base of the tree by its shortest path

> The Shortest-Path Tree represents the original Graph without the clutter

## Relaxation
Each vertex v, except from the source s, is tagged with the property d: the tentative distance from s. Upon discovery, v is tagged with its proper increased the distance.

```
INITIALIZE SINGLE-SOURCE(G, s):
for each vertex v in G.V:
    v.d = INFINITE
    v.pi = NULL
s.d = 0
```

The **relaxation** technique evaluates if there is a shortest possible distance by updating its property d every time a shortest path is found.

```
RELAX(u, v, weight())
if v.d > u.d + w(u,v):  // if there is a shortest path between u and v (if previous distance is larger than new distance)
    v.d = u.d + w(u,v)  // v is tagged with a new shortest distance
    v.pi = u            // u is the proper v predecessor in the Shortest Path Tree
```

| Algorithm    | # each edge is relaxed |
|--------------|------------------------|
| Bellman-Ford | \|V\| - 1 = n(E)         |
| Dijkstra     | 1                      |
