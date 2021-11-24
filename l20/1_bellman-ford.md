# The Bellman-Ford Algorithm

The Bellman-Ford Algortihm solves the single-source shortest path problem and accepts negative edge weights.

| Input        | Output                                             |
|--------------|----------------------------------------------------|
| G(V, E)      | boolean: negativeCycleExists                       |
| w(E): E -> R | !negativeCycleExists -> returns Shortest-Path Tree |

```
BELLMAN-FORD (G, w, s):
    INITIALIZE-SINGLE-SOURCE(G,s) // vtx tags initialization
    
    // relax each edge |V| - 1 times
    for i in |V| - 1:
        for edge(u,v) in G:
            RELAX(u, v, w)
    
    // checks existence of negativeCycle
    for edge(u,v) in G:
        if v.d > u.d+w(u,v)
            return FALSE
    return TRUE
```