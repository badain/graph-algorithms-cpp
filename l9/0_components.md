# Components

## Component Definition
The **component** of a graph G=(V,E) containing a vertex v is the set of vertices in G that are reachable from v
```
comp(v) = {x e V : v ~ x}
```
That is, every x in V where there is a vx-walk in G.
~
## Connected Components of a Graph
The **connected components of G** are the set of the component of every vertice in V.
```
{comp(v) : v e V}
```
> The set of components of G is a **partition** of V. Components are like jigsaw puzzles with repeated pieces that make a set V.

### Partition
A *partition* of a set S is the collection of subsets Bn of S that are **pairwise disjoint** such as
```
B1 U B2 U ... U Bn = S
```

## Equivalence
The component concept can be abstracted as **equivalence classes**.

For an arbitrary *equivalence relation* between vertices **~** of a set S, an **equivalence class** is defined as the set of elements of S that are related to s:
```
[s] := {x e S : x ~ s}, for s e S
```

Similarly to **connected components**, the set of *equivalence classes* is a partition of S.

> The components of a graph are *equivalence classes* under the *equivalence relation* ~ and are partitions of V