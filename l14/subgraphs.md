# Subgraphs
> Specific to Undirected Graphs

A Graph H = (U, F) is a **subgraph** of G = (V, E) if:
```
(V contains U) && (E contains F)
```
*That is: subvertices set is contained in V and subedges set is contained in E*

A Graph H = (U, F) is a **spanning** of G = (V, E) if:
```
(V equals U) && (E contains F)
```
*Same vertices, less edges*

> An edge {u,v} is induced by a set S of vtxs if u, v are both in S. 

A Subgraph H = (U, F) of G = (V, E) is **induced** by U if:
```
Every edge in F is a induced G edge by U
H = G[U]
```
*That is, H is a subgraph induced by U if its edges are only related to vtxs in U*~

### Vertex/Edge Deletion
G[ U/F ] is a graph induced by (U/F), that is, U without F.


## 10 - I need to learn connections