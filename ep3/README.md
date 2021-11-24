# EP03 Loophole
Rafael Badain - 10277102

1. **Model the Loophole Problem as a negative cycle detection**

    A loophole cycle is defined as cycle where `w(1) * w(2) * ... * w(n) > 1` (Eq. 1) (where the weight represents currency exchange rate)

    It's not trivial to evaluate this property in a digraph, but, since the Bellman-Ford Algorithm can identify negative-weight cycles:
    ```
    w(1) * w(2) * ... * w(n) > 1 =>
    log(w(1)) * log(w(2)) * ... * log(w(3)) > 0 =>
    -log(w(1)) * -log(w(2)) * ... * -log(w(3)) < 0
    ```
    Using the negative log of the weights, the Bellman-Ford Alggorithm can identify the existence of loophole cycles.

2. **Solve the Loophole Problem using the Bellman-Ford Algorithm**

    **Input**

    Set V of exchangeable currencies. There is at most one possible way of exchanging a ordered pair of currencies.

    **Exchange**

    Each vertice stores the cumulative path weight of the shortest path.
    ```
    u -> v: u = c(u,v) > 0 *[one u is equivalent to c(u,v) units of v]*
    u -> r -> s: u = c(u,r) = c(u,r)*c(r,s)
    ```

    **Loophole**

    A loophole is a sequence of currencies <v0, v1, v2, ..., vn>:
    1. n >= 1
    2. the sequence is pairwise distinct
    3. v0 = vn (it's a cycle)
    4. PROD(i=1 -> n)[c(vi-1, vi)] > 1 (that is, the product of every pair c(vn-1, vn) is bigger than one) (ends with more unities than the begging)

    **Feasible Multiplier**

    If a loophole doesn't exists, than a **feasible multiplier** function z: V -> R>0 must be found, where z(v) >= z(u)*c(u,v) for each pair of uv where c(uv) is defined.

    *Theorem 24.9*
    
    A Feasible Solution to a graph G with no negative-weight cycles can be obtained by adding a extra vertex v0, that is connected to each one of the original vertices by a 0-weighted edge.
    
    The Feasible Solution is given by `x = ( w(v0, v1), w(v0, v2), w(v0, v3), ..., w(v0, vn) )`

    Any negative-weight cycle in the constraint graph is reachable from v0, thus, if Bellman-Ford returns true then the shortest-path weight is the feasible solution.