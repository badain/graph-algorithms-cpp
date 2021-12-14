# Maximum Flow
Directed graphs can model problems as *flow networks* based on the **rate** in which a material travels from a source to a destination.

- Each directed edge has a **capacity** = flow rate;
- Material does not collect in vertices = "flow conservation" => (enter rate = leave rate)

## The Maximum Flow Problem
What is the maximum flow at which a material can be transported from a source to a destination, without violating any constraints?

# Flow Networks
- **Flow Network:** G = (V,E) is a directed graph
- Each edge (u,v) from E has a **capacity** c(u,v) >= 0
- (Reverse edges are disallowed in this definition, but in the asgt definition we have mandatory digraph audmentation)
- c(u,v) = 0 if (u,v) doesn't exists + no self loops
- **source s**: start vector
- **sink t**: end vector
- exists a path from the source to the sink, and each vertex v in the digraph is between both s and t: `s -> v -> t`

## Flow function f(u,v)
Is a nonnegative funtion of two vertices that maps the current flow between the two vertices.

**Constrained:** f(u,v) is always constrained by the edge capacity c(u,v), that is: `0 <= f(u,v) <= c(u,v)`
**Conserved:** The sum of every flow between every (u,v) pair in V equals the sum of (v,u).

If there is no edge between (u,v) then f(u,v) = 0

### Flow value
|f| is defined as the summation of the flow out of the source minus the summation of the flow in of the source

## Ford-Fulkerson
A method to solve the Maximum Flow Problem, which has 3 important paradigms:
1. residual networks
2. augmenting paths
3. cuts
Flow starts at 0, and is interactively incremented based on augmented paths in residual networks

```
ford-fulkerson-method(G,s,t)
    flow = 0
    while there is (augmenting path p in residual network Gf)
        augument flow f along p
    return f
```

### Residual networks
Residual networks Gf of a Graph G and flow f are the edges in which there is **residual capacity**, that is, there is stil space available to flow increments.

**Residual capacity:** cf(u,v) = c(u,v) - f(u,v)

### Augmenting Path
Is a path from the residual network Gf that allows flow increments.

**Residual capacity** of a path is the smallest residual capacity in the path.

### Cuts
**cut:** The cut(S,T) of a flow network G(V,E) is a partition of V into S and T, such as no vertex is left behind.

*capacity of a cut:* the summation of the capacity of the edges in the imminence of a cut, from S to T

*26.5:* every flow f in G is bounded by the capacity of any cut of G

> the maximum flow is bounded from above by the capacity of a minimum cut of G

**max-flow min-cut theorem:**
- |f| = c(S,T) for some cut (S,T) of G
- no augmenting paths in residual networks
