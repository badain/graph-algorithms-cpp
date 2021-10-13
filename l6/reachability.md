# Reachability
*Definition:* Reachability is a property assigned to a pair of vertices from a graph. A vertex t is **reachable** from vertex s if there is a *st-walk* in the graph.

```
t is reachable from s
s ~ t  (graph)
s ~> t (digraph)
```

*Reachability is useful to indicate relatioships between vertices in a graph*

## Reachability Properties
### Digraphs D
1. Reflexive: `v ~> v`
2. Transitive: `x ~> y` and `y ~> z` imply `x ~> z`
### Graphs G
1. Equivalence: `v ~> u` implies `u ~> v` 