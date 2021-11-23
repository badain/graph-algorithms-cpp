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
**Subpaths of shortest paths are shortest paths**
**Shortest paths contains no cycles**