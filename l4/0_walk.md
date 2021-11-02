# Walk
Walk is a *sequence* `W=<v0, v1, ..., vk>` of vertices. For each `{1, ..., i}` we have the following pairs:

| Digraph          | Graphs           |
|------------------|------------------|
| **(v(i-1), vi)** | **(v(i-1), vi)** |
| archs            | edges            |
| ordered          | unordered        |

### Lenght
The lenght of a walk is (1 - len(vertices)) = k. **NUMBER OF EDGES**

### Naming
A walk from vertice **s to vertice t** is a **st-walk**.

## Path
A walk is a **path** if every vertex in W is pairwise distinct. **NO REPETITIONS**

> Hamiltonian Path = Traverses all vertices in a graph

### Subpath
Q is a subpath of P if there is a index **m** of P that indicates the start of the Path Q;

Q <= P;
```
P = <v0, v1, ..., v(m), v(m+1), v(m+k), ...>
Q =             < u(0), u(1)  , u(k)  >
```
> example: v(13) = u(0); v(14) = u(1); ...

## Properties
1. Every graph G has a path P where **len(P) >= minimum_degree(G)**
2. A path P is **maximal** is there is no other path to which P is a subpath. (P cannot be a subpath)
> Degree: # of edges incident to a vertice

> Maximal does not means longer! Just means that the specific path cannot be longer. A path in which you cannot proceed without turning into a walk.