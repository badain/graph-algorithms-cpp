# DFS on Graphs

Depth-First Search can be adapted to *undirected graphs* by using the **associated symetric digraph**.

> Associated Symetric Digraph: The same graph as a directed digraph, where edges are replaced by bilateral arcs.

## DFS and Components
DFS can be used to compute *components* of a graph G related to a vertex v.

## Edges Classification
In a DFS-visit of a undirected graph using a *associated symetric digraph*, whenever a arc (u,v) is laballed, its anti-parallel siblings should be labelled exacly the same. (Since they both represent the same edge in the original undirected graph)

> In a DFS of a graph G, every edge of G is either a tree edge or a back edge.